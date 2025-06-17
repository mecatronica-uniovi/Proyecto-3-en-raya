#ifndef MOTORES_H
#define MOTORES_H

#include <Arduino.h>

#include "Encoder.h"
#include "reg.h"

class Motor {
public:
    Motor(int in1, int in2, int enable, int cs_encoder);
    
    // Leer posición del encoder
    float leerGrados();

    // Movimiento básico
    void moverAGrados(double gradosRelativos);
    void moverPWM(float PWM);
    void detener();
    void ControlPID_Motor(float pos_des, float Kp, float Ki, float Kd, int pwm_mant);

    // Regulador PID
    void setRegulador(float Kp, float Ki, float Kd, float Tm);

    // Interrupción de finales de carrera
    void setFCTriggered(bool triggered);

    // Resetear los valores del encoder a cero
    bool resetEncoder();


    bool kick_inicial_mejorado(float objetivo, int pwm_mant);

    // Encoder
    Encoder *_encoder = nullptr;

private:
    // Pines
    int _in1 = -1, _in2 = -1, _en = -1;

    // Regulador PID
    RegPID *_regulador;

    // Velocida predefinida
    const int PWM_VELOCIDAD=120;
    const int PWM_MAX=255;
    const int PWM_MIN=0;

    // Interrupción de finales de carrera
    bool _fcTriggered = false;

};

#endif