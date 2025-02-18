#include <cmath>
#include <iostream>
#ifndef MATRIZ_H
#define MATRIZ_H

using namespace std;
 
template <typename T>
class matriz{
    public:
        matriz();
        matriz(int, int);
        ~matriz();

        float angulo_rot_g;
 
        matriz<T> operator+ (const matriz &matriz_2);
        matriz<T> operator- (const matriz &matriz_2);
        matriz<T> operator* (const matriz &matriz_2);
 
        void delete_matriz();
        matriz<T> matriz_rotacion();
        void traspuesta();
        void multiplicar_escalar(T escalar);
        void print_matriz();

        bool simetrica();
        void set_valor(T valor, int row, int col);
        T get_valor(int row, int col) const;
        matriz<T> get_columna(int col);
 
    private:
        T m_ele;
        T m_max;
        T m_min;
        T m_mode;
        T **m_matriz;
 
        int m_dim_matriz;
        int m_cols;
        int m_rows;
};

#endif

// Constructor por defecto: Matriz 2*2
template<typename T>
matriz<T>::matriz(){
    m_rows = 2;
    m_cols = 2;
    m_matriz=nullptr;
}
  
// Constructor por parámetro
template<typename T>
matriz<T>::matriz(int rows , int cols){
    m_cols = cols;
    m_rows = rows;
    m_matriz = new T*[m_rows];
    int i=0;
    for (i = 0; i < m_rows; i++){
        m_matriz[i] = new T[m_cols]();
    }
}
 
// Suma de matrices
template<typename T>
matriz<T> matriz<T>::operator+ (const matriz &matriz_2){
    matriz matriz_result(m_rows, m_cols);
    for (int i = 0; i < m_rows; i++){
        for (int j = 0; j < m_cols; j++){
            matriz_result.m_matriz[i][j] = m_matriz[i][j] + matriz_2.m_matriz[i][j];
        }
    }
    return matriz_result;
}
 
// Resta de matrices
template<typename T>
matriz<T> matriz<T>::operator- (const matriz &matriz_2)
{
    matriz matriz_result(m_rows, m_cols);
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            matriz_result.m_matriz[i][j] = m_matriz[i][j] - matriz_2.m_matriz[i][j];
        }
    }
    return matriz_result;
}
 
// Multiplicación de matrices
template<typename T>
matriz<T> matriz<T>::operator* (const matriz &matriz_2)
{
    matriz matriz_result(m_rows, matriz_2.m_cols);
    T total;
 
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < matriz_2.m_cols; j++) {
            for (int k = 0; k < m_cols; k++) {
                total += (m_matriz[i][k] * matriz_2.m_matriz[k][j]);
            }
            matriz_result.m_matriz[i][j] = total;
            // Reiniciar el total
            total = 0;
        }
    }
    return matriz_result;
}
 
// Verificar si una Matriz es simétrica
template<typename T>
bool matriz<T>::simetrica(){
    if (m_rows != m_cols) {
        return false;
    }
 
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            if (m_matriz[i][j] != m_matriz[j][i]) {
                return false;
            }
        }
    }
    return true;
}

template<typename T>
void matriz<T>::delete_matriz()
{
    for (int i = 0; i < m_rows; i++) {
        delete[] m_matriz[i];
    }
    delete[] m_matriz;
}
 
// Matriz de rotación con ángulo en grados
template<typename T>
matriz<T> matriz<T>::matriz_rotacion(){
    matriz<T> result(m_rows, m_cols);
    float angulo_rad=-angulo_rot_g*M_PI/180;
    result.m_matriz[0][0]=cos(angulo_rad);
    result.m_matriz[0][1]=-sin(angulo_rad);
    result.m_matriz[1][0]=sin(angulo_rad);
    result.m_matriz[1][1]=cos(angulo_rad);

    return result;
}

// Obtener la traspuesta de una Matriz
template<typename T>
void matriz<T>::traspuesta(){
    matriz matriz_result(m_cols, m_rows);
 
    for (int i = 0; i < m_cols; i++) {
        for (int j = 0; j < m_rows; j++) {
            matriz_result.m_matriz[i][j]= m_matriz[j][i];
        }
    }
    matriz_result.print_matriz();
}
 
// Multiplicar a una Matriz por un escalar
template<typename T>
void matriz<T>::multiplicar_escalar(T escalar)
{
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            m_matriz[i][j] = m_matriz[i][j] * escalar;
        }
    }
    cout << "Se multiplicó a la Matriz por el escalar " << escalar << endl;
}
 
// Imprimir Matriz
template<typename T>
void matriz<T>::print_matriz()
{
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            cout << m_matriz[i][j] << " ";
        }
        cout << endl << endl;
    }
    cout << endl << endl;
}

// Cambiar valor de una matriz
template<typename T>
void matriz<T>::set_valor(T valor, int row, int col)
{
    if(row>=0 && row<m_rows && col>=0 && col<m_cols)
        m_matriz[row][col]=valor;
    else
        throw out_of_range("Índice fuera de rango.");
}

template <typename T>
T matriz<T>::get_valor(int fila, int col) const {
    if (fila>=0 && fila<m_rows && col>=0 && col<m_cols) {
        return m_matriz[fila][col];
    } else {
        throw out_of_range("Índice fuera de rango");
    }
}


template <typename T>
matriz<T> matriz<T>::get_columna(int col) {
    if (col<0 || col>=m_cols){
        throw out_of_range("Índice de columna fuera de rango");
    }
    matriz<T> columna(m_rows, 1);
    for (int i=0; i<m_rows; i++){
        columna.set_valor(get_valor(i, col), i, 0);
    }
    return columna;
}


// Destructor
template<typename T>
matriz<T>::~matriz() {}


