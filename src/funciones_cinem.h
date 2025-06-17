#ifndef FUNCIONES_CINEM_H
#define FUNCIONES_CINEM_H

#include<array>
#include<cmath>
#include <iostream>
#include "Arduino.h"


using std::array;

array<double, 4> cinematicaInversa(array<double, 3> xyz, array<double, 4> L);
array<array<double,3>,2> origen_destino();

#endif