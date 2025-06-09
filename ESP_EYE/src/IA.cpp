#include "IA.h"
#include "Tablero.h"

Pos IA::ia(Ganador player, const Tablero& tablero) {
    Ganador adversary;
    if (player == PLAYER_O) {
        adversary = PLAYER_X;
    } else {
        adversary = PLAYER_O;
    }
    Pos jugada = win_now(player, tablero); // Comprobar si alguna línea permite ganar inmediatamente
    if (jugada.row == 0) { // Si no había forma de ganar en este turno, buscar otra jugada
        // Comprobar si alguna línea permite perder inmediatamente
        jugada = win_now(adversary, tablero); //Bloquear jugada del adversario
    }
    if (jugada.row == 0) { // Si aún no se ha decidido una jugada
        // Elegir una casilla al azar
        jugada = pick_winable(player, tablero); // Selecciona una casilla cualquiera que permita ganar

        if (jugada.row == 0) { // Si no existe casilla que permita ganar (partida ya empatada)
            jugada = pick_nopieza(tablero); // Elige una casilla vacía cualquiera
        }
    }
    if (jugada.row == 0) {
        // Error en teoría inalcanzable
        // El tablero está lleno, y por tanto no se puede jugar en ninguna casilla
    }

    return jugada; // Fin de la función
}

Pos IA::win_now(Ganador player, const Tablero& tablero) {
    Pos jugada = {0,0}; // Inicializar la jugada a una posición inalcanzable
    int i, n;
    for(i=0;i<3;i++) //recorre las 8 posibles lineas (3 filas, 3 columnas, 2 diagonales)
    {
        int counter=0;
        //filas
            jugada.row=i+1;
            if (tablero.estado().filas[i]==player) //si el unico que puede ganar en la linea es el player
            {
                for (n=1;n<4;n++) //recorrer todas las casillas de la linea
                {
                    if (tablero.casilla_vacia({jugada.row, n})) //si la casilla está vacía
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
            counter = 0;
            jugada.col=i+1;
            if (tablero.estado().columnas[i]==player) //si el unico que puede ganar en la linea es el player
            {
                for (n=1;n<4;n++) //recorrer todas las casillas de la linea
                {
                    if (tablero.casilla_vacia({n, jugada.col})) //si la casilla está vacía
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
                counter = 0;
                if(tablero.estado().diagonales[0]==player) //si el unico que puede ganar en la linea es el player
                {
                    for (n=1;n<4;n++) //recorrer todas las casillas de la linea
                    {
                        if (tablero.casilla_vacia({n, n})) //si la casilla está vacía
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
                counter = 0;
                if(tablero.estado().diagonales[1]==player) //si el unico que puede ganar en la linea es el player
                {
                    for (n=1;n<4;n++) //recorrer todas las casillas de la linea
                    {
                        if (tablero.casilla_vacia({n, 3-n})) //si la casilla está vacía
                        {
                            counter++; //incrementar el contador
                            jugada.row=n; //almacenar la casilla en memoria
                        }
                    }
                    //despues de comprobar la linea completa
                    if (counter==1) //si solo hay una casilla vacia, en una linea en la que es posible ganar, jugar en ella permite ganar
                    {
                        jugada.col=4-jugada.row;
                        return jugada;
                    }
                }
            }
        jugada= {0,0}; // Reiniciar jugada a una posición inalcanzable
    } //si no ha encontrado ninguna jugada ganadora, devuelve (0,0), posición no jugable, para indicarlo
    return jugada;
}

Pos IA::pick_winable(Ganador player, const Tablero& tablero) {
    
    Pos jugada;
    jugada.row=0;jugada.col=0; //inicializar la jugada a una posición inalcanzable
    //se llama a variables globales, que recogen los jugadores que pueden ganar cada una de las lineas

    int i,n;
    for(i=0;i<3;i++) //recorre las 8 posibles lineas (3 filas, 3 columnas, 2 diagonales)
    {
        //filas
        jugada.row=i+1;
        if (tablero.estado().filas[i]==player) //si el unico que puede ganar en la linea es el player
        {
            for (n=1;n<4;n++) //recorrer todas las casillas de la linea
            {
                if (tablero.casilla_vacia({jugada.row, n})) //si la casilla está vacía
                {
                    jugada.col=n; //almacenar la casilla en memoria
                    return jugada;
                }
            }
        }
        //columnas
        jugada.col=i+1;
        if (tablero.estado().columnas[i]==player) //si el unico que puede ganar en la linea es el player
        {
            for (n=1;n<4;n++) //recorrer todas las casillas de la linea
            {
                if (tablero.casilla_vacia({n, jugada.col})) //si la casilla está vacía
                {
                    jugada.row=n; //almacenar la casilla en memoria
                    return jugada;
                }
            }
        }
        //diagonales
        if (i==0) //solo en la primera iteración (da prioridad a las diagonales, estrategicamente mas utiles que otras lineas)
        {
            if(tablero.estado().diagonales[0]==player) //si el unico que puede ganar en la linea es el player
            {
                for (n=1;n<4;n++) //recorrer todas las casillas de la linea
                {
                    if (tablero.casilla_vacia({n, n})) //si la casilla está vacía
                    {
                        jugada.row=n; //almacenar la casilla en memoria
                        jugada.col=jugada.row;
                        return jugada;
                    }
                }
            }
            if(tablero.estado().diagonales[1]==player) //si el unico que puede ganar en la linea es el player
            {
                for (n=1;n<4;n++) //recorrer todas las casillas de la linea
                {
                    if (tablero.estado().datos[n][3-n]==NO_PIEZA) //si la casilla está vacía
                    {
                        jugada.row=n; //almacenar la casilla en memoria
                        jugada.col=3-jugada.row;
                        return jugada;
                    }
                }
            }
        }
        jugada =  {0,0}; // Reiniciar jugada a una posición inalcanzable
    } //si no ha encontrado ninguna jugada ganadora, devuelve (0,0), posición no jugable, para indicarlo
    return jugada;

}

Pos IA::pick_nopieza(const Tablero& tablero) {
     Pos jugada;
    jugada.row=0;jugada.col=0; //inicializar la jugada a una posición inalcanzable
    int i,n;
    for (i=1;i<4;i++) //recorre todas las filas
    {
        for(n=1;n<4;n++) //recorre todas las columnas de la fila actual
        {
            if(tablero.estado().datos[i][n]==NO_PIEZA) //si la casilla está vacia
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

Pos IA::ia_Nueva(Ganador player, const Tablero& tablero) {
    // Acceder al estado a través de tablero.estado()
    const Tablero::Estado& estado = tablero.estado();
    
    Ganador adversary;
    if (player == PLAYER_O) {
        adversary = PLAYER_X;
    } else {
        adversary = PLAYER_O;
    }
    
    // Llamar a funciones de IA pasando el tablero
    Pos jugada = win_now(player, tablero);
    
    if (jugada.row == 0) {
        jugada = win_now(adversary, tablero);
    }
    
    if (jugada.row == 0) {
        jugada = pick_winable(player, tablero);
        
        if (jugada.row == 0) {
            jugada = pick_nopieza(tablero);
        }
    }
    
    if (jugada.row == 0) {
        // Manejo de error
    }
    
    return jugada;
}
