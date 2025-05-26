#ifndef TABLERO_H
#define TABLERO_H

//definiciones de filas iniciales
#define I_ROW_O 0 //fila inicial de los circulos azules
#define I_ROW_X 4 //fila inicial de las cruces rojas

//enumeracion de posibles estados de una casilla
enum TipoPieza { PIEZA_O, PIEZA_X, NO_PIEZA, CASILLA_NO_VALIDA};

enum Ganador { PLAYER_O, PLAYER_X,NO_PLAYER,BOTH_PLAYERS};


//estructura de posiciones, definiendo fila y columna
struct Pos {
    int row,col;
};

//definicion de todas las casillas del tablero (tablero de 5x5) almacenando todo ello en la variable datos
struct Tablero {
    enum TipoPieza datos[5][5];
    enum Ganador columnas[3];
    enum Ganador filas[3];
    enum Ganador diagonales[2];
};

//declaracion como variable externa de la global _tablero
extern struct Tablero _tablero;

//funcion que inicializa la tabla
void InitTablero();

//funcion que muestra el estado actual del tablero
void ShowTablero();

//funcion como Show, pero que almacena en una cadena en lugar de imprimir en pantalla
void TableroAString(char dest[]);

//funcion que obtiene la posicion de la primera ficha disponible
struct Pos GetColPrimeraFichaDisponible(enum TipoPieza tipo);

//funcion que gestiona el movimiento de fichas
void MoverFicha(enum TipoPieza tipo, struct Pos destino);


int checkwinner();


//funcion que actualiza las variables globales que recogen que jugador puede ganar en cada una de las lineas
void _checkwinnable();

//funcion que determina el movimiento que debe realizar el robot
Pos ia(Ganador player);

//funcion que comprueba si es posible ganar en un solo turno, y en caso afirmativo devuelve la casilla en que se debe jugar para lograrlo
Pos win_now(Ganador player);

//selecciona una casilla que permita acercarse a la victoria, sin especial relevancia a cual sea esta
Pos pick_winable(Ganador player);

//selecciona una casilla vacia sin importar cual
Pos pick_nopieza();

#endif // _INC_TABLERO_H
