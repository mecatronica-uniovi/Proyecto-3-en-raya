#include "Tablero.h"
#include "IA.h"
#include "CoordenadasUtils.h"
#include <cstdio>
#include <sstream>


Tablero::Tablero() {
    InitTablero();
}

void Tablero::InitTablero() {
    int row, col;
    for (col = 0; col < 5; col++) {
        _estado.datos[I_ROW_O][col] = PIEZA_O;
        _estado.datos[I_ROW_X][col] = PIEZA_X;
    }
    for (col = 0; col < 5; col++) {
        for (row = 1; row <= 3; row++) {
            _estado.datos[row][col] = NO_PIEZA;
        }
    }
}

void Tablero::ShowTablero() const {
    int row, col;
    for (row = 0; row < 5; row++) {
        for (col = 0; col < 5; col++) {
            switch (_estado.datos[row][col]) {
                case NO_PIEZA:
                    Serial.print(" ");
                    break;
                case PIEZA_O:
                    Serial.print("O");
                    break;
                case PIEZA_X:
                    Serial.print("X");
                    break;
                case CASILLA_NO_VALIDA:
                    Serial.print(" ");
                    break;
            }
            if (col < 4 && row > 0 && row < 4) {
                Serial.print("|");
            } else {
                Serial.print(" ");
            }
        }
        Serial.print("\n");
        if (row < 4) {
            Serial.print("  - - -  \n");
        }
    }
    Serial.print("\n");
}

std::string Tablero::TableroAString() const {
    std::string result;
    char buffer[100] = {0}; // Buffer suficiente para el tablero
    int posString = 0;
    
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            switch (_estado.datos[row][col]) {
                // ... (código idéntico al original)
            }
        }
        posString += sprintf(buffer + posString, "\n");
    }
    
    result = buffer;
    return result;
}

Pos Tablero::GetColPrimeraFichaDisponible(TipoPieza tipo) const {
    int noficha;
    struct Pos pos;
    pos.row = -1;
    pos.col = -1;
    switch (tipo)
    {
    case NO_PIEZA:
        return pos;
    case PIEZA_O:
    {
        pos.row = I_ROW_O;
        break;
    }
    case PIEZA_X:
    {
        pos.row = I_ROW_X;
        break;
    }
    case CASILLA_NO_VALIDA:
        return pos;
    }
    for (noficha = 1, pos.col = 0; (pos.col < 6) && noficha; pos.col++)
    {
        if (_estado.datos[pos.row][pos.col] == tipo)
        {
            noficha = 0;
        }
    }
    if (noficha == 0)
    {
        pos.col--;
    }
    if (pos.col > 5)
    {
        pos.col = -1;
    }
    return pos;
}

void Tablero::MoverFicha(TipoPieza tipo, Pos destino) {
    struct Pos origen;
    origen = GetColPrimeraFichaDisponible(tipo);
    if (origen.row >= 0 && origen.col >= 0)
    {
        if (_estado.datos[destino.row][destino.col] == NO_PIEZA)
        {
            _estado.datos[origen.row][origen.col] = NO_PIEZA;
            _estado.datos[destino.row][destino.col] = tipo;
        }
    }
}

int Tablero::checkwinner() const {
    int i, n;

    // Inicialización de arrays
    int filas[3], columnas[3];
    for (i = 0; i < 3; i++) {  // Usar la i ya declarada
        filas[i] = _estado.datos[i+1][1];
        columnas[i] = _estado.datos[1][i+1];
    }

    int diagonales[2];
    diagonales[0] = _estado.datos[1][1];
    diagonales[1] = _estado.datos[3][1];

    // Verificación de líneas
    for (n = 0; n < 3; n++) {
        for (i = 1; i < 4; i++) {
            if (filas[n] != _estado.datos[n+1][i]) {
                filas[n] = NO_PIEZA;
            }
            if (columnas[n] != _estado.datos[i][n+1]) {
                columnas[n] = NO_PIEZA;
            }
        }

        if (diagonales[0] != _estado.datos[n+1][n+1]) {
            diagonales[0] = NO_PIEZA;
        }
        if (diagonales[1] != _estado.datos[3-n][n+1]) {
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

int Tablero::checkempate() const {
    int i, j;
    for (i = 1; i < 4; i++) {
        for (j = 1; j < 4; j++) {
            if (_estado.datos[i][j] == NO_PIEZA) {
                return 0; // No hay empate, hay casillas vacías
            }
        }
    }
    return -1; // Tablero lleno, es un empate
}

void Tablero::_checkwinnable() {
    for (int n = 0; n < 3; n++) {
        for (int i = 1; i < 4; i++) {
            // Filas
            if (_estado.filas[n] == BOTH_PLAYERS) {
                if (_estado.datos[n+1][i] == PIEZA_O) _estado.filas[n] = PLAYER_O;
                else if (_estado.datos[n+1][i] == PIEZA_X) _estado.filas[n] = PLAYER_X;
            } else if (_estado.filas[n] == PLAYER_O && _estado.datos[n+1][i] == PIEZA_X) {
                _estado.filas[n] = NO_PLAYER;
            } else if (_estado.filas[n] == PLAYER_X && _estado.datos[n+1][i] == PIEZA_O) {
                _estado.filas[n] = NO_PLAYER;
            }

            // Columnas
            if (_estado.columnas[n] == BOTH_PLAYERS) {
                if (_estado.datos[i][n+1] == PIEZA_O) _estado.columnas[n] = PLAYER_O;
                else if (_estado.datos[i][n+1] == PIEZA_X) _estado.columnas[n] = PLAYER_X;
            } else if (_estado.columnas[n] == PLAYER_O && _estado.datos[i][n+1] == PIEZA_X) {
                _estado.columnas[n] = NO_PLAYER;
            } else if (_estado.columnas[n] == PLAYER_X && _estado.datos[i][n+1] == PIEZA_O) {
                _estado.columnas[n] = NO_PLAYER;
            }

            // Diagonales
            if (n == 0) {
                if (_estado.diagonales[0] == BOTH_PLAYERS) {
                    if (_estado.datos[i][i] == PIEZA_O) _estado.diagonales[0] = PLAYER_O;
                    else if (_estado.datos[i][i] == PIEZA_X) _estado.diagonales[0] = PLAYER_X;
                } else if (_estado.diagonales[0] == PLAYER_O && _estado.datos[i][i] == PIEZA_X) {
                    _estado.diagonales[0] = NO_PLAYER;
                } else if (_estado.diagonales[0] == PLAYER_X && _estado.datos[i][i] == PIEZA_O) {
                    _estado.diagonales[0] = NO_PLAYER;
                }
            } else if (n == 1) {
                if (_estado.diagonales[1] == BOTH_PLAYERS) {
                    if (_estado.datos[4-i][i] == PIEZA_O) _estado.diagonales[1] = PLAYER_O;
                    else if (_estado.datos[4-i][i] == PIEZA_X) _estado.diagonales[1] = PLAYER_X;
                } else if (_estado.diagonales[1] == PLAYER_O && _estado.datos[4-i][i] == PIEZA_X) {
                    _estado.diagonales[1] = NO_PLAYER;
                } else if (_estado.diagonales[1] == PLAYER_X && _estado.datos[4-i][i] == PIEZA_O) {
                    _estado.diagonales[1] = NO_PLAYER;
                }
            }
        }
    }
}

bool Tablero::casilla_vacia(Pos pos) const {
    // Devuelve 1 si la casilla está vacía (NO_PIEZA), 0 en caso contrario o fuera de rango
    if (pos.row < 1 || pos.row > 3 || pos.col < 1 || pos.col > 3) {
        return 0; // Casilla fuera de rango
    }
    return _estado.datos[pos.row][pos.col] == NO_PIEZA;
}

MovimientoCoords Tablero::MoverFichaDevuelveCoords(TipoPieza tipo, Pos destino)
{ 
    struct Pos origen;
    MovimientoCoords coords;
    origen=GetColPrimeraFichaDisponible(tipo);
    if (origen.row >= 0 && origen.col >= 0) {
        if (_estado.datos[destino.row][destino.col] == NO_PIEZA) {
            _estado.datos[origen.row][origen.col] = NO_PIEZA;
            _estado.datos[destino.row][destino.col] = tipo;
        }
    }
    coords.origen_xyz = CoordenadasUtils::ConvertirCoordenadas(origen);
    coords.destino_xyz = CoordenadasUtils::ConvertirCoordenadas(destino);
    return coords; // Devolver las coordenadas de destino
}

void Tablero::AsignarFicha(TipoPieza tipo, Pos destino) {
    _estado.datos[destino.row][destino.col] = tipo;
}

void Tablero::ActualizarTableroDesdeString(const String &estado, Tablero &tablero)
{
    tablero.InitTablero(); // Reiniciar el tablero antes de actualizar
    int index = 0;
    for (int fila = 0; fila < 5; fila++)
    {
        for (int col = 0; col < 5; col++)
        {
            char valor = estado[index++];

            Pos pos = {fila, col};

            switch (valor)
            {
            case 'X':
                tablero.AsignarFicha(PIEZA_X, pos);
                break;
            case 'O':
                tablero.AsignarFicha(PIEZA_O, pos);
                break;
            case ' ':
                tablero.AsignarFicha(NO_PIEZA, pos);
                break;
            }
        }
    }
    tablero.ShowTablero();
}