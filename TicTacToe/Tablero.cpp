#include "Tablero.h"
using namespace std;

Tablero _tablero; // Definición de la variable global _tablero
//funcion de interfaz en windows, no utilizada en el codigo final
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

//funcion de interfaz en windows, no utilizada en el codigo final
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

//funcion que escribe el estado del tablero como una cadena de caracteres
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

//funcion que busca la posicion de una ficha aun no jugada
struct Pos GetColPrimeraFichaDisponible(enum TipoPieza tipo)
{
    int noficha;
    struct Pos pos;
    pos.row=-1;
    pos.col=-1;
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

//funcion que desplaza una ficha del tipo indicado a la posicion de destino deseada
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

//funcion que comprueba si alguno de los jugadores ha ganado
int checkwinner()
{
    int i, n;
    
    // Inicialización de arrays
    int filas[3], columnas[3];
    for (i = 0; i < 3; i++) {  // Usar la i ya declarada
        filas[i] = _tablero.datos[i+1][1];
        columnas[i] = _tablero.datos[1][i+1];
    }
    
    int diagonales[2];
    diagonales[0] = _tablero.datos[1][1];
    diagonales[1] = _tablero.datos[3][1];
    
    // Verificación de líneas
    for (n = 0; n < 3; n++) {
        for (i = 1; i < 4; i++) {
            if (filas[n] != _tablero.datos[n+1][i]) {
                filas[n] = NO_PIEZA;
            }
            if (columnas[n] != _tablero.datos[i][n+1]) {
                columnas[n] = NO_PIEZA;
            }
        }
        
        if (diagonales[0] != _tablero.datos[n+1][n+1]) {
            diagonales[0] = NO_PIEZA;
        }
        if (diagonales[1] != _tablero.datos[3-n][n+1]) {
            diagonales[1] = NO_PIEZA;
        }
    }
    
    // Verificación de ganador - retorna inmediatamente al encontrar uno
    if (filas[0] == PIEZA_O || filas[1] == PIEZA_O || filas[2] == PIEZA_O ||
        columnas[0] == PIEZA_O || columnas[1] == PIEZA_O || columnas[2] == PIEZA_O ||
        diagonales[0] == PIEZA_O || diagonales[1] == PIEZA_O) {
        return PLAYER_O;
    }
    
    if (filas[0] == PIEZA_X || filas[1] == PIEZA_X || filas[2] == PIEZA_X ||
        columnas[0] == PIEZA_X || columnas[1] == PIEZA_X || columnas[2] == PIEZA_X ||
        diagonales[0] == PIEZA_X || diagonales[1] == PIEZA_X) {
        return PLAYER_X;
    }
    return NO_PLAYER;
}

// Función que comprueba si hay un empate una vez se ha completado el tablero
int checkempate()
{
    int i, j;
    for (i = 1; i < 4; i++) {
        for (j = 1; j < 4; j++) {
            if (_tablero.datos[i][j] == NO_PIEZA) {
                return 0; // No hay empate, hay casillas vacías
            }
        }
    }
    return 1; // Tablero lleno, es un empate
}

//funcion que actualiza las variables globales sobre que jugador puede ganar en cada linea
void _checkwinnable()
{
    for (int n = 0; n < 3; n++) {
        for (int i = 1; i < 4; i++) {
            // Filas
            if (_tablero.filas[n] == BOTH_PLAYERS) {
                if (_tablero.datos[n+1][i] == PIEZA_O) _tablero.filas[n] = PLAYER_O;
                else if (_tablero.datos[n+1][i] == PIEZA_X) _tablero.filas[n] = PLAYER_X;
            } else if (_tablero.filas[n] == PLAYER_O && _tablero.datos[n+1][i] == PIEZA_X) {
                _tablero.filas[n] = NO_PLAYER;
            } else if (_tablero.filas[n] == PLAYER_X && _tablero.datos[n+1][i] == PIEZA_O) {
                _tablero.filas[n] = NO_PLAYER;
            }

            // Columnas
            if (_tablero.columnas[n] == BOTH_PLAYERS) {
                if (_tablero.datos[i][n+1] == PIEZA_O) _tablero.columnas[n] = PLAYER_O;
                else if (_tablero.datos[i][n+1] == PIEZA_X) _tablero.columnas[n] = PLAYER_X;
            } else if (_tablero.columnas[n] == PLAYER_O && _tablero.datos[i][n+1] == PIEZA_X) {
                _tablero.columnas[n] = NO_PLAYER;
            } else if (_tablero.columnas[n] == PLAYER_X && _tablero.datos[i][n+1] == PIEZA_O) {
                _tablero.columnas[n] = NO_PLAYER;
            }

            // Diagonales
            if (n == 0) {
                if (_tablero.diagonales[0] == BOTH_PLAYERS) {
                    if (_tablero.datos[i][i] == PIEZA_O) _tablero.diagonales[0] = PLAYER_O;
                    else if (_tablero.datos[i][i] == PIEZA_X) _tablero.diagonales[0] = PLAYER_X;
                } else if (_tablero.diagonales[0] == PLAYER_O && _tablero.datos[i][i] == PIEZA_X) {
                    _tablero.diagonales[0] = NO_PLAYER;
                } else if (_tablero.diagonales[0] == PLAYER_X && _tablero.datos[i][i] == PIEZA_O) {
                    _tablero.diagonales[0] = NO_PLAYER;
                }
            } else if (n == 1) {
                if (_tablero.diagonales[1] == BOTH_PLAYERS) {
                    if (_tablero.datos[4-i][i] == PIEZA_O) _tablero.diagonales[1] = PLAYER_O;
                    else if (_tablero.datos[4-i][i] == PIEZA_X) _tablero.diagonales[1] = PLAYER_X;
                } else if (_tablero.diagonales[1] == PLAYER_O && _tablero.datos[4-i][i] == PIEZA_X) {
                    _tablero.diagonales[1] = NO_PLAYER;
                } else if (_tablero.diagonales[1] == PLAYER_X && _tablero.datos[4-i][i] == PIEZA_O) {
                    _tablero.diagonales[1] = NO_PLAYER;
                }
            }
        }
    }
}

//funcion que determina el movimiento que debe realizar el robot
Pos ia(Ganador player)
{
    Ganador adversary; //se determina cual es la pieza del jugador contrario (adversario)
    if (player==PLAYER_O)
    {
        adversary=PLAYER_X;
    } else
    {
        adversary=PLAYER_O;
    }
    struct Pos jugada;
    jugada=win_now(player); //comprobar si alguna linea permite ganar inmediatamente
    if (jugada.row==0) //si no habia forma de ganar en este turno, buscar otra jugada
    {
        //comprobar si alguna linea permite perder inmediatamente
        jugada=win_now(adversary);
    }
    if (jugada.row==0) //si no habia riesgo de derrota inmediata
    {
        //comprobar si reconoce el estado del tablero de jugadas preprogramadas
        //pendiente
    }
    if (jugada.row==0) //si aún no se ha decididido una jugada
    {
        //elige una casilla al azar
        jugada=pick_winable(player); //selecciona una casilla cualquiera que permita ganar

        if (jugada.row==0) //si no existe casilla que permita ganar (partida ya empatada)
        {
            jugada=pick_nopieza(); //elige una casilla vacia cualquiera
        }
    }
    if (jugada.row==0)
    {
        //error en teoria inalcanzable
        //el tablero está lleno, y por tanto no se puede jugar en ninguna casilla
    }
    else
    {
            return jugada; //fin de la función
    }
}

//funcion que comprueba si es posible ganar en un solo turno, y en caso afirmativo devuelve la casilla en que se debe jugar para lograrlo
Pos win_now(Ganador player)
{
    Pos jugada;
    jugada.row=0;jugada.col=0; //inicializar la jugada a una posición inalcanzable
    //se llama a variables globales, que recogen los jugadores que pueden ganar cada una de las lineas

    int i,n;
    int counter=0;
    for(i=0;i<3;i++) //recorre las 8 posibles lineas (3 filas, 3 columnas, 2 diagonales)
    {
        //filas
            jugada.row=i+1;
            if (_tablero.filas[i]==player) //si el unico que puede ganar en la linea es el player
            {
                for (n=1;n<4;n++) //recorrer todas las casillas de la linea
                {
                    if (casilla_vacia({jugada.row, n})) //si la casilla está vacía
                    {
                        counter++; //incrementar el contador
                        jugada.col=n; //almacenar la casilla en memoria
                    }
                }
                //despues de comprobar la linea completa
                if (counter==1) //si solo hay una casilla vacia, en una linea en la que es posible ganar, jugar en ella permite ganar
                {
                    return jugada;
                }
            }
        //columnas
            jugada.col=i+1;
            if (_tablero.columnas[i]==player) //si el unico que puede ganar en la linea es el player
            {
                for (n=1;n<4;n++) //recorrer todas las casillas de la linea
                {
                    if (casilla_vacia({n, jugada.col})) //si la casilla está vacía
                    {
                        counter++; //incrementar el contador
                        jugada.row=n; //almacenar la casilla en memoria
                    }
                }
                //despues de comprobar la linea completa
                if (counter==1) //si solo hay una casilla vacia, en una linea en la que es posible ganar, jugar en ella permite ganar
                {
                    return jugada;
                }
            }
        //diagonales
            if (i==0) //solo en la primera iteración
            {
                if(_tablero.diagonales[0]==player) //si el unico que puede ganar en la linea es el player
                {
                    for (n=1;n<4;n++) //recorrer todas las casillas de la linea
                    {
                        if (casilla_vacia({n, n})) //si la casilla está vacía
                        {
                            counter++; //incrementar el contador
                            jugada.row=n; //almacenar la casilla en memoria
                        }
                    }
                    //despues de comprobar la linea completa
                    if (counter==1) //si solo hay una casilla vacia, en una linea en la que es posible ganar, jugar en ella permite ganar
                    {
                        jugada.col=jugada.row;
                        return jugada;
                    }
                }
                if(_tablero.diagonales[1]==player) //si el unico que puede ganar en la linea es el player
                {
                    for (n=1;n<4;n++) //recorrer todas las casillas de la linea
                    {
                        if (casilla_vacia({n, 3-n})) //si la casilla está vacía
                        {
                            counter++; //incrementar el contador
                            jugada.row=n; //almacenar la casilla en memoria
                        }
                    }
                    //despues de comprobar la linea completa
                    if (counter==1) //si solo hay una casilla vacia, en una linea en la que es posible ganar, jugar en ella permite ganar
                    {
                        jugada.col=3-jugada.row;
                        return jugada;
                    }
                }
            }
        jugada.row=0;jugada.col=0;
    } //si no ha encontrado ninguna jugada ganadora, devuelve (0,0), posición no jugable, para indicarlo
    return jugada;
}

//selecciona una casilla que permita acercarse a la victoria, sin especial relevancia a cual sea esta
Pos pick_winable(Ganador player)
{
    Pos jugada;
    jugada.row=0;jugada.col=0; //inicializar la jugada a una posición inalcanzable
    //se llama a variables globales, que recogen los jugadores que pueden ganar cada una de las lineas

    int i,n;
    for(i=0;i<3;i++) //recorre las 8 posibles lineas (3 filas, 3 columnas, 2 diagonales)
    {
        //filas
        jugada.row=i+1;
        if (_tablero.filas[i]==player) //si el unico que puede ganar en la linea es el player
        {
            for (n=1;n<4;n++) //recorrer todas las casillas de la linea
            {
                if (casilla_vacia({jugada.row, n})) //si la casilla está vacía
                {
                    jugada.col=n; //almacenar la casilla en memoria
                    return jugada;
                }
            }
        }
        //columnas
        jugada.col=i+1;
        if (_tablero.columnas[i]==player) //si el unico que puede ganar en la linea es el player
        {
            for (n=1;n<4;n++) //recorrer todas las casillas de la linea
            {
                if (casilla_vacia({n, jugada.col})) //si la casilla está vacía
                {
                    jugada.row=n; //almacenar la casilla en memoria
                    return jugada;
                }
            }
        }
        //diagonales
        if (i==0) //solo en la primera iteración (da prioridad a las diagonales, estrategicamente mas utiles que otras lineas)
        {
            if(_tablero.diagonales[0]==player) //si el unico que puede ganar en la linea es el player
            {
                for (n=1;n<4;n++) //recorrer todas las casillas de la linea
                {
                    if (casilla_vacia({n, n})) //si la casilla está vacía
                    {
                        jugada.row=n; //almacenar la casilla en memoria
                        jugada.col=jugada.row;
                        return jugada;
                    }
                }
            }
            if(_tablero.diagonales[1]==player) //si el unico que puede ganar en la linea es el player
            {
                for (n=1;n<4;n++) //recorrer todas las casillas de la linea
                {
                    if (_tablero.datos[n][3-n]==NO_PIEZA) //si la casilla está vacía
                    {
                        jugada.row=n; //almacenar la casilla en memoria
                        jugada.col=3-jugada.row;
                        return jugada;
                    }
                }
            }
        }
        jugada.row=0;jugada.col=0;
    } //si no ha encontrado ninguna jugada ganadora, devuelve (0,0), posición no jugable, para indicarlo
    return jugada;
}

//selecciona una casilla vacia sin importar cual
Pos pick_nopieza()
{
    Pos jugada;
    jugada.row=0;jugada.col=0; //inicializar la jugada a una posición inalcanzable
    int i,n;
    for (i=1;i<4;i++) //recorre todas las filas
    {
        for(n=1;n<4;n++) //recorre todas las columnas de la fila actual
        {
            if(_tablero.datos[i][n]==NO_PIEZA) //si la casilla está vacia
            {
                //es una casilla sin pieza, el objetivo de la función
                jugada.row=i;
                jugada.col=n;
                return jugada;
            }
        }
    }
    return jugada; //si no encuentra una casilla vacía, devuelve (0,0), posición no jugable, para indicarlo
}

// Función que comprueba que la casilla está vacía
int casilla_vacia(Pos pos)
{
    // Devuelve 1 si la casilla está vacía (NO_PIEZA), 0 en caso contrario o fuera de rango
    if (pos.row < 1 || pos.row > 3 || pos.col < 1 || pos.col > 3) {
        return 0; // Casilla fuera de rango
    }
    return _tablero.datos[pos.row][pos.col] == NO_PIEZA;
}

// Función que convierte coordenadas de tablero a coordenadas cartesianas
array<double, 3> ConvertirCoordenadas(struct Pos pos) {
    array<double, 3> pos_xyz;
    // Definir los desplazamientos base para filas y columnas
    // Las unidades son mm
    const double base_x = 20.0;
    const double base_y = 0.0;
    const double base_z = 165.0; // La altura del tablero es constante.
    const double step = 39.25; // Distancia entre el centro de dos casillas adyacentes

    // Comprobar si la posición es válida
    if (pos.row >= 0 && pos.row <= 4 && pos.col >= 0 && pos.col <= 4) { //Estamos dentro de los límites del tablero
        // Calcular las coordenadas cartesianas}
        pos_xyz[0] = base_x + step * pos.col; // x depende de la columna
        pos_xyz[1] = base_y + step * pos.row; // y depende de la fila
        pos_xyz[2] = base_z; // z constante
    } else {
        pos_xyz = {-1.0, -1.0, -1.0}; // Casilla no válida
    }
    return pos_xyz;
}


Pos ia_Nueva(Ganador player)
{
    Ganador adversary; //se determina cual es la pieza del jugador contrario (adversario)
    if (player==PLAYER_O)
    {
        adversary=PLAYER_X;
    } else
    {
        adversary=PLAYER_O;
    }
    struct Pos jugada;
    jugada=win_now(player); //comprobar si alguna linea permite ganar inmediatamente
    if (jugada.row==0) //si no habia forma de ganar en este turno, buscar otra jugada
    {
        //comprobar si alguna linea permite perder inmediatamente
        jugada=win_now(adversary);
    }
    if (jugada.row==0) //si no habia riesgo de derrota inmediata
    {
        //comprobar si reconoce el estado del tablero de jugadas preprogramadas
        //pendiente
    }
    if (jugada.row==0) //si aún no se ha decididido una jugada
    {
        //elige una casilla al azar
        jugada=pick_winable(player); //selecciona una casilla cualquiera que permita ganar

        if (jugada.row==0) //si no existe casilla que permita ganar (partida ya empatada)
        {
            jugada=pick_nopieza(); //elige una casilla vacia cualquiera
        }
    }
    if (jugada.row==0)
    {
        //error en teoria inalcanzable
        //el tablero está lleno, y por tanto no se puede jugar en ninguna casilla
    }
    else
    {
            return jugada; //fin de la función
    }

}

MovimientoCoords MoverFichaDevuelveCoords(enum TipoPieza tipo, struct Pos destino)
{
    struct Pos origen;
    MovimientoCoords coords;
    origen=GetColPrimeraFichaDisponible(tipo);
    if (origen.row >= 0 && origen.col >= 0) {
        if (_tablero.datos[destino.row][destino.col] == NO_PIEZA) {
            _tablero.datos[origen.row][origen.col] = NO_PIEZA;
            _tablero.datos[destino.row][destino.col] = tipo;
        }
    }
    coords.origen_xyz = ConvertirCoordenadas(origen);
    coords.destino_xyz = ConvertirCoordenadas(destino);
    return coords; // Devolver las coordenadas de destino
}
