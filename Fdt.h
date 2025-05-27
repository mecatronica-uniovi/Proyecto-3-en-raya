#ifndef FDT_H
#define FDT_H

/* CLASE FDT: Los atributos son de tipo privado por defecto lo que significa
que no se podrá acceder a ellos desde fuera de la clase. Los métodos también
son privados por lo que solo se pueden invocar dentro de la clase. */
class Fdt{
    // Atributos privados:
    int Grado_;
    double *ptrNum_;
    double *ptrDen_;
    double *ptrVectorX_;
    double *ptrVectorY_;

    // Métodos privados:
    double ProductoEscalar(double *ptrA, double *ptrB);
    void Desplazamiento(double *ptrC);

public:
    // Constructor (mismo nombre que la clase):
    Fdt(int Grado,double *ptrNum,double *ptrDen);

    // Método público de cálulo de la salida en función de la entrada:
    double CalcularSalida(double Xk);
};

#endif // FDT_H
