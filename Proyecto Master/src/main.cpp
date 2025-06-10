#include "configuracion.h"
#include "motores.h"
#include "reg.h"
#include "funciones_motores.h"
#include "Fdt.h"

using namespace std;

// Definiciones con inicialización
double num[]={0, 0, 2.099};
double den[]= {0, 0.238, 1};
extern float Ts;

// Funcion de transferencia del motor
Fdt FuncionTrans(3, num, den);

// Regulador PID empleado
// RegPID Regulador(100, 200, 10, Ts); // Para prueba con escalón unitario
RegPID PID1(0.01, 0.01, 0.01, Ts);
RegPID PID2(0.01, 0.01, 0.01, Ts);
RegPID PID3(0.01, 0.01, 0.01, Ts);
array<RegPID*, 3> reguladores={&PID1, &PID2, &PID3};

// Definición de motores
Motores motor1;
Motores motor2;
Motores motor3;
Motores servo_pinza;
array<Motores*, 3> motores={&motor1, &motor2, &motor3};

/*PARA PROBAR LAZO DE CONTROL DE UN MOTOR -----------------------------------------------------------------------------*/
array<double, 3> xyz_des={0,200,0};


void setup(){
    Serial.begin(115200);
    
    motor1.configurarPines(IN1_M1, IN2_M1, CS1);
    motor1.asignarFDC(fc1, fc2);
    motor1.asignarEncoder(CS1);

    motor2.configurarPines(IN1_M2, IN2_M2, CS2);
    motor2.asignarFDC(fc3, fc4);
    motor2.asignarEncoder(CS2);

    motor3.configurarPines(IN1_M3, IN2_M3, CS3);
    motor3.asignarFDC(fc5, fc6);
    motor3.asignarEncoder(CS3);

    AsociaRegMotor(reguladores, motores);

    // while (!Serial);
    // Serial.println("Sistema de Control PID");
    // Serial.println("Comandos disponibles:");
    // Serial.println("KP valor - Cambiar ganancia proporcional");
    // Serial.println("KI valor - Cambiar ganancia integral");
    // Serial.println("KD valor - Cambiar ganancia derivativa");
    // Serial.println("TM valor - Cambiar tiempo de muestreo");
    // Serial.println("VALORES - Mostrar valores actuales");
    // Serial.println("RUN - Ejecutar simulacion");

}

/*PRUEBA DE LECTURA DE ENCODERS----------------------------------------------------------------------------------------*/
// void loop(){
//     float ang1= motor1.leerEncoder();
//     float ang2= motor2.leerEncoder();
//     float ang3= motor3.leerEncoder();

//     Serial.print(ang1);
//     Serial.print(",");
//     Serial.print(ang2);
//     Serial.print(",");
//     Serial.println(ang3);

//     delay(250);
// }

/* PRUEBA SIMULACIÓN LAZO DE CONTROL ESCALÓN UNITARIO -----------------------------------------------------------------*/
// void loop(){
//     if (Serial.available() > 0) {
//         String comando = Serial.readStringUntil('\n');
//         comando.trim();
//         if (comando == "RUN") {
//             ejecutarSimulacion();
//         } else {
//             Regulador.procesarComandoSerial(comando);
//         }
//     }
//     array<bool,3> fdc=Final_Carrera();
//     for (int i=0; i<3; i++){
//         if(fdc[i]==true){
//             cout<<"FDC "<<i<<" activo, parada de motor "<<i<<endl;
//         }
//     }
// }

// /*PRUEBA DE LECTURA DE FDC -----------------------------------------------------------------------------------------*/
// void loop() {
//     array<bool,3> fdc = Final_Carrera(); // Llama a tu función revisada

//     if (fdc[0] == true) {
//         Serial.println("Motor 1 está parado.");
//     }
//     if (fdc[1] == true) {
//         Serial.println("Motor 2 está parado.");
//     }
//     if (fdc[2] == true) {
//         Serial.println("Motor 3 está parado.");
//     }
//     cout<<"\n "<<endl;

//     delay(2000);
// }


void loop(){
    ControlMotores(motor1, motor2, motor3, xyz_des, L_eslab);
    //motor1.detectarInterrupcion();
    //motor1.moverAGrados(anguloDeseado);
}
