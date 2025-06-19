#ifndef FUNCIONES_CINEM_H
#define FUNCIONES_CINEM_H

#include<array>
#include<cmath>
#include <iostream>
#include "Arduino.h"
#include "matrices.h"


using std::array;

array<double, 4> cinematicaInversa(array<double, 3> xyz, array<double, 4> L);
void cinematicaDirecta(array<double, 4> theta, array<double, 4> L);
array<double, 4> GradosRobot_a_Encoder(array<double, 4> ang_robot);
array<double, 4> GradosEncoder_a_Robot(const array<double, 4>& ang_enc,const array<double, 4>& ang_calib);

#endif