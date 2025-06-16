#include "Motor.h"

#include <Arduino.h>
#include <iostream>
#include "configuracion.h"

Motor::Motor(int in1, int in2, int enable, int cs_encoder) {
    // Inicializar el regulador PID
    _regulador = nullptr;
    
    // Inicializar el motor con los pines de control
    _in1 = in1;
    _in2 = in2;
    _en = enable;
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(enable, OUTPUT);

    // Inicializar el encoder
    _encoder = new Encoder(cs_encoder);
    _encoder->begin();
}

float Motor::leerGrados()
{
    return _encoder->leerGrados();
}

void Motor::setRegulador(float Kp, float Ki, float Kd, float Tm) {
    _regulador = new RegPID(Kp, Ki, Kd, Tm);
}

// Resetear el encoder a cero
bool Motor::resetEncoder() {
    return _encoder->setAMT203Zero();
}

void Motor::ControlPID_Motor(float pos_des, float Kp, float Ki, float Kd, int pwm_mant) {
    // Configuración PID mejorada
    float error_anterior = 0.0;
    float integral = 0.0;
    unsigned long tiempo_anterior = millis();
    
    // Tolerancias ajustadas dinámicamente
    const float tolerancia_movimiento = 1.0;
    const float tolerancia_estable = 1.0;
    const float max_integral = 250.0;
    
    // Nuevos parámetros para mejor control
    const int pwm_min_d_in1mico = 80;  // PWM mínimo dinámico (mayor que el estático)
    const float zona_transicion = 10.0; // Zona donde comienza a reducirse el PWM
    const float factor_boost = 2;    // Factor para aumentar Kp cerca del objetivo
    
    // Contadores para estabilidad
    int contador_estable = 0;
    const int min_tiempo_estable = 20;

    Serial.println("Iniciando control PID mejorado...");

    while (true) {
        float posicion = _encoder->leerGrados();
        if (posicion == -1) {
            delay(5);
            continue;
        }

        // Normalizar ángulos para encoder circular
        float error = pos_des - posicion;
        if (error > 180) error -= 360;
        else if (error < -180) error += 360;

        // Tiempo para PID
        unsigned long tiempo_actual = millis();
        float dt = (tiempo_actual - tiempo_anterior) / 1000.0;
        tiempo_anterior = tiempo_actual;
        if (dt <= 0.0 || dt > 0.1) dt = 0.01;

        // Término integral con anti-windup
        integral += error * dt;
        integral = constrain(integral, -max_integral, max_integral);

        // Término derivativo con filtrado
        float derivada = (error - error_anterior) / dt;
        error_anterior = error;

        // Ganancia adaptativa cerca del objetivo
        float Kp_adaptativo = Kp;
        if (abs(error) < zona_transicion) {
            Kp_adaptativo = Kp * factor_boost;
        }

        // Cálculo PID
        float salida = Kp_adaptativo * error + Ki * integral + Kd * derivada;

        // Verificar si está en posición
        if (abs(error) <= tolerancia_movimiento) {
            contador_estable++;

            if (contador_estable >= min_tiempo_estable) {
                int pwm_mantenimiento= pwm_min_d_in1mico + abs(error) * 15;
                    pwm_mantenimiento = constrain(pwm_mantenimiento, pwm_min_d_in1mico, 150);

                if (abs(error) > tolerancia_estable) {
                    // Control direccional con PWM dinámico
                    if (error > 0) {
                        digitalWrite(_in1, LOW);
                        digitalWrite(_in2, HIGH);
                    } else {
                        digitalWrite(_in1, HIGH);
                        digitalWrite(_in2, LOW);
                    }
                    analogWrite(_en, pwm_mantenimiento);
                } else {
                    // Mantener posición con PWM mínimo
                    analogWrite(_en, pwm_mant);
                    Serial.println("Posición alcanzada y estable. Manteniendo posición.");
                    break;
                }

                Serial.printf("MANTENIENDO - Pos: %.2f°, Obj: %.2f°, Err: %.2f°, PWM: %d\n",
                             posicion, pos_des, error, pwm_mantenimiento);
            }
        } else {
            contador_estable = 0;

            // Control PID normal con PWM dinámico
            int duty = abs((int)salida);
            
            // Asegurar un mínimo de PWM para vencer fricción estática
            if (duty < pwm_min_d_in1mico && duty > 0) {
                duty = pwm_min_d_in1mico;
            }
            
            duty = constrain(duty, pwm_min_d_in1mico, 255);

            // Determ_in1r dirección
            if (salida > 0) {
                digitalWrite(_in1, LOW);
                digitalWrite(_in2, HIGH);
            } else {
                digitalWrite(_in1, HIGH);
                digitalWrite(_in2, LOW);
            }

            analogWrite(_en, duty);

            Serial.printf("MOVIENDO - Pos: %.2f°, Obj: %.2f°, Err: %.2f°, PWM: %d, I: %.2f\n",
                         posicion, pos_des, error, duty, integral);
        }

        delay(10);
    }

    Serial.println("Control PID completado.");
}

void Motor::mover() {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    if (_en >= 0 && _en <= 33) {
        analogWrite(_en, PWM_VELOCIDAD);
    }
}

void Motor::moverPWM(float pwm) {
    // PARA CAMBIAR EL SENTIDO DE GIRO DEL MOTOR
    if (pwm > 0) {
        digitalWrite(_in1, HIGH);
        digitalWrite(_in2, LOW);
    } else {
        digitalWrite(_in1, LOW);
        digitalWrite(_in2, HIGH);
        pwm = -pwm;
    }
    if (_en >= 0 && _en <= 33) {
        analogWrite(_en,pwm);
    }
    
    // digitalWrite(_in1, HIGH);
    // digitalWrite(_in2, LOW);
    // if (_en >= 0 && _en <= 33) {
    //     analogWrite(_en, pwm);
    // }
}

void Motor::detener() {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
    analogWrite(_en, 0);
}

void Motor::setFCTriggered(bool triggered) {
    _fcTriggered = triggered;
}

bool Motor::kick_inicial_mejorado(float objetivo, int pwm_mant) {

    float posicion_actual = _encoder->leerGrados();
    if (posicion_actual == -1) {
        Serial.println("Error leyendo encoder en kick inicial.");
        return false;
    }

    // if (objetivo > posicion_actual) {
    //     return false;
    // } // Verificar esta línea, lo que quiero evitar es que se produzca el kick cuando el hombro baja

    float error_inicial = objetivo - posicion_actual;

    // Normalizar ángulo
    if (error_inicial > 180)
        error_inicial -= 360;
    else if (error_inicial < -180)
        error_inicial += 360;

    // Determinar dirección del kick
    if (error_inicial > 0) {
        digitalWrite(_in1, LOW);
        digitalWrite(_in2, HIGH);
    } else {
        digitalWrite(_in1, HIGH);
        digitalWrite(_in2, LOW);
    }

    if (error_inicial > 20)
    {
        // Kick progresivo 
        Serial.println("Iniciando kick para error > 20...");

        // Fase 1: Rampa de subida suave (500ms)
        for (int pwm = pwm_mant; pwm <= 255; pwm += 50) {
            analogWrite(_en, pwm);
            delay(25); // 25ms por paso = 500ms total

            // Verificar si ya se está moviendo
            float pos_temp = _encoder->leerGrados();
            if (pos_temp != -1) {
                float mov_temp = abs(pos_temp - posicion_actual);
                if (mov_temp > 180)
                    mov_temp = 360 - mov_temp;
                if (mov_temp > 3.0) {
                    Serial.printf("Motor respondió en PWM %d, movimiento: %.2f°\n", pwm, mov_temp);
                    break;
                }
            }
        }

        // Fase 2: Mantener PWM constante por más tiempo (1000ms)
        analogWrite(_en, 255);
        delay(1000);
        // Fase 3: Rampa de bajada suave (300ms)
        for (int pwm = 255; pwm >= pwm_mant; pwm -= 10) {
            analogWrite(_en, pwm);
            delay(20); // 20ms por paso
        }
        // Fase 4: PWM mínimo por un momento para estabilizar
        analogWrite(_en, pwm_mant);
        delay(200);
        // Verificar que se movió significativamente
        float posicion_post_kick = _encoder->leerGrados();
        if (posicion_post_kick == -1) {
            analogWrite(_en, 0);
            return false;
        }
        float movimiento = abs(posicion_post_kick - posicion_actual);
        if (movimiento > 180)
            movimiento = 360 - movimiento; // Normalizar
        if (movimiento > 20.0) {
            Serial.printf("Kick suave exitoso. Movimiento total: %.2f°\n", movimiento);
            Serial.printf("Posición inicial: %.2f° -> Posición final: %.2f°\n", posicion_actual, posicion_post_kick);
            return true;
        } else {
            Serial.printf("Kick insuficiente (movimiento: %.2f°), reintentando...\n", movimiento);
            analogWrite(_en, 0); // Asegurar que se detenga
            return false;
        }

    } else {
        Serial.println("Iniciando kick progresivo");
        // Fase 1: Rampa de subida suave (500ms)
        for (int pwm = pwm_mant; pwm <= 180; pwm += 10) {
            analogWrite(_en, pwm);
            delay(25); // 25ms por paso = 500ms total   
            // Verificar si ya se está moviendo
            float pos_temp = _encoder->leerGrados();
            if (pos_temp != -1) {
                float mov_temp = abs(pos_temp - posicion_actual);
                if (mov_temp > 180)
                    mov_temp = 360 - mov_temp;
                if (mov_temp > 3.0) {
                    Serial.printf("Motor respondió en PWM %d, movimiento: %.2f°\n", pwm, mov_temp);
                    break;
                }
            }
        }

        // Fase 2: Mantener PWM constante por más tiempo (800ms)
        analogWrite(_en, 180);
        delay(800);

        // Fase 3: Rampa de bajada suave (300ms)
        for (int pwm = 180; pwm >= pwm_mant; pwm -= 10) {
            analogWrite(_en, pwm);
            delay(20); // 20ms por paso
        }
        // Fase 4: PWM mínimo por un momento para estabilizar
        analogWrite(_en, pwm_mant);
        delay(200);
        // Verificar que se movió significativamente
        float posicion_post_kick = _encoder->leerGrados();
        if (posicion_post_kick == -1) {
            analogWrite(_en, 0);
            return false;
        }
        float movimiento = abs(posicion_post_kick - posicion_actual);
        if (movimiento > 180)
            movimiento = 360 - movimiento; // Normalizar
        if (movimiento > 15.0) {
            Serial.printf("Kick progresivo exitoso. Movimiento total: %.2f°\n", movimiento);
            Serial.printf("Posición inicial: %.2f° -> Posición final: %.2f°\n", posicion_actual, posicion_post_kick);
            return true;
        } else {
            Serial.printf("Kick insuficiente (movimiento: %.2f°), reintentando...\n", movimiento);
            analogWrite(_en, 0); // Asegurar que se detenga
            return false;
        }

    }

}


// Función de kick inicial fuera de la clase
// Función de kick inicial mejorada
// bool kick_inicial_mejorado(float error_inicial, int en, int inA, int inB, int csEncoder, int pwm_min)
// {
//     float posicion_actual = leerEncoder(csEncoder);
//     // if (posicion_actual == -1)
//     // {
//     //     Serial.println("Error leyendo encoder en kick inicial.");
//     //     return false;
//     // }

//     // float error_inicial = objetivo - posicion_actual;

//     // Normalizar ángulo
//     if (error_inicial > 180)
//         error_inicial -= 360;
//     else if (error_inicial < -180)
//         error_inicial += 360;

//     // Determinar dirección del kick
//     if (error_inicial > 0)
//     {
//         digitalWrite(inA, LOW);
//         digitalWrite(inB, HIGH);
//     }
//     else
//     {
//         digitalWrite(inA, HIGH);
//         digitalWrite(inB, LOW);
//     }

//     if (error_inicial > 20)
//     {
//         // Kick progresivo y suave - rampa de subida
//         Serial.println("Iniciando kick progresivo...");

//         // Fase 1: Rampa de subida suave (500ms)
//         for (int pwm = pwm_min; pwm <= 255; pwm += 85)
//         {
//             analogWrite(en, pwm);
//             delay(25); // 25ms por paso = 500ms total

//             // Verificar si ya se está moviendo
//             float pos_temp = leerEncoder(csEncoder);
//             if (pos_temp != -1)
//             {
//                 float mov_temp = abs(pos_temp - posicion_actual);
//                 if (mov_temp > 180)
//                     mov_temp = 360 - mov_temp;
//                 if (mov_temp > 3.0)
//                 {
//                     Serial.printf("Motor respondió en PWM %d, movimiento: %.2f°\n", pwm, mov_temp);
//                     break;
//                 }
//             }
//         }

//         // Fase 2: Mantener PWM constante por más tiempo (1000ms)
//         analogWrite(en, 255);
//         delay(1000);

//         // Fase 3: Rampa de bajada suave (300ms)
//         for (int pwm = 255; pwm >= pwm_min; pwm -= 10)
//         {
//             analogWrite(en, pwm);
//             delay(20); // 20ms por paso
//         }

//         // Fase 4: PWM mínimo por un momento para estabilizar
//         analogWrite(en, pwm_min);
//         delay(200);

//         // Verificar que se movió significativamente
//         float posicion_post_kick = leerEncoder(csEncoder);
//         if (posicion_post_kick == -1)
//         {
//             analogWrite(en, 0);
//             return false;
//         }

//         float movimiento = abs(posicion_post_kick - posicion_actual);
//         if (movimiento > 180)
//             movimiento = 360 - movimiento; // Normalizar

//         if (movimiento > 20.0)
//         {
//             Serial.printf("Kick suave exitoso. Movimiento total: %.2f°\n", movimiento);
//             Serial.printf("Posición inicial: %.2f° -> Posición final: %.2f°\n", posicion_actual, posicion_post_kick);
//             return true;
//         }
//         else
//         {
//             Serial.printf("Kick insuficiente (movimiento: %.2f°), reintentando...\n", movimiento);
//             analogWrite(en, 0); // Asegurar que se detenga
//             return false;
//         }
//     }
//     else
//     {
//         // Kick progresivo y suave - rampa de subida
//         Serial.println("Iniciando kick progresivo...");

//         // Fase 1: Rampa de subida suave (500ms)
//         for (int pwm = pwm_min; pwm <= 250; pwm += 20)
//         {
//             analogWrite(en, pwm);
//             delay(25); // 25ms por paso = 500ms total

//             // Verificar si ya se está moviendo
//             float pos_temp = leerEncoder(csEncoder);
//             if (pos_temp != -1)
//             {
//                 float mov_temp = abs(pos_temp - posicion_actual);
//                 if (mov_temp > 180)
//                     mov_temp = 360 - mov_temp;
//                 if (mov_temp > 3.0)
//                 {
//                     Serial.printf("Motor respondió en PWM %d, movimiento: %.2f°\n", pwm, mov_temp);
//                     break;
//                 }
//             }
//         }

//         // Fase 2: Mantener PWM constante por más tiempo (800ms)
//         analogWrite(en, 200);
//         delay(800);

//         // Fase 3: Rampa de bajada suave (300ms)
//         for (int pwm = 200; pwm >= pwm_min; pwm -= 10)
//         {
//             analogWrite(en, pwm);
//             delay(20); // 20ms por paso
//         }

//         // Fase 4: PWM mínimo por un momento para estabilizar
//         analogWrite(en, pwm_min);
//         delay(200);

//         // Verificar que se movió significativamente
//         float posicion_post_kick = leerEncoder(csEncoder);
//         if (posicion_post_kick == -1)
//         {
//             analogWrite(en, 0);
//             return false;
//         }

//         float movimiento = abs(posicion_post_kick - posicion_actual);
//         if (movimiento > 180)
//             movimiento = 360 - movimiento; // Normalizar

//         if (movimiento > 15.0)
//         {
//             Serial.printf("Kick suave exitoso. Movimiento total: %.2f°\n", movimiento);
//             Serial.printf("Posición inicial: %.2f° -> Posición final: %.2f°\n", posicion_actual, posicion_post_kick);
//             return true;
//         }
//         else
//         {
//             Serial.printf("Kick insuficiente (movimiento: %.2f°), reintentando...\n", movimiento);
//             analogWrite(en, 0); // Asegurar que se detenga
//             return false;
//         }
//     }
// }