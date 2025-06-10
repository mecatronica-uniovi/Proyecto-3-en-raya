#ifndef MOTORES_H
#define MOTORES_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <SPI.h>
#include "reg.h"

class Motores {
public:
    Motores();
    ~Motores();

    // Configuración básica
    void configurarPines(int in1, int in2, int en);
    void asignarFDC(int fdc1, int fdc2);  // Nombre en español solo para FDC
    void asignarEncoder(int cs_encoder);
    
    // Movimiento básico
    void mover();
    void moverPWM(float PWM);
    void detener();
    void moverAGrados(double gradosRelativos);
    
    // Lectura de posición
    float leerEncoder();
    
    // Control de pinza
    void configurarPinza(int pin_servo, int angulo_abierto, int angulo_cerrado);
    void moverPinza();
    
    // Regulador PID
    void setRegulador(RegPID* reg);
    
    // Variables públicas
    double _pos;
    RegPID* regulador;

    // Interrupciones
    static volatile bool fdcInterruptTriggered;
    static Motores* activeInstance;

private:
    // Pines
    int _in1 = -1, _in2 = -1, _en = -1;
    int _fdc1 = -1, _fdc2 = -1;
    int _cs_encoder = -1;
    
    // Servo pinza
    Servo _servoPinza;
    int _angulo_abierto = 0;
    int _angulo_cerrado = 0;
    bool _pinza_abierta = true;
    
    // Comunicación SPI
    uint8_t SPI_T(uint8_t msg, int cs);
    bool _spi_initialized = false;
    
    // Manejo de interrupciones
    void handleFDCInterrupt(int fdcPin);
    static void ISRWrapper();
};

#endif