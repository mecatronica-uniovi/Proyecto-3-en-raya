#ifndef FUNCIONES_MOTORES_H
#define FUNCIONES_MOTORES_H

#include <array>
#include <iostream>
#include "motores.h"
#include "reg.h"

using namespace std;

// void muevePinza();
// array<bool,3> Final_Carrera();
// void configurarMotores();
// void configurarFDC();
// void moverMotor(int en, int inA, int inB);
// void detenerMotores();
// void moverMotorAdelante();
// void moverMotorAtras();
// inline void moverMotorAGrados(float gradosRelativos);
// inline float leerGrados();
// inline void setupEncoder();
// void SetPWM(int motor, array<double, 4> PWM);

void LecturaEncoders(Motores& motor1, Motores& motor2, Motores& motor3);
void AsociaRegMotor(array<RegPID*, 3>& reguladores, array<Motores*, 3>& motores);
void MoverMotores(Motores& motor1, float PWM1, Motores& motor2, float PWM2, Motores& motor3, float PWM3);
void ControlMotores(Motores& motor1, Motores& motor2, Motores& motor3, array<double, 3>& xyz, const array<double, 4>& L);


#endif // FUNCIONES_MOTORES_H