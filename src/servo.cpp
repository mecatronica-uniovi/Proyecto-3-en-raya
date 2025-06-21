#include "servo.h"
void muevePinza(bool abierta) {
    const int tiempo_movimiento_servo = 1000; // ms para movimiento
    
    if (abierta) {
        printf("Cerrando pinza\n");
        ServoPinza.write(pinza_cerrada);
    } else {
        printf("Abriendo pinza\n");
        ServoPinza.write(pinza_abierta);
    }
    
    delay(tiempo_movimiento_servo); // Espera a que el servo complete el movimiento
    abierta = !abierta;
}
