#include "Motor.h"
#include "funciones_cinem.h"
#include "configuracion.h"
#include "MyServer.h"

// Posición de los pines de los encoders
#define CS_BASE 10
#define CS_HOMBRO 41
#define CS_CODO 38

Motor *_base, *_hombro, *_codo;

void IRAM_ATTR onFallingFC()
{
    // Verificar si los finales de carrera están activados
    if (digitalRead(fc1) == LOW || digitalRead(fc2) == LOW || digitalRead(fc3) == LOW || digitalRead(fc4) == LOW)
    {
        // Manejo de interrupciones de finales de carrera
        _base->setFCTriggered(true);
        _hombro->setFCTriggered(true);

        float impulso=0;

        if(digitalRead(fc1) == LOW || digitalRead(fc3) == LOW){
            impulso=5;
        }
        if(digitalRead(fc2) == LOW || digitalRead(fc4) == LOW){
            impulso=-5;
        }

        // _base->moverAGrados(impulso);
        // _hombro->moverAGrados(impulso);

    }
    else
    {
        return; // No hacer nada si no se ha activado ningún final de carrera
    }
}

void setup()
{
    Serial.begin(115200);
    // Configuración Servidor WiFi
    // mutex_setup(); // Inicializar el mutex para proteger las medidas
    // delay(1000);
    // xTaskCreate(mantener_wifi, "Mantiene la conexión WiFi activa", 20000, NULL, 5, NULL); // Crear el hilo para mantener la conexión WiFi activa
    // printf("Tarea 1 lanzada\n");
    // xTaskCreate(recibir_mensaje, "Recibe e imprime por pantalla los datos enviados por el cliente", 20000, NULL, 5, NULL); // Crear el hilo para recibir mensajes
    // printf("Tarea 2 lanzada\n");
    
    // Inicialización bus SPI para los encoders
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV32);

    // Finales de carrera
    pinMode(fc1, INPUT_PULLUP);
    pinMode(fc2, INPUT_PULLUP);
    pinMode(fc3, INPUT_PULLUP);
    pinMode(fc4, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(fc1), onFallingFC, FALLING);
    attachInterrupt(digitalPinToInterrupt(fc2), onFallingFC, FALLING);
    attachInterrupt(digitalPinToInterrupt(fc3), onFallingFC, FALLING);
    attachInterrupt(digitalPinToInterrupt(fc4), onFallingFC, FALLING);

    // Crear los motores
    _base = new Motor(IN1_M1, IN2_M1, EN_M1, CS_BASE);
    //_base->setRegulador(Kp_base, Ki_base, Kd_base, Ts);

    _hombro = new Motor(IN1_M2, IN2_M2, EN_M2, CS_HOMBRO);
    //_hombro->setRegulador(Kp_hombro_subir, Ki_hombro,Kd_hombro, Ts); // Hace falta?

    _codo = new Motor(IN1_M3, IN2_M3, EN_M3, CS_CODO);
    //_codo->setRegulador(Kp_codo, Ki_codo, Kd_codo, Ts);

    // // Reiniciar encoders
    _base->resetEncoder();
    _codo->resetEncoder();
    _hombro->resetEncoder();
}

String comando = "";

void loop()
{

    if (Serial.available())
    {
        String comando = Serial.readStringUntil('\n');
        comando.trim(); // Elimina espacios y \r

        if (comando.length() == 0)
            return;

        // Comandos de reseteo
        if (comando.equalsIgnoreCase("reset_base"))
        {
            Serial.println("Reseteando encoder de base...");
            _base->resetEncoder();
        }
        else if (comando.equalsIgnoreCase("reset_hombro"))
        {
            Serial.println("Reseteando encoder de hombro...");
            _hombro->resetEncoder();
        }
        else if (comando.equalsIgnoreCase("reset_codo"))
        {
            Serial.println("Reseteando encoder de codo...");
            _codo->resetEncoder();
        }

        // Comandos de movimiento tipo "B45.0", "H90", "C-30.5"
        else if (comando.length() > 1)
        {
            char motor = comando.charAt(0);
            float grados = comando.substring(1).toFloat();

            switch (motor)
            {
            case 'B':
            case 'b':
                Serial.printf("Moviendo base a %.2f grados\n", grados);
                _base->ControlPID_Motor(grados, Kp_base, Ki_base, Kd_base, PWM_MANT_BASE);
                break;

            case 'H':
            case 'h':
                Serial.printf("Moviendo hombro a %.2f grados\n", grados);
                // Evitar que se produzca el kick inicial cuando se va de una posición alta a una baja, tal que -20 -> 20
                if(grados < (_hombro->leerGrados()))
                {_hombro->kick_inicial_mejorado(grados, PWM_MANT_HOMBRO);
                _hombro->ControlPID_Motor(grados, Kp_hombro_subir, Ki_hombro_subir, Kd_hombro_subir, PWM_MANT_HOMBRO);
                }
                else
                {
                    _hombro->ControlPID_Motor(grados, Kp_hombro_bajar, Ki_hombro_bajar, Kd_hombro_bajar, PWM_MANT_HOMBRO);
                }
                
                break;

            case 'C':
            case 'c':
                Serial.printf("Moviendo codo a %.2f grados\n", grados);
                if(grados > (_codo->leerGrados()))
                {_codo->kick_inicial_mejorado(grados, PWM_MANT_CODO); // Revisar kick inicial del codo, no funciona bien
                _codo->ControlPID_Motor(grados, Kp_codo_subir, Ki_codo, Kd_codo_subir, PWM_MANT_CODO);
                }
                else
                {   
                    _codo->ControlPID_Motor(grados, Kp_codo_bajar, Ki_codo, Kd_codo_bajar, PWM_MANT_CODO);
                }
                break;

            default:
                printf("Comando no reconocido %s\n",comando.c_str());
                break;
            }
        }
        else
        {
            printf("Formato de comando no válido: %s\n",comando.c_str());
        }

    delay(3000);

    float deg_base = _base->leerGrados();
    float deg_hombro = _hombro->leerGrados();
    float deg_codo = _codo->leerGrados();

    Serial.print(">Base:");
    Serial.println(deg_base);

    Serial.print(">Hombro:");
    Serial.println(deg_hombro);

    Serial.print(">Codo:");
    Serial.println(deg_codo);
    }
}

/*cinem inversa*/
// array<double,4> ang={0,0,0,0}; // Array para almacenar los ángulos calculados
// Configuración de xyz para llegar al centro del tablero
// array<double, 3> xyz_centro = {0, 220, 0}; // ÁNGULOS NECESARIOS:  -0.00, 5.39, 51.32, 63.29
// // Configuración de xyz para llegar al extremo más alejado del tablero
// array<double, 3> xyz_extremo = {0, 300, 5}; // ÁNGULOS NECESARIOS: -0.00, 22.45, 29.17, 68.38 
// // Configuración de xyz para llegar al extremo más cercano del tablero
// array<double, 3> xyz_cercano = {0, 135, 5}; // ÁNGULOS NECESARIOS: -0.00, -15.10, 69.64, 65.46
// void loop(){
//     ang=cinematicaInversa(xyz_extremo, L_eslab);
//     Serial.print("Angulos: ");
//     for (int i = 0; i < 4; i++) {
//         Serial.print(ang[i]);
//         if (i < 3) Serial.print(", ");
//     }
//     Serial.println(" ");
//     delay(5000);

//     _base->ControlPID_Motor(ang[0], Kp_base, Ki_base, Kd_base, PWM_MANT_BASE);
    // if (abs(ang[1]) > abs(_hombro->leerGrados()))
    // {
    //     _hombro->kick_inicial_mejorado(ang[1], PWM_MANT_HOMBRO);
    //     _hombro->ControlPID_Motor(ang[1], Kp_hombro_subir, Ki_hombro, Kd_hombro, PWM_MANT_HOMBRO);
    // }
    // else
    // {
    //     _hombro->ControlPID_Motor(ang[1], Kp_hombro_bajar, Ki_hombro, Kd_hombro, PWM_MANT_HOMBRO);
    // }
//     if (ang[2] > (_codo->leerGrados()))
//     {
//         _codo->kick_inicial_mejorado(ang[2], PWM_MANT_CODO); // Revisar kick inicial del codo, no funciona bien
//         _codo->ControlPID_Motor(ang[2], Kp_codo, Ki_codo, Kd_codo, PWM_MANT_CODO);
//     }
//     else
//     {
//         _codo->ControlPID_Motor(ang[2], Kp_codo_bajar, Ki_codo, Kd_codo, PWM_MANT_CODO);
//     }
// }

/* EVALUAR ENCODERS */
// void loop()
// {
//     float deg_base = _base->leerGrados();
//     float deg_hombro = _hombro->leerGrados();
//     float deg_codo = _codo->leerGrados();

//     Serial.print(">Base:");
//     Serial.println(deg_base);

//     Serial.print(">Hombro:");
//     Serial.println(deg_hombro);

//     Serial.print(">Codo:");
//     Serial.println(deg_codo);

//   // _hombro->moverPWM(80);

//     delay(2000);
// }
