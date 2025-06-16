#include "Motor.h"

#include "configuracion.h"

// Posición de los pines de los encoders
#define CS_BASE 10
#define CS_HOMBRO 41
#define CS_CODO 38

Motor *_base, *_hombro, *_codo;

void IRAM_ATTR onFallingFC()
{
    delayMicroseconds(500);
    // Verificar si los finales de carrera están activados
    if (digitalRead(fc1) == LOW || digitalRead(fc2) == LOW || digitalRead(fc3) == LOW || digitalRead(fc4) == LOW)
    {
        // Manejo de interrupciones de finales de carrera
        _base->setFCTriggered(true);
        _hombro->setFCTriggered(true);
        _codo->setFCTriggered(true);

        Serial.println("Final de carrera activado");

        // if(digitalRead(fc1) == LOW || digitalRead(fc3) == LOW ){
        //     // Si se activa el final de carrera 1 o 3, mover motores a la posición inicial
        //     _base->ControlPID_Motor(_base->leerGrados()-5);
        //     //  _hombro->ControlPID_Motor(_base->leerGrados()-5);
        //     //  _codo->ControlPID_Motor(_codo->leerGrados()-5);
        // }
        // else if(digitalRead(fc2) == LOW || digitalRead(fc4) == LOW){
        //     // Si se activa el final de carrera 2 o 4, mover motores a la posición inicial
        //     _base->ControlPID_Motor(_base->leerGrados()+5);
        //     // _hombro->ControlPID_Motor(_hombro->leerGrados()+5);
        //     // _codo->ControlPID_Motor(_codo->leerGrados()+5);
        // }

        _base->detener();
        //_base->kick_inicial_mejorado();
        _hombro->detener();
        _codo->detener();

        Serial.println("Motores alejados y parados");
    }
    else
    {
        return; // No hacer nada si no se ha activado ningún final de carrera
    }
}

void setup()
{
    Serial.begin(115200);

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
    _base->setRegulador(Kp_base, Ki_base, Kd_base, Ts);

    _hombro = new Motor(IN1_M2, IN2_M2, EN_M2, CS_HOMBRO);
    _hombro->setRegulador(Kp_hombro, Ki_hombro,Kd_hombro, Ts);

    _codo = new Motor(IN1_M3, IN2_M3, EN_M3, CS_CODO);
    _codo->setRegulador(Kp_codo, Ki_codo, Kd_codo, Ts);
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
                if(abs(grados) > abs(_hombro->leerGrados()))
                {_hombro->kick_inicial_mejorado(grados, PWM_MANT_HOMBRO);
                _hombro->ControlPID_Motor(grados, Kp_hombro, Ki_hombro, Kd_hombro, PWM_MANT_HOMBRO);
                }
                else
                {
                    _hombro->ControlPID_Motor(grados, Kp_hombro, Ki_hombro, Kd_hombro, PWM_MANT_HOMBRO);
                }
                
                break;

            case 'C':
            case 'c':
                Serial.printf("Moviendo codo a %.2f grados\n", grados);
                if(abs(grados) > abs(_codo->leerGrados()))
                {_codo->kick_inicial_mejorado(grados, PWM_MANT_CODO); // Revisar kick inicial del codo, no funciona bien
                _codo->ControlPID_Motor(grados, Kp_codo, Ki_codo, Kd_codo, PWM_MANT_CODO);
                }
                else
                {
                    _codo->ControlPID_Motor(grados, Kp_codo, Ki_codo, Kd_codo, PWM_MANT_CODO);
                }
                break;

            default:
                Serial.println("Comando no reconocido.");
                break;
            }
        }
        else
        {
            Serial.println("Formato de comando no válido.");
        }
    }
}

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

//    _hombro->moverPWM(80);

//     delay(100);
// }

// void loop() {
//     // Aquí puedes implementar la lógica de tu aplicación
//     Serial.println("Hola, mundo!");
//     delay(1000); // Espera 1 segundo antes de repetir
// }