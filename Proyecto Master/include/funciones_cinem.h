#ifndef FUNCIONES_CINEM_H
#define FUNCIONES_CINEM_H

#include <array>
#include <iostream>
#include "matrices.h"

extern std::array<double, 4> ang_robot;
extern std::array<double, 4> long_eslabones;
extern float Ts;

double relacionLineal(double x, double x_max, double x_min, double y_max, double y_min);
void cinematicaDirecta(std::array<double, 4> theta, std::array<double, 4> L);
std::array<double, 4> cinematicaInversa(std::array<double, 3> xyz, std::array<double, 4> L);
void desplazamientoRobot(std::array<double, 4> ang);
void posicion_inicial();
void movimiento_segun_angulos(std::array<double, 4> theta, std::array<double, 4> L);
void movimiento_segun_coord(std::array<double, 3> xyz, std::array<double, 4> L);
void ejecutarSimulacion();

#endif // FUNCIONES_CINEM_H