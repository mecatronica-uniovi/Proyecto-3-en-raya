#include "Fdt.h"

/* Constructor de la clase Fdt, recibe el parámetro de Grado, el puntero al numerador y
el puntero al denominador de la función */
Fdt:: Fdt(int Grado, double *ptrNum, double *ptrDen){
    Grado_=Grado;			// Copia del valor del Grado
    ptrNum_=ptrNum;			// Copia del puntero al Numerador
    ptrDen_=ptrDen;			// Copia del puntero al Denominador
    ptrVectorX_=new double[Grado_];	// Crea un vector X del tamaño del grado
    ptrVectorY_=new double[Grado_];	// Crea un vector de Y del tamaño del grado
    for (int i=0; i< Grado ; i++){
        ptrVectorX_[i]=0;		// Inicializa los vectores de X e Y a cero
        ptrVectorX_[i]=0;
   }
}

/* Método del productor escalar, recibe dos punteros de la misma longitud que el grado
y realiza su producto escalar */
double Fdt::ProductoEscalar(double *ptrA, double *ptrB){
    double resultado = 0;
    for (int i = 0; i < Grado_; i++){
        resultado += ptrA[i] * ptrB[i];
    }
    return resultado;
}

/* Método de desplazamiento, los elementos del puntero se actualizan con el valor anterior.
Por ejemplo si mi grado es 3, entro en el for con i=2 y en la posición 2 del puntero, almaceno
el valor de la posición 1. */
void Fdt::Desplazamiento(double *ptrC){
    for (int i = Grado_ - 1; i >= 1; i--){
        ptrC[i] = ptrC[i - 1];
    }
}

/* Método de cálculo de la salida, calulará el valor de Yk en función de la entrada Xk.
Primero desplazará los vectores de valores de X e Y, actualizará el valor del vector de
valores de X con el Xk y por último, mediante los métodos de producto escalar, calculará
el valor de la correspondiente Yk actualizando también el vector de valores de Y.*/
double Fdt::CalcularSalida(double Xk){
    double Yk=0;
    Desplazamiento(ptrVectorY_);
    ptrVectorY_[0]=0;
    Desplazamiento(ptrVectorX_);
    ptrVectorX_[0]=Xk;
    Yk= (ProductoEscalar(ptrVectorX_,ptrNum_)-ProductoEscalar(ptrVectorY_,ptrDen_))/ptrDen_[0];
    ptrVectorY_[0]=Yk;

    return Yk;
}
