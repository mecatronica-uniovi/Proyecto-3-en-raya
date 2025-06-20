#include "Motor.h"
#include "funciones_cinem.h"
#include "configuracion.h"
#include "MyServer.h"

// Posición de los pines de los encoders
#define CS_BASE 10
#define CS_HOMBRO 41
#define CS_CODO 38

#define I_ANG_BASE 0
#define I_ANG_HOMBRO 1
#define I_ANG_CODO 2

Motor *_base, *_hombro, *_codo;

EstadoRobot estado_actual;

void IRAM_ATTR onFallingFC()
{
    // Verificar si los finales de carrera están activados
    if (digitalRead(fc1) == LOW || digitalRead(fc2) == LOW || digitalRead(fc3) == LOW || digitalRead(fc4) == LOW)
    {
        // Manejo de interrupciones de finales de carrera
        _base->setFCTriggered(true);
        _hombro->setFCTriggered(true);

        float impulso = 0;

        if (digitalRead(fc1) == LOW || digitalRead(fc3) == LOW)
        {
            impulso = 5;
        }
        if (digitalRead(fc2) == LOW || digitalRead(fc4) == LOW)
        {
            impulso = -5;
        }
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
    _hombro = new Motor(IN1_M2, IN2_M2, EN_M2, CS_HOMBRO);
    _codo = new Motor(IN1_M3, IN2_M3, EN_M3, CS_CODO);
    // Reiniciar encoders
    _base->resetEncoder();
    _codo->resetEncoder();
    _hombro->resetEncoder();

    while (Serial.available() == 0)
    {
        // Esperar hasta que el usuario escriba algo
        delay(100);
    }

    String entrada = Serial.readStringUntil('\n');
    entrada.trim();

    if (entrada == "0")
    {
        Serial.println("Modo seleccionado: INICIALIZANDO");
        estado_actual = EstadoRobot::INICIALIZANDO;
    }
    else if (entrada == "1")
    {
        Serial.println("Modo seleccionado: TEST");
        estado_actual = EstadoRobot::TEST;
    }
    else
    {
        Serial.println("Entrada inválida. Por defecto: INICIALIZANDO");
        estado_actual = EstadoRobot::INICIALIZANDO;
    }
}

String comando = "";
bool inicializado = false;
array<double, 4> ang_robot = {0, 0, 0, 0};
array<double, 4> ang_encoder = {0, 0, 0, 0};
array<double, 3> posicion_objetivo = {0, 220, 105};
// array<double, 4> ang_calib = {0, 11.07, 57.92, 0};
array<double, 4> ang_calib = {0, 11.07, 0, 0};
float deg_base = 0;
float deg_hombro = 0;
float deg_codo = 0;
int32_t _lastTime = 0;
int32_t _samplingPeriod_ms = 10000;

void loop()
{
    switch (estado_actual)
    {
    case EstadoRobot::INICIALIZANDO:
        printf("Inicializando robot\n");
        if (!inicializado)
        {
            ang_robot = cinematicaInversa(posicion_objetivo, L_eslab);
            ang_encoder = GradosRobot_a_Encoder(ang_robot, ang_calib);

            _base->ControlPID_Motor(ang_encoder[0], Kp_base, Ki_base, Kd_base, PWM_MANT_BASE);

            if (ang_encoder[1] < _hombro->leerGrados())
            {
                _hombro->kick_inicial_mejorado(ang_encoder[1], PWM_MANT_HOMBRO);
                _hombro->ControlPID_Motor(ang_encoder[1], Kp_hombro_subir, Ki_hombro_subir, Kd_hombro_subir, PWM_MANT_HOMBRO);
            }
            else
            {
                _hombro->ControlPID_Motor(ang_encoder[1], Kp_hombro_bajar, Ki_hombro_bajar, Kd_hombro_bajar, PWM_MANT_HOMBRO);
            }

            if (ang_encoder[2] > _codo->leerGrados())
            {
                _codo->kick_inicial_mejorado(ang_encoder[2], PWM_MANT_CODO);
                _codo->ControlPID_Motor(ang_encoder[2], Kp_codo_subir, Ki_codo, Kd_codo_subir, PWM_MANT_CODO);
            }
            else
            {
                _codo->ControlPID_Motor(ang_encoder[2], Kp_codo_bajar, Ki_codo, Kd_codo_bajar, PWM_MANT_CODO);
            }

            inicializado = true;
            estado_actual = EstadoRobot::ESPERANDO;
        }
        break;

    case EstadoRobot::ESPERANDO:
        printf("Esperando comando\n");
        _hombro->moverPWM(100);
        _codo->moverPWM(-50);
        if (Serial.available())
        {
            String input = Serial.readStringUntil('\n');
            input.trim();
            int count = sscanf(input.c_str(), "%lf,%lf,%lf",
                               &posicion_objetivo[0], &posicion_objetivo[1], &posicion_objetivo[2]);

            if (count == 3)
            {
                estado_actual = EstadoRobot::MOVIENDO;
            }
            else
            {
                estado_actual = EstadoRobot::ERROR;
            }
        }
        break;

    case EstadoRobot::MOVIENDO:
        printf("Moviendo robot\n");
        ang_robot = cinematicaInversa(posicion_objetivo, L_eslab);
        ang_encoder = GradosRobot_a_Encoder(ang_robot, ang_calib);

        _base->ControlPID_Motor(ang_encoder[0], Kp_base, Ki_base, Kd_base, PWM_MANT_BASE);

        if (ang_encoder[1] < _hombro->leerGrados())
        {
            _hombro->kick_inicial_mejorado(ang_encoder[1], PWM_MANT_HOMBRO);
            _hombro->ControlPID_Motor(ang_encoder[1], Kp_hombro_subir, Ki_hombro_subir, Kd_hombro_subir, PWM_MANT_HOMBRO);
        }
        else
        {
            _hombro->ControlPID_Motor(ang_encoder[1], Kp_hombro_bajar, Ki_hombro_bajar, Kd_hombro_bajar, PWM_MANT_HOMBRO);
        }

        if (ang_encoder[2] > _codo->leerGrados())
        {
            _codo->kick_inicial_mejorado(ang_encoder[2], PWM_MANT_CODO);
            _codo->ControlPID_Motor(ang_encoder[2], Kp_codo_subir, Ki_codo, Kd_codo_subir, PWM_MANT_CODO);
        }
        else
        {
            _codo->ControlPID_Motor(ang_encoder[2], Kp_codo_bajar, Ki_codo, Kd_codo_bajar, PWM_MANT_CODO);
        }

        deg_base = _base->leerGrados();
        deg_hombro = _hombro->leerGrados();
        deg_codo = _codo->leerGrados();

        Serial.print(">Base:");
        Serial.println(deg_base);

        Serial.print(">Hombro:");
        Serial.println(deg_hombro);

        Serial.print(">Codo:");
        Serial.println(deg_codo);

        cinematicaDirecta(ang_robot, L_eslab);
        
        // Después de moverse, volver a esperar
        estado_actual = EstadoRobot::ESPERANDO;
        break;

    case EstadoRobot::DETENIDO:
        printf("Robot detenido\n");
        _base->detener();
        _hombro->detener();
        _codo->detener();
        break;

    case EstadoRobot::ERROR:
        printf("Fallo de robot\n");
        _base->detener();
        _hombro->detener();
        _codo->detener();
        posicion_objetivo = {0, 220, 105};
        inicializado = false;
        estado_actual = EstadoRobot::INICIALIZANDO;
        break;

    case EstadoRobot::TEST:
        if (Serial.available())
        {
            String comando = Serial.readStringUntil('\n');
            comando.trim(); // Elimina espacios y \r

            if (comando.length() == 0)
                return;

            // Comandos de reseteo
            if (comando.equalsIgnoreCase("R"))
            {
                Serial.println("Reseteando todos los encoders...");
                _base->resetEncoder();
                _hombro->resetEncoder();
                _codo->resetEncoder();
            }
            else if (comando.startsWith("T="))
            {
                scanf(comando.c_str() + 2, "%d", &_samplingPeriod_ms);
            }
            else if (comando.startsWith("C"))
            { // Cn=valor , n =0, 1, 2
                int index = atoi(comando.c_str() + 1);
                if (index >= 0 && index <= 3 && comando[2] == '=')
                    ang_calib[index] = atof(comando.c_str() + 3);
            }
            else if (comando.startsWith("MOV"))
            {
                estado_actual = EstadoRobot::ESPERANDO;
            }
            else if (comando.startsWith("POS"))
            {
                cinematicaDirecta(ang_encoder,L_eslab);
            }
        }

        int32_t curTime = millis();
        if (curTime - _lastTime >= _samplingPeriod_ms)
        {
            _lastTime = curTime;
            ang_encoder = {_base->leerGrados(), _hombro->leerGrados(), 90 - _codo->leerGrados(), 90};
            printf("Angulos encoders MEDIDOS:\n"
                   "   Base: %.2lf , Hombro: %.2lf , Codo: %.2lf\n",
                   ang_encoder[I_ANG_BASE], ang_encoder[I_ANG_HOMBRO], ang_encoder[I_ANG_CODO]);

            ang_robot = GradosEncoder_a_Robot(ang_encoder, ang_calib);
            // Serial.print("Angulos robot CALCULADOS:\n");
            // Serial.print("Base: ");
            // Serial.println(ang_robot[0]);
            // Serial.print("Hombro: ");
            // Serial.println(ang_robot[1]);
            // Serial.print("Codo: ");
            // Serial.println(ang_robot[2]);

            Serial.print("\n");

            cinematicaDirecta(ang_robot, L_eslab);
        }
    }

    delay(100); // Pequeño delay para evitar sobrecarga
}
