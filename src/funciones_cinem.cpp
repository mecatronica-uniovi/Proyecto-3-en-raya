#include "funciones_cinem.h"

// void cinematicaDirecta(array<double, 4> theta, array<double, 4> L){
//     // Conversión de ángulos (º -> rad)
//     theta[0]=theta[0]*PI/180.0;
//     theta[1]=(theta[1]*PI/180.0)+PI/2;
//     theta[2]=theta[2]*PI/180.0;
//     theta[3]=theta[3]*PI/180.0;

//     // Parámetros de D-H
//     array<double, 5> a={0, L[0], L[1], L[2], L[3]}; // Longitudes de eslabones
//     array<double, 5> alpha={0, PI / 2, 0, 0, 0};    // Ángulos entre ejes Z
//     array<double, 5> d={0, 0, 0, 0, 0};             // Traslaciones en Z (mecanismo plano = 0)
 
//     // Inicialización de la matriz de transformación como matriz identidad
//     Matriz::M4x4 T=Matriz::identidad();

//     // Bucle de cálculo empleando la funcion dh de matrices.h
//     for (int i=0;i<4;i++){
//         Matriz::M4x4 Ti=Matriz::dh(-theta[i], a[i], alpha[i], d[i]);
//         T = Matriz::multiplicar(T, Ti);
//     }

//     // Añado el último eslabón a la matriz
//     Matriz::M4x4 T5 = Matriz::dh(0, a[4], 0, 0);
//     T=Matriz::multiplicar(T, T5);

//     // Redondeos a 0 de la posición
//     double X=T.m[1][3];
//     if(abs(X)<1e-3){
//         X=0;
//     }
//     double Y=-T.m[0][3];
//     if(abs(Y)<1e-3){
//         Y=0;
//     }
//     double Z=-T.m[2][3];
//     if(abs(Z)<1e-3){
//         Z=0;
//     }
    
//     cout<<"\nPluma en coordenadas: [" << X << ", " << Y << ", " << Z << "]\n";
// }

/*Cálculo de los ángulos necesarios en las articulaciones para alcanzar la
posición X,Y,Z solicitada. Dado que se trata de un mecanismo plano, primero
se trabaja en coordenadas W,Z y luego W se desdobla en X,Y según el ángulo de
la base del robot. */
array<double, 4> cinematicaInversa(array<double, 3> xyz, array<double, 4> L){
    array<double, 4> angulos;

    // Posiciones de la muñeca del mecanismo plano W2,Z2. Theta2 con teorema del coseno
    float z2=xyz[2]+L[3];
    float w2=sqrt(xyz[0]*xyz[0]+xyz[1]*xyz[1]);
    float L_punta=sqrt(w2*w2+z2*z2);
    
    float alfa_1=atan2(z2,w2);
    float alfa_2=acos((L[1]*L[1]+L_punta*L_punta-L[2]*L[2])/(2*L[1]*L_punta));
    float theta_1=PI/2-(alfa_1+alfa_2);
    
    // Posiciones del codo del mecanismo plano W1,Z1. Theta1 con trigonometría
    float alfa_3=acos((L[2]*L[2]-L_punta*L_punta+L[1]*L[1])/(2*L[2]*L[1]));
    float theta_2=PI-alfa_3;
    
    // Posiciones del hombro del mecanismo plano W0,Z0. Theta0 con tangente
    float theta_3=PI-(theta_1+theta_2);

    // Posiciones de la pluma del mecanismo plano W3,Z3. Theta3 según los demás angulos del mecanismo
    float alfa_4=atan2(xyz[1],xyz[0]);
    float theta_0=PI/2-alfa_4;

    // Conversión a grados
    angulos[0]=theta_0*180/PI;
    angulos[1]=theta_1*180/PI-(0.57+5.39);
    angulos[2]=theta_2*180/PI-(60+51.32+0.83);
    angulos[3]=theta_3*180/PI-(63.29-1.40);

    return angulos;
}

array<array<double,3>,2> origen_destino(){
    
}