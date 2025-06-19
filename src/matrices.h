#ifndef MATRICES_H
#define MATRICES_H

#include <cmath>

// Clase para manejo de matrices 4x4
class Matriz {
public:
    // Estructura de una matriz de 4x4 inicializada a 0
    struct M4x4{
        double m[4][4]={};
    };

    static M4x4 identidad();
    static M4x4 multiplicar(const M4x4 &A, const M4x4 &B);
    static M4x4 dh(double theta, double a, double alpha, double d);
};

#endif // MATRICES_H