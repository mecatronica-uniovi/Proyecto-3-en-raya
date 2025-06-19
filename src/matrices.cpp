#include "matrices.h"

    // Se crea una matriz identidad I de tipo 4x4 (solo 1s en la diagonal,
    // el resto 0), y se devuelve I
    Matriz::M4x4 Matriz::identidad(){
        M4x4 I;
        for (int i=0; i<4;i++){
            I.m[i][i]=1;
        }
        return I;
    }

    // Se inicializa una matriz de 4x4 llamada resultado y se multiplican
    // los elementos de dos matrices también de 4x4 pasadas como argumentos
    // de manera que C[i][j]= sum(A[i][k]*B[k][j]). Esta operación se realiza
    // con tres bucles anidados. Los parámetros A y B se pasan constantes para
    // evitar su modificación, y como referencia para evitar copias innecesarias.
    Matriz::M4x4 Matriz::multiplicar(const M4x4 &A, const M4x4 &B){
        M4x4 resultado;
        for (int i=0;i<4;i++){
            for (int j=0;j<4;j++){
                for (int k=0; k<4;k++){
                    resultado.m[i][j]+=A.m[i][k]*B.m[k][j];
                }
            }
        }
        return resultado;
    }

    // Creación de una matriz T con los parámetros de Denavit-Hartenberg
Matriz::M4x4 Matriz::dh(double theta, double a, double alpha, double d){
    M4x4 T;

    T.m[0][0] = cos(theta);
    T.m[0][1] = -sin(theta);
    T.m[0][2] = 0;
    T.m[0][3] = a;

    T.m[1][0] = sin(theta) * cos(alpha);
    T.m[1][1] = cos(theta) * cos(alpha);
    T.m[1][2] = -sin(alpha);
    T.m[1][3] = -sin(alpha) * d;

    T.m[2][0] = sin(theta) * sin(alpha);
    T.m[2][1] = cos(theta) * sin(alpha);
    T.m[2][2] = cos(alpha);
    T.m[2][3] = cos(alpha) * d;

    T.m[3][0] = 0;
    T.m[3][1] = 0;
    T.m[3][2] = 0;
    T.m[3][3] = 1;

    return T;
}
