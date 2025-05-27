#ifndef REG_H
#define REG_H

class RegP{
    protected:
        float Kp_;
    public:
        // Constructor:
        RegP(float Kp);

        // Métodos:
        void setKp(float valor);
        virtual float calcularUk(float ek); // La declaro virtual porque se va a volver a
                                            // definir en clases derivadas
};

class RegPI:public RegP{
    protected:
        float Ki_, Tm_, integral, val_anterior;
    public:
        // Constructor
        RegPI(float Kp, float Ki, float Tm);

        // Métodos
        void setKi(float valor);
        void setTm(float valor);
        float calcularUk(float ek);
};

class RegPID:public RegPI{
    protected:
        float Kd_, derivada;
    public:
        // Constructor
        RegPID(float Kp, float Ki, float Kd, float Tm);

        // Métodos
        void setKd(float valor);
        float calcularUk(float ek);
};

#endif // REG_H
