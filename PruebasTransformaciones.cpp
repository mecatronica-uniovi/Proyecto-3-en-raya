#include "robot_plano.h"
#include <iostream>

using namespace std;

int main(){
    matriz<float> M0(2,2), M1(2,2), M2(2,2), M3(2,2);
    robot_plano robot;

    cout<<"M0:"<<endl;
    M0.print_matriz();

    cout<<"M1:"<<endl;
    M1.print_matriz();

    cout<<"M2:"<<endl;
    M2.print_matriz();

    cout<<"M3:"<<endl;
    M3.print_matriz();

    M0.angulo_rot_g=60;
    M1.angulo_rot_g=0;
    M2.angulo_rot_g=90;
    M3.angulo_rot_g=90;

    robot.determina_ang_eslabones(60,0,90,90);

    robot.R0=M0.matriz_rotacion();
    robot.R01=M1.matriz_rotacion();
    robot.R12=M2.matriz_rotacion();
    robot.R23=M3.matriz_rotacion();

    cout<<"R0:"<<endl;
    robot.R0.print_matriz();

    cout<<"R01:"<<endl;
    robot.R01.print_matriz();

    cout<<"R12:"<<endl;
    robot.R12.print_matriz();

    cout<<"R23:"<<endl;
    robot.R23.print_matriz();

    robot.determina_long_eslabones(0,125,125,90);

    cout<<"Traslacion de S0:"<<endl;
    robot.T0.print_matriz();

    cout<<"Traslacion de S1:"<<endl;
    robot.T01.print_matriz();

    cout<<"Traslacion de S2:"<<endl;
    robot.T12.print_matriz();

    cout<<"Traslacion de S3:"<<endl;
    robot.T23.print_matriz();

    robot.cinematica_directa();

    cout<<"Localización final de S1:"<<endl;
    robot.P1.print_matriz();

    cout<<"Localización final de S2:"<<endl;
    robot.P2.print_matriz();

    cout<<"Localización final de S3:"<<endl;
    robot.P3.print_matriz();

    cout<<"CINEMATICA DIRECTA  XYZ"<<endl;
    robot.cinematica_directa_xyz();
    robot.xyz.print_matriz();

    matriz<float> pos_act(3,1);
    pos_act.set_valor(-125,0,0);
    pos_act.set_valor(0,1,0);
    pos_act.set_valor(35,2,0);

    cout<<"CINEMATICA INVERSA: "<<endl;
    cout<<"X_des=-125, Y_des=0, Z_des=35"<<endl;
    robot.cinematica_inversa(pos_act);
    

    return 0;
}