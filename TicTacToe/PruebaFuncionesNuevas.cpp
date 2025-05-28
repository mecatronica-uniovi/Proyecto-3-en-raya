#include "Tablero.h"
#include <stdio.h>
#include <array>
#include <iostream>
#include <cstring>
using namespace std;

int main() //debug de IA
{
    for (int i = 0; i < 3; ++i) {
        _tablero.filas[i] = BOTH_PLAYERS;
        _tablero.columnas[i] = BOTH_PLAYERS;
        if (i < 2) _tablero.diagonales[i] = BOTH_PLAYERS;
    }



    InitTablero();
    ShowTablero();
    struct Pos dest;


    char input[5];
    char* cmd;
    enum TipoPieza turno;
    turno=PIEZA_O; //el humano juega O y va primero

    int winner=NO_PLAYER;
    while(1)
    {
        if(turno==PIEZA_O) //turno humano
        {
            _checkwinnable();
            printf("\n Introduzca su movimiento, en formato Fila;Columna (Ej. 1;1 para esquina superior izquierda): ");
            gets(input);
            printf("\n");
            if (strstr(input,"r")!=NULL) //si el input es la tecla r
            {
                //reiniciar

                printf("Reiniciando tablero\n");

                for (int i = 0; i < 3; ++i) {
                    _tablero.filas[i] = BOTH_PLAYERS;
                    _tablero.columnas[i] = BOTH_PLAYERS;
                    if (i < 2) _tablero.diagonales[i] = BOTH_PLAYERS;
                }

                InitTablero();
                ShowTablero();
            }
            else
            {
                cmd=input;
                dest.row=atoi(cmd);
                cmd=strstr(cmd,";");
                dest.col=atoi(cmd+1);
            }
            if (casilla_vacia(dest) == 0) {
                printf("Movimiento no valido, casilla ocupada o no valida\n");
                ShowTablero();
                continue; //vuelve al inicio del bucle
            }
            //array<double, 3> dest_user_xyz = ConvertirCoordenadas(dest);
            //printf("Jugador ha seleccionado la casilla: %d;%d (Coordenadas cartesianas: %.2f, %.2f, %.2f)\n", dest.row, dest.col, dest_user_xyz[0], dest_user_xyz[1], dest_user_xyz[2]);
        }
        else //turno ia
        {
            _checkwinnable();
            dest=ia_Nueva(PLAYER_X);
            //array<double, 3> dest_ia_cart = ConvertirCoordenadas(dest);
            //printf("IA ha seleccionado la casilla: %d;%d (Coordenadas cartesianas: %.2f, %.2f, %.2f)\n", dest.row, dest.col, dest_ia_cart[0], dest_ia_cart[1], dest_ia_cart[2]);
        }
        if (strstr(input,"r")==NULL) //si el input NO es la tecla r
        {
            MovimientoCoords coords = MoverFichaDevuelveCoords(turno,dest);
            ShowTablero();
            // El movimiento que se ha realizado va desde coords.origen_xyz a coords.destino_xyz
            printf("Movimiento: (%.2f, %.2f, %.2f) -> (%.2f, %.2f, %.2f)\n",
                   coords.origen_xyz[0], coords.origen_xyz[1], coords.origen_xyz[2],
                   coords.destino_xyz[0], coords.destino_xyz[1], coords.destino_xyz[2]);
        }

        switch (turno) {
            case PIEZA_O:
                turno = PIEZA_X;
            break;
            case PIEZA_X:
                turno = PIEZA_O;
            break;
            default:
            break;
        }

        winner=checkwinner();
        switch (winner) {
            case PLAYER_O:
                printf("Victoria jugador O\n");
                printf("Reiniciando tablero\n");
                InitTablero();
                ShowTablero();
            break;
            case PLAYER_X:
                printf("Victoria jugador X\n");
                printf("Reiniciando tablero\n");
                InitTablero();
                ShowTablero();
            break;
            case NO_PLAYER:
                int empate = checkempate();
                if (empate) {
                    printf("Empate\n");
                    InitTablero();
                    ShowTablero();
                } else {
                    printf("No hay ganador\n");
                }
            break;
        }

        if (strstr(input,"r")!=NULL) //si el input es la tecla r
        {
            turno=PIEZA_O;
        }
    }
}
