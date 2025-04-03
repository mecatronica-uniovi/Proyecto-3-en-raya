#include "Tablero.h"
#include <stdio.h>

void InitTablero()
{
    int row,col;
    for (col=0;col<5;col++)
    {
        _tablero.datos[I_ROW_O][col]=PIEZA_O;
        _tablero.datos[I_ROW_X][col]=PIEZA_X;
    }
    for (col=0;col<=4;col++)
    {
        for (row=1;row<=3;row++)
        {
            _tablero.datos[row][col]=NO_PIEZA;
        }
    }
}

/*void ShowTablero()
{
    char txt[40];
    TableroAString(txt);
    printf(txt);
}*/

void ShowTablero()
{
    int row,col;
    for (row=0;row<5;row++)
    {
        for (col=0;col<5;col++)
        {
            switch (_tablero.datos[row][col])
            {
            case NO_PIEZA:
            {
                printf(" ");
                break;
            }
            case PIEZA_O:
            {
                printf("O");
                break;
            }
            case PIEZA_X:
            {
                printf("X");
                break;
            }
            case CASILLA_NO_VALIDA:
            {
                printf(" ");
                break;
            }
            }

            if (col<4&&row>0&&row<4)
            {
                printf("|");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");

        if (row<4)
        {
            printf("  - - -  \n");
        }
    }
    printf("\n");
}

void TableroAString(char dest[]) //esta funcion no solo es util para mostrar en pantalla, genera una cadena utilizable para transmitir informacion a otros dispositivos
{
    int row,col;
    int posString=0;
    for (row=0;row<5;row++)
    {
        for (col=0;col<5;col++)
        {
            switch (_tablero.datos[row][col])
            {
            case NO_PIEZA:
            {
                posString+=sprintf(dest+posString," ");
                break;
            }
            case PIEZA_O:
            {
                posString+=sprintf(dest+posString,"O");
                break;
            }
            case PIEZA_X:
            {
                posString+=sprintf(dest+posString,"X");
                break;
            }
            case CASILLA_NO_VALIDA:
            {
                posString+=sprintf(dest+posString," ");
                break;
            }
            }
        }
        posString+=sprintf(dest+posString,"\n");
    }
}

struct Pos GetColPrimeraFichaDisponible(enum TipoPieza tipo)
{
    int noficha;
    struct Pos pos;
    pos.row=-1;
    pos.col=-1; //el fallo es este, no se sobreescribe la columna
    switch (tipo)
    {
    case NO_PIEZA:
        return pos;
    case PIEZA_O:
    {
        pos.row=I_ROW_O;
        break;
    }
    case PIEZA_X:
    {
        pos.row=I_ROW_X;
        break;
    }
    case CASILLA_NO_VALIDA:
        return pos;
    }
    for (noficha=1,pos.col=0;(pos.col<6)&&noficha;pos.col++)
    {
        if(_tablero.datos[pos.row][pos.col]==tipo)
        {
            noficha=0;
        }
    }
    if (noficha==0)
    {
        pos.col--;
    }
    if (pos.col>5)
    {
        pos.col=-1;
    }
    return pos;
}

void MoverFicha(enum TipoPieza tipo, struct Pos destino)
{
    struct Pos origen;
    origen=GetColPrimeraFichaDisponible(tipo);
    if (origen.row>=0 && origen.col>=0)
    {
        if(_tablero.datos[destino.row][destino.col]==NO_PIEZA)
        {
            _tablero.datos[origen.row][origen.col]=NO_PIEZA;
            _tablero.datos[destino.row][destino.col]=tipo;
        }
    }
}

int checkwinner()
{
    int winner;
    int i,n;
    int filas[3];filas[0]=_tablero.datos[1][1];filas[1]=_tablero.datos[2][1];filas[2]=_tablero.datos[3][1];
    int columnas[3];columnas[0]=_tablero.datos[1][1];columnas[1]=_tablero.datos[1][2];columnas[2]=_tablero.datos[1][3];
    int diagonales[2];diagonales[0]=_tablero.datos[1][1];diagonales[1]=_tablero.datos[3][1];
    for (n=0;n<3;n++)
    {
        for (i=1;i<4;i++)
        {
            if (filas[n]==_tablero.datos[n+1][i])
            {

            }
            else
            {
                filas[n]=NO_PIEZA;
            }
            if (columnas[n]==_tablero.datos[i][n+1])
            {

            }
            else
            {
                columnas[n]=NO_PIEZA;
            }
        }

        if (diagonales[0]==_tablero.datos[n+1][n+1])
        {

        }
        else
        {
            diagonales[0]=NO_PIEZA;
        }
        if (diagonales[1]==_tablero.datos[3-n][n+1])
        {

        }
        else
        {
            diagonales[1]=NO_PIEZA;
        }
    }

    if (filas[0]!=NO_PIEZA)
        winner=filas[0];
    if (filas[1]!=NO_PIEZA)
        winner=filas[1];
    if (filas[2]!=NO_PIEZA)
        winner=filas[2];
    if (columnas[0]!=NO_PIEZA)
        winner=columnas[0];
    if (columnas[1]!=NO_PIEZA)
        winner=columnas[1];
    if (columnas[2]!=NO_PIEZA)
        winner=columnas[2];
    if (diagonales[0]!=NO_PIEZA)
        winner=diagonales[0];
    if (diagonales[1]!=NO_PIEZA)
        winner=diagonales[1];

    if (winner==PIEZA_O)
    {
        winner=PLAYER_O;
    }
    else if (winner==PIEZA_X)
    {
        winner=PLAYER_X;
    }
    else
    {
        winner=NO_PLAYER;
    }

    return winner;
}

/*void checkwinnable(enum TipoPieza tipo) //egoista
{
    int adversary;
    if (tipo==PIEZA_X)
    {
        adversary=PIEZA_O;
    }
    else
    {
        adversary=PIEZA_X;
    }
    int i,n;
    int filas[3];filas[0]=0;filas[1]=0;filas[2]=0;
    int columnas[3];columnas[0]=0;columnas[1]=0;columnas[2]=0;
    int diagonales[2];diagonales[0]=0;diagonales[1]=0;
    for (n=0;n<3;n++)
    {
        for (i=1;i<4;i++)
        {
            if ((_tablero.datos[i][1]!=adversary)&&(filas[n]!=NO_PLAYER))
            {
                if (_tablero.datos[i][1]==tipo)
                {
                    filas[n]++;
                }
                //mientras no haya una pieza del adversario bloqueando la linea
            }
            else
            {
                filas[n]=NO_PLAYER;
            }
            if ((_tablero.datos[1][i]!=adversary)&&(columnas[n]!=NO_PLAYER))
            {
                if (_tablero.datos[1][i]==tipo)
                {
                    columnas[n]++;
                }
            }
            else
            {
                columnas[n]=NO_PLAYER;
            }
        }

        if ((_tablero.datos[n+1][n+1]!=adversary)&&(diagonales[0]!=NO_PLAYER))
        {
            if (_tablero.datos[n+1][n+1]==tipo)
                diagonales[0]++;
        }
        else
        {
            diagonales[0]=NO_PLAYER;
        }
        if ((_tablero.datos[3-n][n+1]!=adversary)&&(diagonales[1]!=NO_PLAYER))
        {
            if (_tablero.datos[3-n][n+1]==tipo)
                diagonales[1]++;
        }
        else
        {
            diagonales[1]=NO_PLAYER;
        }
    }
}*/



void _checkwinnable()
{
    int i,n;
    for (n=0;n<3;n++)
    {
        for (i=1;i<4;i++)
        {
            //comprobacion de filas

            if (_tablero.filas[n]==NO_PLAYER)
            {
                //si segun memoria no es posible ganar en esta linea, no comprobar
            }
            else if (_tablero.filas[n]==BOTH_PLAYERS)
            {
                //si hasta el momento no habia fichas en la linea (ambos juugadores pueden ganar en ella)
                //actualizar player EN CASO DE HABER PIEZA
                if (_tablero.datos[n+1][i]==PIEZA_O)
                {
                    _tablero.filas[n]=PLAYER_O;
                }
                else if (_tablero.datos[n+1][i]==PIEZA_X)
                {
                    _tablero.filas[n]=PLAYER_X;
                }
            }
            else if (_tablero.filas[n]==PLAYER_O)
            {
                //si en esta linea solo puede ganar player O
                if (_tablero.datos[n+1][i]==PIEZA_X)
                {
                    //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                    _tablero.filas[n]=NO_PLAYER;
                }
            }
            else if (_tablero.filas[n]==PLAYER_X)
            {
                //si en esta linea solo puede ganar player X
                if (_tablero.datos[n+1][i]==PIEZA_O)
                {
                    //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                    _tablero.filas[n]=NO_PLAYER;
                }
            }

            //comprobacion de columnas

            if(_tablero.columnas[n]==NO_PLAYER)
            {
                //si segun memoria no es posible ganar en esta linea, no comprobar
            }
            //si hasta el momento no habia fichas en la linea (ambos juugadores pueden ganar en ella)
            //actualizar player EN CASO DE HABER PIEZA
            if (_tablero.datos[i][n+1]==PIEZA_O)
            {
                _tablero.columnas[n]=PLAYER_O;
            }
            else if (_tablero.datos[i][n+1]==PIEZA_X)
            {
                _tablero.columnas[n]=PLAYER_X;
            }
            else if (_tablero.columnas[n]==PLAYER_O)
            {
                //si en esta linea solo puede ganar player O
                if (_tablero.datos[i][n+1]==PIEZA_X)
                {
                    //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                    _tablero.columnas[n]=NO_PLAYER;
                }
            }
            else if (_tablero.columnas[n]==PLAYER_X)
            {
                //si en esta linea solo puede ganar player X
                if (_tablero.datos[i][n+1]==PIEZA_O)
                {
                    //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                    _tablero.columnas[n]=NO_PLAYER;
                }
            }

            //comprobacion de diagonales
            if (n==0)
            {
                if (_tablero.diagonales[n]==NO_PLAYER)
                {
                    //si segun memoria no es posible ganar en esta linea, no comprobar
                }
                else if (_tablero.diagonales[n]==BOTH_PLAYERS)
                {
                    //si hasta el momento no habia fichas en la linea (ambos juugadores pueden ganar en ella)
                    //actualizar player EN CASO DE HABER PIEZA
                    if (_tablero.datos[i][i]==PIEZA_O)
                    {
                        _tablero.diagonales[n]=PLAYER_O;
                    }
                    else if (_tablero.datos[i][i]==PIEZA_X)
                    {
                        _tablero.diagonales[n]=PLAYER_X;
                    }
                }
                else if (_tablero.diagonales[n]==PLAYER_O)
                {
                    //si en esta linea solo puede ganar player O
                    if (_tablero.datos[i][i]==PIEZA_X)
                    {
                        //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                        _tablero.diagonales[n]=NO_PLAYER;
                    }
                }
                else if (_tablero.diagonales[n]==PLAYER_X)
                {
                    //si en esta linea solo puede ganar player X
                    if (_tablero.datos[i][i]==PIEZA_O)
                    {
                        //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                        _tablero.diagonales[n]=NO_PLAYER;
                    }
                }
            }
            else if (n==1)
            {
                if (_tablero.diagonales[n]==NO_PLAYER)
                {
                    //si segun memoria no es posible ganar en esta linea, no comprobar
                }
                else if (_tablero.diagonales[n]==BOTH_PLAYERS)
                {
                    //si hasta el momento no habia fichas en la linea (ambos juugadores pueden ganar en ella)
                    //actualizar player EN CASO DE HABER PIEZA
                    if (_tablero.datos[4-i][i]==PIEZA_O)
                    {
                        _tablero.diagonales[n]=PLAYER_O;
                    }
                    else if (_tablero.datos[4-i][i]==PIEZA_X)
                    {
                        _tablero.diagonales[n]=PLAYER_X;
                    }
                }
                else if (_tablero.diagonales[n]==PLAYER_O)
                {
                    //si en esta linea solo puede ganar player O
                    if (_tablero.datos[4-i][i]==PIEZA_X)
                    {
                        //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                        _tablero.diagonales[n]=NO_PLAYER;
                    }
                }
                else if (_tablero.diagonales[n]==PLAYER_X)
                {
                    //si en esta linea solo puede ganar player X
                    if (_tablero.datos[4-i][i]==PIEZA_O)
                    {
                        //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                        _tablero.diagonales[n]=NO_PLAYER;
                    }
                }
            }
        }
    }
}

//toda la funcion anterior se podria reducir con una funcion que contenga
/*
                if (diagonales[n]==NO_PLAYER)
                {
                    //si segun memoria no es posible ganar en esta linea, no comprobar
                }
                else if (diagonales[n]==BOTH_PLAYERS)
                {
                    //si hasta el momento no habia fichas en la linea (ambos juugadores pueden ganar en ella)
                    //actualizar player EN CASO DE HABER PIEZA
                    if (_tablero.datos[i][i]==PIEZA_O)
                    {
                        diagonales[n]=PLAYER_O;
                    }
                    else if (_tablero.datos[i][i]==PIEZA_X)
                    {
                        diagonales[n]=PLAYER_X;
                    }
                }
                else if (diagonales[n]==PIEZA_O)
                {
                    //si en esta linea solo puede ganar player O
                    if (_tablero.datos[i][i]==PIEZA_X)
                    {
                        //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                        diagonales[n]=NO_PLAYER;
                    }
                }
                else if (diagonales[n]==PIEZA_X)
                {
                    //si en esta linea solo puede ganar player X
                    if (_tablero.datos[i][i]==PIEZA_O)
                    {
                        //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                        diagonales[n]=NO_PLAYER;
                    }
                }
*/
//que es una seccion de codigo que se repite 4 veces (con modificacion de variables, pero mismo codigo)

void checkwinnable() //version local de la funcion para simulaciones de accion
{
    int filas[3];filas[0]=_tablero.filas[0];filas[1]=_tablero.filas[1];filas[2]=_tablero.filas[2];
    int columnas[3];columnas[0]=_tablero.columnas[0];columnas[1]=_tablero.columnas[1];columnas[2]=_tablero.columnas[2];
    int diagonales[2];diagonales[0]=_tablero.diagonales[0];diagonales[1]=_tablero.diagonales[1];
    int i,n;
    for (n=0;n<3;n++)
    {
        for (i=1;i<4;i++)
        {
            //comprobacion de filas

            if (filas[n]==NO_PLAYER)
            {
                //si segun memoria no es posible ganar en esta linea, no comprobar
            }
            else if (filas[n]==BOTH_PLAYERS)
            {
                //si hasta el momento no habia fichas en la linea (ambos juugadores pueden ganar en ella)
                //actualizar player EN CASO DE HABER PIEZA
                if (_tablero.datos[n+1][i]==PIEZA_O)
                {
                    filas[n]=PLAYER_O;
                }
                else if (_tablero.datos[n+1][i]==PIEZA_X)
                {
                    filas[n]=PLAYER_X;
                }
            }
            else if (filas[n]==PLAYER_O)
            {
                //si en esta linea solo puede ganar player O
                if (_tablero.datos[n+1][i]==PIEZA_X)
                {
                    //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                    filas[n]=NO_PLAYER;
                }
            }
            else if (filas[n]==PLAYER_X)
            {
                //si en esta linea solo puede ganar player X
                if (_tablero.datos[n+1][i]==PIEZA_O)
                {
                    //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                    filas[n]=NO_PLAYER;
                }
            }

            //comprobacion de columnas

            if(columnas[n]==NO_PLAYER)
            {
                //si segun memoria no es posible ganar en esta linea, no comprobar
            }
            //si hasta el momento no habia fichas en la linea (ambos juugadores pueden ganar en ella)
            //actualizar player EN CASO DE HABER PIEZA
            if (_tablero.datos[i][n+1]==PIEZA_O)
            {
                columnas[n]=PLAYER_O;
            }
            else if (_tablero.datos[i][n+1]==PIEZA_X)
            {
                columnas[n]=PLAYER_X;
            }
            else if (columnas[n]==PLAYER_O)
            {
                //si en esta linea solo puede ganar player O
                if (_tablero.datos[i][n+1]==PIEZA_X)
                {
                    //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                    columnas[n]=NO_PLAYER;
                }
            }
            else if (columnas[n]==PLAYER_X)
            {
                //si en esta linea solo puede ganar player X
                if (_tablero.datos[i][n+1]==PIEZA_O)
                {
                    //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                    columnas[n]=NO_PLAYER;
                }
            }

            //comprobacion de diagonales
            if (n==0)
            {
                if (diagonales[n]==NO_PLAYER)
                {
                    //si segun memoria no es posible ganar en esta linea, no comprobar
                }
                else if (diagonales[n]==BOTH_PLAYERS)
                {
                    //si hasta el momento no habia fichas en la linea (ambos juugadores pueden ganar en ella)
                    //actualizar player EN CASO DE HABER PIEZA
                    if (_tablero.datos[i][i]==PIEZA_O)
                    {
                        diagonales[n]=PLAYER_O;
                    }
                    else if (_tablero.datos[i][i]==PIEZA_X)
                    {
                        diagonales[n]=PLAYER_X;
                    }
                }
                else if (diagonales[n]==PLAYER_O)
                {
                    //si en esta linea solo puede ganar player O
                    if (_tablero.datos[i][i]==PIEZA_X)
                    {
                        //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                        diagonales[n]=NO_PLAYER;
                    }
                }
                else if (diagonales[n]==PLAYER_X)
                {
                    //si en esta linea solo puede ganar player X
                    if (_tablero.datos[i][i]==PIEZA_O)
                    {
                        //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                        diagonales[n]=NO_PLAYER;
                    }
                }
            }
            else if (n==1)
            {
                if (diagonales[n]==NO_PLAYER)
                {
                    //si segun memoria no es posible ganar en esta linea, no comprobar
                }
                else if (diagonales[n]==BOTH_PLAYERS)
                {
                    //si hasta el momento no habia fichas en la linea (ambos juugadores pueden ganar en ella)
                    //actualizar player EN CASO DE HABER PIEZA
                    if (_tablero.datos[4-i][i]==PIEZA_O)
                    {
                        diagonales[n]=PLAYER_O;
                    }
                    else if (_tablero.datos[4-i][i]==PIEZA_X)
                    {
                        diagonales[n]=PLAYER_X;
                    }
                }
                else if (diagonales[n]==PLAYER_O)
                {
                    //si en esta linea solo puede ganar player O
                    if (_tablero.datos[4-i][i]==PIEZA_X)
                    {
                        //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                        diagonales[n]=NO_PLAYER;
                    }
                }
                else if (diagonales[n]==PLAYER_X)
                {
                    //si en esta linea solo puede ganar player X
                    if (_tablero.datos[4-i][i]==PIEZA_O)
                    {
                        //si aparece una pieza del adversario en la linea, ya no es posible ganar en ella
                        diagonales[n]=NO_PLAYER;
                    }
                }
            }
        }
    }
}
