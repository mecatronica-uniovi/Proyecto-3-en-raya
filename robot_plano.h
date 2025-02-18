#include "matriz.h"
#include <cmath>

#ifndef ROBOT_PLANO
#define ROBOT_PLANO

class robot_plano{
    public:
        robot_plano();

        float L0, ang_0;
        float L01, ang_1;
        float L12, ang_2;
        float L23, ang_3;

        matriz<float> P0;
        matriz<float> P1;
        matriz<float> P2;
        matriz<float> P3;

        matriz<float> R0;
        matriz<float> R01;
        matriz<float> R12;
        matriz<float> R23;

        matriz<float> T0;
        matriz<float> T01;
        matriz<float> T12;
        matriz<float> T23;

        void determina_ang_eslabones(float ang_0, float ang_1, float ang_2, float ang_3);
        void determina_long_eslabones(float l0, float l01, float l12, float l23);
        void cinematica_directa();      
};

#endif

robot_plano::robot_plano(){
    L0=0;
    L01=0;
    L12=0;
    L23=0;

    ang_0=0;
    ang_1=0;
    ang_2=0;
    ang_3=0;

    P0=matriz<float>(2,1);
    P1=matriz<float>(2,1);
    P2=matriz<float>(2,1);
    P3=matriz<float>(2,1);

    R0=matriz<float>(2, 2);
    R01=matriz<float>(2, 2);  // Matriz 2x2
    R12=matriz<float>(2, 2);
    R23=matriz<float>(2, 2);

    T0=matriz<float>(2,1);
    T01=matriz<float>(2, 1);  // Matriz 2x1
    T12=matriz<float>(2, 1);
    T23=matriz<float>(2, 1);

    R0.angulo_rot_g=ang_0;
    R01.angulo_rot_g=ang_1;
    R12.angulo_rot_g=ang_2;
    R23.angulo_rot_g=ang_3;  
}

void robot_plano::determina_long_eslabones(float l0,float l01, float l12, float l23){
    L0=l0;
    L01=l01;
    L12=l12;
    L23=l23;

    T0.set_valor(ang_0,0,0);
    T0.set_valor(ang_0,1,0);

    T01.set_valor(L01*cos(ang_1),0,0);
    T01.set_valor(L01*sin(ang_1),1,0);

    T12.set_valor(L12*cos(ang_2),0,0);
    T12.set_valor(L12*sin(ang_2),1,0);

    T23.set_valor(L23*cos(ang_3),0,0);
    T23.set_valor(L23*sin(ang_3),1,0);

}

void robot_plano::determina_ang_eslabones(float ang0, float ang1, float ang2, float ang3){
    ang_0=ang0;
    ang_1=ang1;
    ang_2=ang2;
    ang_3=ang3;

    R0.angulo_rot_g=ang_0;
    R01.angulo_rot_g=ang_1;
    R12.angulo_rot_g=ang_2;
    R23.angulo_rot_g=ang_3;
}

void robot_plano::cinematica_directa(){
    R0.matriz_rotacion();
    R01.matriz_rotacion();
    R12.matriz_rotacion();
    R23.matriz_rotacion();

    P1.set_valor(R01.get_valor(0,0)*L01+P0.get_valor(0,0),0,0);
    P1.set_valor(R01.get_valor(1,0)*L01+P0.get_valor(1,0),1,0);

    P2.set_valor(R12.get_valor(0,0)*L12+P1.get_valor(0,0),0,0);
    P2.set_valor(R12.get_valor(1,0)*L12+P1.get_valor(1,0),1,0);

    P3.set_valor(R23.get_valor(0,0)*L23+P2.get_valor(0,0),0,0);
    P3.set_valor(R23.get_valor(1,0)*L23+P2.get_valor(1,0),1,0);
}