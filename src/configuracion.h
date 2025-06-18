// configuracion.h
#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <iostream>
#include <array>

using namespace std;

// Angulos maximos pinza
const int pinza_abierta=90;
const int pinza_cerrada=120;

// Periodo de muestreo
const float Ts = 0.01;

// Ganancias PID base
#define Kp_base 15 //Ganancias de la base no hace falta tocarlas
#define Ki_base 2
#define Kd_base 1

// Ganancias PID hombro
#define Kp_hombro_subir 12 // Ganancias del hombro subir
#define Ki_hombro_subir 0
#define Kd_hombro_subir 1.2

#define Kp_hombro_bajar 0.5 // Ganancias del hombro revisar
#define Ki_hombro_bajar 0
#define Kd_hombro_bajar 0

// Ganancias PID codo
#define Kp_codo_subir 50
#define Kp_codo_bajar 35
#define Ki_codo 0
#define Kd_codo_subir 0.1
#define Kd_codo_bajar 0

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

// const array<double, 4> L_eslab={0,125,125,90};
// const array<double, 4> L_eslab={200,210,300,50}; 
const array<double, 4> L_eslab={55,250,220,145}; // Longitudes de los eslabones del robot

#endif