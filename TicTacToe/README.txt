En esta carpeta se encuentran 3 clases: 
CoordenadasUtils.cpp que incluye una función para convertir las posiciones del tablero a coordenadas cartesianas. 
IA.cpp que realiza los movimientos que se asignan al robot. 
Tablero.cpp que incluye todas las funciones necesarias para mostrar el tablero, mover fichas y colocar fichas. 

El archivo: mainUsandoClases.cpp es un programa que permite al usuario realizar una partida contra la IA utilizando
la terminal. 

Para compilar ese archivo: 

g++ mainUsandoClases.cpp Tablero.cpp IA.cpp CoordenadasUtils.cpp -o mainUsandoClases.exe

Para ejecutar: 

.\mainUsandoClases.exe

Además, se mantienen los ficheros originales con los que se hicieron pruebas previas utilizando únicamente funciones
en lugar de clases, estos ficheros son: 

FuncionesTablero.cpp, FuncionesTablero.h, main.cpp y PruebaFuncionesNuevas.cpp

En los ficheros: FuncionesTablero.cpp y FuncionesTablero.h se incluye la declaración de tipos, estructuras y 
funciones empleadas. 
En los ficheros main.cpp y PruebaFuncionesNuevas.cpp son dos archivos de prueba que permiten jugar una partida contra
la IA. 

Para compilar main.cpp:

g++ main.cpp FuncionesTablero.cpp -o main.exe

Para ejecutar main.exe: 

.\main.exe


Para compilar PruebaFuncionesNuevas.cpp:

g++ PruebaFuncionesNuevas.cpp FuncionesTablero.cpp -o PruebaFuncionesNuevas.exe

Para ejecutar PruebaFuncionesNuevas.exe:

.\PruebaFuncionesNuevas.exe