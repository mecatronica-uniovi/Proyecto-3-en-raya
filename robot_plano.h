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

        matriz<float> xyz;

        void determina_ang_eslabones(float ang_0, float ang_1, float ang_2, float ang_3);
        void determina_long_eslabones(float l0, float l01, float l12, float l23);
        void cinematica_directa();    
        void cinematica_directa_xyz();
        void cinematica_inversa(matriz<float> XYZ);   
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

    xyz=matriz<float>(3,1);

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

    T0.set_valor(cos(ang_0),0,0);
    T0.set_valor(sin(ang_0),1,0);

    ang_1=M_PI*0.5-ang_1;

    T01.set_valor(L01*cos(ang_1),0,0);
    T01.set_valor(L01*sin(ang_1),1,0);

    ang_2=ang_1-M_PI*0.5;

    T12.set_valor(L12*cos(ang_2),0,0);
    T12.set_valor(L12*sin(ang_2),1,0);

    ang_3=-M_PI*0.5;

    T23.set_valor(L23*cos(ang_3),0,0);
    T23.set_valor(L23*sin(ang_3),1,0);

}

void robot_plano::determina_ang_eslabones(float ang0, float ang1, float ang2, float ang3){
    ang_0=ang0*M_PI/180;
    ang_1=ang1*M_PI/180;
    ang_2=ang2*M_PI/180;
    ang_3=ang3*M_PI/180;

    R0.angulo_rot_g=ang_0;
    R01.angulo_rot_g=ang_1;
    R12.angulo_rot_g=ang_2;
    R23.angulo_rot_g=ang_3;

    R0.matriz_rotacion();
    R0.multiplicar_escalar(L0);

    R01.matriz_rotacion();
    R01.multiplicar_escalar(L01);

    R12.matriz_rotacion();
    R12.multiplicar_escalar(L12);

    R23.matriz_rotacion();
    R23.multiplicar_escalar(L23);

}

void robot_plano::cinematica_directa(){
    R0.matriz_rotacion();
    R0.multiplicar_escalar(L0);

    R01.matriz_rotacion();
    R01.multiplicar_escalar(L01);

    R12.matriz_rotacion();
    R12.multiplicar_escalar(L12);

    R23.matriz_rotacion();
    R23.multiplicar_escalar(L23);
    
    P1=R01.get_columna(1)+P0;
    P2=R12.get_columna(1)+P1;
    P3=R23.get_columna(0)+P2;

}

void robot_plano::cinematica_directa_xyz(){
    xyz.set_valor(P3.get_valor(1,0),2,0);
    float ang=R0.angulo_rot_g*M_PI/180;
    
    float x=P3.get_valor(0,0)*sin(ang_0);
    xyz.set_valor(x,0,0);

    float y=P3.get_valor(0,0)*cos(ang_0);
    xyz.set_valor(y,1,0);   
}

void robot_plano::cinematica_inversa(matriz<float> XYZ){
    
    ang_3=90;
    
    float Z_art2=L23+XYZ.get_valor(2,0);
    XYZ.set_valor(Z_art2,2,0);
    xyz=XYZ;

    float x2=xyz.get_valor(0,0);
    float y2=xyz.get_valor(1,0);
    float w2=sqrt(x2*x2+y2*y2);
    float z2=xyz.get_valor(2,0);

    // cout<<"x2= "<<x2<<" ,y2= "<<y2<<" ,z2= "<<z2<<endl;

    float theta_2_rad=acos((L01*L01+L12*L12-w2*w2-z2*z2)/(2*L01*L12));
    ang_2=theta_2_rad*180/M_PI;

    // cout<<"El ángulo theta_2 es: "<<ang_2<<endl;
    // cout<<" "<<endl;

    float w1=w2-L12*sin(theta_2_rad);
    float z1=z2-L12*cos(theta_2_rad);

    // cout<<"w1= "<<w1<<" ,z1= "<<z1<<endl;

    float theta_1_rad=acos((L12*L12+w1*w1+z1*z1-L01*L01)/(2*L12*L12*(w1*w1+w2*w2)))-M_PI/2;
    ang_1=theta_1_rad*180/M_PI;

    // cout<<"El ángulo theta_1 es: "<<ang_1<<endl;
    // cout<<" "<<endl;

    float w0=w1-L01*cos(theta_1_rad);
    float z0=z1-L01*sin(theta_1_rad);

    // cout<<"w0= "<<w0<<" ,z0= "<<z0<<endl;

    float theta_0_rad=atan(y2/x2);
    ang_0=theta_0_rad*180/M_PI-90;

    // cout<<"El ángulo theta_0 es: "<<ang_0<<endl;
    // cout<<" "<<endl;

    cout<<"theta_0= "<<ang_0<<endl;
    cout<<"theta_1= "<<ang_1<<endl;
    cout<<"theta_2= "<<ang_2<<endl;
    cout<<"theta_3= "<<ang_3<<endl;

}