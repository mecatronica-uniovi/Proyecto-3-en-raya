#include "Tablero.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

struct Tablero _tablero;

int main1()
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
    turno=PIEZA_O;

    int winner=NO_PLAYER;
    while(1)
    {
        printf("\n Introduzca su movimiento, en formato Fila;Columna (Ej. 1;1 para esquina superior izquierda): ");
        gets(input);
        printf("\n");

        if (strstr(input,"r")!=NULL) //si el input es la tecla r
        {
            //reiniciar

            printf("Reiniciando tablero\n");

            _tablero.filas[0]=BOTH_PLAYERS;_tablero.filas[1]=BOTH_PLAYERS;_tablero.filas[2]=BOTH_PLAYERS;
            _tablero.columnas[0]=BOTH_PLAYERS;_tablero.columnas[1]=BOTH_PLAYERS;_tablero.columnas[2]=BOTH_PLAYERS;
            _tablero.diagonales[0]=BOTH_PLAYERS;_tablero.diagonales[1]=BOTH_PLAYERS;

            InitTablero();
            ShowTablero();
        }
        else
        {
            cmd=input;
            dest.row=atoi(cmd);
            cmd=strstr(cmd,";");
            dest.col=atoi(cmd+1);

            MoverFicha(turno,dest);
            ShowTablero();


            if (turno==PIEZA_O)
            {
                turno=PIEZA_X;
            }
            else
            {
                turno=PIEZA_O;
            }

            winner=checkwinner();

            if (winner==PLAYER_O)
            {
                printf("Victoria jugador O\n");
                printf("Reiniciando tablero\n");
                InitTablero();
                ShowTablero();
            }
            else if (winner==PLAYER_X)
            {
                printf("Victoria jugador X\n");
                printf("Reiniciando tablero\n");
                InitTablero();
                ShowTablero();
            }
            else
            {

            }
        }
    }
}


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

                _tablero.filas[0]=BOTH_PLAYERS;_tablero.filas[1]=BOTH_PLAYERS;_tablero.filas[2]=BOTH_PLAYERS;
                _tablero.columnas[0]=BOTH_PLAYERS;_tablero.columnas[1]=BOTH_PLAYERS;_tablero.columnas[2]=BOTH_PLAYERS;
                _tablero.diagonales[0]=BOTH_PLAYERS;_tablero.diagonales[1]=BOTH_PLAYERS;

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
        }
        else //turno ia
        {
            _checkwinnable();
            dest=ia(PLAYER_X);
        }
        if (strstr(input,"r")==NULL) //si el input NO es la tecla r
        {
            MoverFicha(turno,dest);
            ShowTablero();
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
