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

// void Motor::moverAGrados(double gradosRelativos) {
//     std::cout<<"Moviendo a grados: "<<gradosRelativos<<std::endl;

//     // TODO Verificar interrupciones activadas

//     // TODO Verificar finales de carrera
    
//     float inicio = _encoder->leerGrados();
//     if (inicio < 0) {
//         return;
//     }

//     float destino = fmod(inicio + gradosRelativos + 360.0, 360.0);
//     bool sentidoHorario = gradosRelativos <= inicio;
//     if(sentidoHorario){
//         std::cout<<"Sentido horario"<<std::endl;
//     }
//     else{
//         std::cout<<"Sentido antihorario"<<std::endl;
//     }

//     digitalWrite(_in1, sentidoHorario ? HIGH : LOW);
//     digitalWrite(_in2, sentidoHorario ? LOW : HIGH);

//     float error = 0;

//     while (!_fcTriggered) {
//         float actual = _encoder->leerGrados();
//         //         Serial.print(">"); Serial.print(_encoder->id()); Serial.print(": ");
//         // Serial.println(actual);
//         if (actual < 0) break;

//         // Cálculo de error
//         error=destino-actual;
//         if (sentidoHorario) {
//             error = (destino > inicio) ? (destino - actual) : 
//                    ((actual < inicio) ? (destino + 360.0 - actual) : (destino - actual));
//         } else {
//             error = (destino < inicio) ? (destino - actual) : 
//                    ((actual > inicio) ? (destino - 360.0 - actual) : (destino - actual));
//         }

//         Serial.print(">"); Serial.print(_encoder->id()); Serial.print(": ");
//         Serial.println(error);

//         // Control PID
//         int pwm = PWM_VELOCIDAD;
//         if (_regulador) {
//             pwm = constrain(abs(_regulador->calcularUk(error)), pwm_mant, PWM_MAX);
            
//             // Ajustar dirección si es necesario
//             bool nuevoSentido = error >= 0;
//             if (nuevoSentido != sentidoHorario) {
//                 sentidoHorario = nuevoSentido;
//                 digitalWrite(_in1, sentidoHorario ? HIGH : LOW);
//                 digitalWrite(_in2, sentidoHorario ? LOW : HIGH);
//             }
//         }

//         analogWrite(_en, pwm);

//         // Condición de parada por precisión
//         if (abs(error) < 1.0) break;

//         //TODO Verificación periódica de finales de carrera

//         delay(5);
//     }

//     detener();
//     _fcTriggered = false;
// }

// Resetear el encoder a cero
bool Motor::resetEncoder() {
    // Resetear el encoder a cero
    return _encoder->setAMT203Zero();
}

// CONTROL PID NORMALIZADO
// void Motor::ControlPID_Motor(float pos_des, float Kp, float Ki, float Kd, int pwm_mant)
// {
//     // Inicialización PID
//     float error_anterior = 0.0;
//     float integral = 0.0;
//     unsigned long tiempo_anterior = millis();

//     const float tolerancia = 1.0; // grados

//     // Leer posición inicial
//     float posicion_actual = _encoder->leerGrados();
//     if (posicion_actual == -1)
//     {
//         Serial.println("Error leyendo encoder.");
//         return;
//     }

//     // Ajuste circular (opcional, depende del encoder)
//         if ((posicion_actual - pos_des) < -180)
//             posicion_actual += 360;
//         else if ((posicion_actual - pos_des) > 180)
//             posicion_actual -= 360;
//     float err_deg = pos_des - posicion_actual;
 
//         if (err_deg > 180)
//             err_deg -= 360;
//         else if (err_deg < -180)
//             err_deg += 360;


//     if (abs(err_deg) <= tolerancia)
//     {
//         analogWrite(_en, pwm_mant); // Asegurar que el motor se detenga con un PWM mínimo
//         Serial.printf("PWM enviado: %d\n   |   ", pwm_mant);
//         Serial.println("Ya en posición.");
//         return;
//     }

//     Serial.println("Iniciando control PID...");

//     // Bucle de control PID
//     while (true)
//     {
//         float posicion = _encoder->leerGrados();
//         if (posicion == -1)
//             continue;

//             if ((posicion - pos_des) < -180)
//                 posicion += 360;
//             else if ((posicion - pos_des) > 180)
//                 posicion -= 360;

//         float error = pos_des - posicion;

//             if (error > 180)
//                 error -= 360;
//             else if (error < -180)
//                 error += 360;

//         // Tiempo para PID
//         unsigned long tiempo_actual = millis();
//         float dt = (tiempo_actual - tiempo_anterior) / 1000.0; // en segundos
//         tiempo_anterior = tiempo_actual;

//         if (dt <= 0.0)
//             dt = 0.001; // protección división por cero

//         // PID
//         integral += error * dt;
//         float derivada = (error - error_anterior) / dt;
//         float salida = Kp * error + Ki * integral + Kd * derivada;
//         error_anterior = error;

//         // PWM dinámico
//         int duty = abs((int)salida);
//         duty = constrain(duty, 0, 255); // PWM válido para Arduino

//         // Dirección
//         if (salida > 0)
//         {
//             digitalWrite(_in1, LOW);
//             digitalWrite(_in2, HIGH);
//         }
//         else
//         {
//             digitalWrite(_in1, HIGH);
//             digitalWrite(_in2, LOW);
//         }

//         analogWrite(_en, duty);

//         Serial.printf("Pos: %.2f°, Obj: %.2f°, Err: %.2f°, PWM: %d\n", posicion, pos_des, error, duty);

//         // Condición de parada
//         if (abs(error) <= tolerancia)
//         {
//             analogWrite(_en, pwm_mant); // Asegurar que el motor se detenga con un PWM mínimo
//             break;
//         }
//         delay(10); // 10 ms de espera
//     }

//     Serial.println("Movimiento completado.");
// }

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
    const int pwm_min_dinamico = 30;  // PWM mínimo dinámico (mayor que el estático)
    const float zona_transicion = 3.0; // Zona donde comienza a reducirse el PWM
    const float factor_boost = 1.5;    // Factor para aumentar Kp cerca del objetivo
    
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
                int pwm_mantenimiento= pwm_min_dinamico + abs(error) * 15;
                    pwm_mantenimiento = constrain(pwm_mantenimiento, pwm_min_dinamico, 150);

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
            if (duty < pwm_min_dinamico && duty > 0) {
                duty = pwm_min_dinamico;
            }
            
            duty = constrain(duty, pwm_min_dinamico, 255);

            // Determinar dirección
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
    // if (pwm > 0) {
    //     digitalWrite(_in1, HIGH);
    //     digitalWrite(_in2, LOW);
    // } else {
    //     digitalWrite(_in1, LOW);
    //     digitalWrite(_in2, HIGH);
    //     pwm = -pwm;
    // }
    // if (_en >= 0 && _en <= 33) {
    //     analogWrite(_en,pwm);
    // }
    // }
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    if (_en >= 0 && _en <= 33) {
        analogWrite(_en, pwm);
    }
}

void Motor::detener() {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
    analogWrite(_en, 0);
}

// bool Motor::kick_inicial()
// {
//     float objetivo_kick = 0;

//     if (objetivo > 0)
//     {
//         objetivo_kick = objetivo - 5; // Ajuste para el kick inicial
//     }
//     else
//     {
//         objetivo_kick = objetivo + 5; // Ajuste para el kick inicial
//     }

//     moverMotor_maxVel(objetivo_kick, en, inA, inB, csEncoder);

//     return true;
// }

void Motor::setFCTriggered(bool triggered) {
    _fcTriggered = triggered;
}