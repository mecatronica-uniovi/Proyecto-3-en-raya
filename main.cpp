// Cabeceras necesarias:
#include <iostream>
#include <math.h>
#include "Fdt.h"
#include "reg.h"

using namespace std;

int main()
{
    // Valores del numerador y del denominador
    double num[]={0.000259,0.001686,0};
    double den[]={1,-1.511,0.5488};

    int n=100;                      // Numero de iteraciones
    Fdt FuncionTrans(3,num,den);	// Objeto de Fdt

    // Valores de los reguladores
    float Kp=100;
    float Ki=200;
    float Kd=10;

    // Tiempo de muestreo (para la integral)
    float Ts=0.05;

    // Inicialización de parámetros
    float Ref=1;           // Referencia = Escalón unitario
    float Uk=0;            // Acción de control
    float error=0;         // Error (error=referencia-salida)
    float Yk=0;            // Salida

    RegPID Regulador(Kp, Ki, Kd, Ts); // Objeto Regulador de tipo PID

    /* Teniendo en cuenta que mi referencia es un escalón unitario, calculo
       la acción de control junto con la salida y las muestro por pantalla*/
    for (int i=0;i<n;i++){
        error=Ref-Yk;                           // Calculo el error
        Uk=Regulador.calcularUk(error);         // Calculo la acción de control necesaria
        cout << "U["<<i<<"]= "<< Uk << endl;    // Muestro la acción de control por pantalla
        Yk=FuncionTrans.CalcularSalida(Uk);     // Calculo la salida teniendo en cuenta la acción de control
                                                // con la FdT definida po rel numerador y el denominador
        cout << "Y["<<i<<"]= "<< Yk << endl;    // Muestro la salida por pantalla
    }

    return 0;
}
