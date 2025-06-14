#ifndef REG_H
#define REG_H

#include <Arduino.h>

class RegP {
protected:
    float Kp_;
public:
    RegP(float Kp);
    void setKp(float valor);
    virtual float calcularUk(float ek);
    virtual void procesarComando(const String& cmd, float valor);
    virtual void mostrarValores();
};

class RegPI : public RegP {
protected:
    float Ki_, Tm_, integral, val_anterior;
public:
    RegPI(float Kp, float Ki, float Tm);
    void setKi(float valor);
    void setTm(float valor);
    float calcularUk(float ek) override;
    void procesarComando(const String& cmd, float valor) override;
    void mostrarValores() override;
};

class RegPID : public RegPI {
protected:
    float Kd_, derivada;
public:
    RegPID(float Kp, float Ki, float Kd, float Tm);
    void setKd(float valor);
    float calcularUk(float ek) override;
    void procesarComando(const String& cmd, float valor) override;
    void mostrarValores() override;
    void procesarComandoSerial(const String& comando);
    
};

#endif // REG_H