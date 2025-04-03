#include "Tablero.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

struct Tablero _tablero;

int main()
{
    _tablero.filas[0]=BOTH_PLAYERS;_tablero.filas[1]=BOTH_PLAYERS;_tablero.filas[2]=BOTH_PLAYERS;
    _tablero.columnas[0]=BOTH_PLAYERS;_tablero.columnas[1]=BOTH_PLAYERS;_tablero.columnas[2]=BOTH_PLAYERS;
    _tablero.diagonales[0]=BOTH_PLAYERS;_tablero.diagonales[1]=BOTH_PLAYERS;



    InitTablero();
    ShowTablero();
    struct Pos dest;


    char input[5];
    char* cmd;
    enum TipoPieza turno;
    turno=PIEZA_O;

    int winner;
    winner=NO_PLAYER;
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
