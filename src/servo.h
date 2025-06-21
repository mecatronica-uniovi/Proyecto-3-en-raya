#ifndef servo_h
#define servo_h

#include <ESP32Servo.h>
#include "configuracion.h"

extern Servo ServoPinza;

void muevePinza(bool abierta);

#endif