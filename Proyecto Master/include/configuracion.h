// configuracion.h
#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <iostream>
#include <array>

using namespace std;

// Angulos maximos pinza
const int pinza_abierta=90;
const int pinza_cerrada=120;

// Pines de la PCB para finales de carrera
const int fc1 = 9;
const int fc2 = 40;
const int fc3 = 21;
const int fc4 = 1;
const int fc5 = 2;
const int fc6=20;

// PWM motores
const int PWM_MAX=255;
const int PWM_MIN=0;
const int PWM_VELOCIDAD=100;

// Motor 1
/* Nuestros: 
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
*/


/* Mallada: */
#define IN1_M1 15
#define IN2_M1 16
#define EN_M1 7

#define IN1_M2 6
#define IN2_M2 14
#define EN_M2 5

#define IN1_M3 18
#define IN2_M3 8
#define EN_M3 17

// Pines encoder
#define CS1 10
#define CS2 40
#define CS3 41

// const array<double, 4> L_eslab={0,125,125,90};
const array<double, 4> L_eslab={200,125,125,90};

#endif