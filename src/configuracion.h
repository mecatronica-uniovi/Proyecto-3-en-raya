// configuracion.h
#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <iostream>
#include <array>

using namespace std;

enum class EstadoRobot
{
    INICIALIZANDO,
    ESPERANDO,
    MOVIENDO,
    DETENIDO,
    ERROR,
    TEST,
    AJUSTE
};

// Angulos maximos pinza
const int pinza_abierta=120;
const int pinza_cerrada=90;
#define _PIN_SERVO 4

// Periodo de muestreo
const float Ts = 0.01;

// Ganancias PID base
extern float Kp_base; //Ganancias de la base
extern float Ki_base;
extern float Kd_base;

// Ganancias PID hombro
extern float Kp_hombro_subir; // Ganancias del hombro subir
extern float Ki_hombro_subir;
extern float Kd_hombro_subir;

extern float Kp_hombro_bajar; // Ganancias del hombro revisar
extern float Ki_hombro_bajar;
extern float Kd_hombro_bajar;

// Ganancias PID codo
extern float Kp_codo_subir;
extern float Kp_codo_bajar;
extern float Ki_codo;
extern float Kd_codo_subir;
extern float Kd_codo_bajar;

// Pines de la PCB para finales de carrera
const int fc1 = 9;
const int fc2 = 1;
const int fc3 = 2;
const int fc4 = 40;
const int fc5 = 20;
const int fc6 = 21;

// PWM motores
const int PWM_MANT_BASE=0;
const int PWM_MANT_HOMBRO=120;
const int PWM_MANT_CODO=90;

// Motor 1
#define IN1_M1 16
#define IN2_M1 15
#define EN_M1 17

// Motor 2
#define IN1_M2 7
#define IN2_M2 6
#define EN_M2 5

// Motor 3
#define IN1_M3 8
#define IN2_M3 18
#define EN_M3 14

// Pines encoder
#define CS1 10
#define CS2 41
#define CS3 38

const array<double, 4> L_eslab={56.75,250,220,142}; // Longitudes de los eslabones del robot
const double L_tablero=164.25;

#endif