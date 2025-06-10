#include "funciones_motores.h"
#include "funciones_cinem.h"
#include "motores.h"
#include "configuracion.h"
#include <array>

void LecturaEncoders(Motores& motor1, Motores& motor2, Motores& motor3){
    motor1._pos=motor1.leerEncoder();
    motor2._pos=motor2.leerEncoder();
    motor3._pos=motor3.leerEncoder();
}

void AsociaRegMotor(array<RegPID*, 3>& reguladores, array<Motores*, 3>& motores){
    for (int i=0; i<3; i++){
        motores[i]->setRegulador(reguladores[i]);
        cout<<"Motor "<<i<<" asociado a regulador "<< i<<endl;
    }
}

void MoverMotores(Motores& motor1, float PWM1, Motores& motor2, float PWM2, Motores& motor3, float PWM3){
    // Movimiento de motores
    motor1.moverPWM(PWM1);
    motor2.moverPWM(PWM2);
    motor3.moverPWM(PWM3);
}

void ControlMotores(Motores& motor1, Motores& motor2, Motores& motor3, array<double, 3>& xyz, const array<double, 4>& L){
    array<double, 4> angulos_pos=cinematicaInversa(xyz, L); // Referencia de ángulo

    if (!motor1.regulador || !motor2.regulador || !motor3.regulador){
        cout<<"Error: Regulador no inicializado en algun motor"<<endl;
        return;
    }
    
    float error_m1=motor1._pos-angulos_pos[0];
    // float error_m2=motor2._pos-angulos_pos[1];
    // float error_m3=motor3._pos-angulos_pos[2];
    float error_m2=0;
    float error_m3=0;

    #if LOG_LEVEL>=2
        cout<<"Error del motor 1: "<<error_m1<<endl;
        cout<<"Error del motor 2: "<<error_m2<<endl;
        cout<<"Error del motor 3: "<<error_m3<<endl;
    #endif

    float Uk_m1=motor1.regulador->calcularUk(error_m1);
    // float Uk_m2=motor2.regulador->calcularUk(error_m2);
    // float Uk_m3=motor3.regulador->calcularUk(error_m3);
    float Uk_m2=0;
    float Uk_m3=0;

    Uk_m1 = constrain(Uk_m1, PWM_MIN, PWM_MAX);
    // Uk_m2 = constrain(Uk_m2, PWM_MIN, PWM_MAX);
    // Uk_m3 = constrain(Uk_m3, PWM_MIN, PWM_MAX);

    #if LOG_LEVEL>=2
        cout<<"Uk_m1= "<<Uk_m1<<endl;
        cout<<"Uk_m2= "<<Uk_m2<<endl;
        cout<<"Uk_m3= "<<Uk_m3<<endl;
    #endif
    
    MoverMotores(motor1, Uk_m1, motor2, Uk_m2, motor3, Uk_m3);
}