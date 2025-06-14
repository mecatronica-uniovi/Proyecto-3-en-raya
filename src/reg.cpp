// Inluyo la cabecera en la que definí las clases
#include "reg.h"

/* --------------------------------------------- CLASE DEL REGULADOR PROPORCIONAL: ---------------------------------------------*/

// Constructor de la clase RegP
RegP::RegP(float Kp){
    Kp_=Kp;             // Guardo Kp copiandolo al atributo Kp_ de la clase
}

// Función de dar valor al Kp
void RegP :: setKp(float valor){
    Kp_=valor;          // Similar al constructor pero como método
}

// Función de calculo de acción de control con regulador proporcional
float RegP :: calcularUk(float ek){
    float Uk=Kp_*ek;    // La acción de control es proporcional al error
    return Uk;
}

/* --------------------------------------------- CLASE DEL REGULADOR PROP-INTEG: ---------------------------------------------*/

// Constructor de la clase RegPI, hereda Kp de la clase RegP
RegPI::RegPI(float Kp, float Ki, float Tm) : RegP(Kp){
    // Copio los valores a los artibutos (con _) de la clase RegPI, Kp no hace falta, lo
    // hereda de RegP
    Ki_=Ki;
    Tm_=Tm;

    // Inicializo valores
    integral=0;
    val_anterior=0;
}

// Función de dar valor al Ki
void RegPI :: setKi(float valor){
    Ki_=valor;      // Similar a setKp de RegP
}

// Función de dar valor al Tm
void RegPI :: setTm(float valor){
    Tm_=valor;      // Como setKi pero con el tiempo de muestreo
}

// Función de cálculo de acción de control con regulador proporcional - integral
float RegPI :: calcularUk(float ek){
    float Uk;

    integral=integral+(Tm_*(ek+val_anterior)/2); // El valor de la integral es el que tenía más el área del trapecio correspondiente, cuya
                                                 // altura es el error respecto al valor anterior y cuya base mide el periodo de muestreo.
    Uk=Kp_*ek+Ki_*integral;                      // La acción de control es la suma de la obtenida con el regulador proporcional (error* Kp)
                                                 // más la dada por el regulador integral (área bajo la curva * Kp = integral * Ki)
    val_anterior=ek;                             // Ahora el valor actual del error pasa a ser el anterior
    return Uk;
}

/* --------------------------------------------- CLASE DEL REGULADOR PROP-INTEG-DERIV: ---------------------------------------------*/

// Constructor de la clase RegPID, hereda de RegPI (que ya hereda de RegP)
RegPID::RegPID(float Kp, float Ki, float Kd, float Tm) : RegPI(Kp, Ki, Tm) {
    Kd_ = Kd;
    derivada = 0;
}

// Función de dar valor a Kd
void RegPID::setKd(float valor) {
    Kd_ = valor;
}

// Función de cálculo de acción de control con regulador PID
float RegPID::calcularUk(float ek) {
    float Uk=0;

    // Parte proporcional e integral (heredada de RegPI)
    integral = integral + (Tm_ * (ek + val_anterior) / 2);
    
    // Derivada del error (aproximación)
    derivada = (ek - val_anterior) / Tm_;
    
    // Acción de control
    Uk = Kp_ * ek + Ki_ * integral + Kd_ * derivada;

    val_anterior = ek;

    return Uk;
}

/*---------------------------------------- PROCESAMIENTO DE NUEVOS VALORES POR TECLADO -------------------------------------------*/
// Implementación para RegP (clase base)
void RegP::procesarComando(const String& cmd, float valor) {
    if (cmd == "KP") {
        setKp(valor);
        Serial.print("Kp cambiado a: ");
        Serial.println(Kp_);
    }
}

void RegP::mostrarValores() {
    Serial.print("Kp: ");
    Serial.println(Kp_);
}

// Implementación para RegPI
void RegPI::procesarComando(const String& cmd, float valor) {
    if (cmd == "KI") {
        setKi(valor);
        Serial.print("Ki cambiado a: ");
        Serial.println(Ki_);
    } else if (cmd == "TM") {
        setTm(valor);
        Serial.print("Tm cambiado a: ");
        Serial.println(Tm_);
    } else {
        RegP::procesarComando(cmd, valor);
    }
}

void RegPI::mostrarValores() {
    RegP::mostrarValores();
    Serial.print("Ki: ");
    Serial.println(Ki_);
    Serial.print("Tm: ");
    Serial.println(Tm_);
}

// Implementación para RegPID
void RegPID::procesarComando(const String& cmd, float valor) {
    if (cmd == "KD") {
        setKd(valor);
        Serial.print("Kd cambiado a: ");
        Serial.println(Kd_);
    } else {
        RegPI::procesarComando(cmd, valor);
    }
}

void RegPID::mostrarValores() {
    RegPI::mostrarValores();
    Serial.print("Kd: ");
    Serial.println(Kd_);
}

void RegPID::procesarComandoSerial(const String& comando) {
    int espacio = comando.indexOf(' ');
    
    if (espacio != -1) {
        String cmd = comando.substring(0, espacio);
        float valor = comando.substring(espacio + 1).toFloat();
        procesarComando(cmd, valor);
    } else if (comando == "VALORES") {
        mostrarValores();
    } else {
        Serial.println("Comando no reconocido");
    }
}