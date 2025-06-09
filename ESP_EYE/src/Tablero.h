#ifndef TABLERO_H
#define TABLERO_H
#include <Arduino.h>
#include <array>
#include <string>
#include <sstream>
#include <vector>
// Definiciones constantes
constexpr int I_ROW_O = 0; // fila inicial de los circulos azules
constexpr int I_ROW_X = 4; // fila inicial de las cruces rojas

// Enumeraciones
enum TipoPieza { PIEZA_O, PIEZA_X, NO_PIEZA, CASILLA_NO_VALIDA };
enum Ganador { PLAYER_O, PLAYER_X, NO_PLAYER, BOTH_PLAYERS };

// Estructura Pos
struct Pos {
    int row, col;
};

// Estructura MovimientoCoords
struct MovimientoCoords {
    std::array<double, 3> origen_xyz;
    std::array<double, 3> destino_xyz;
};

class Tablero {
public:
    // Estado del tablero
    struct Estado {
        TipoPieza datos[5][5];
        Ganador columnas[3];
        Ganador filas[3];
        Ganador diagonales[2];
    };

    // Constructor e inicialización
    Tablero();
    
    // Métodos
    void InitTablero();
    void ShowTablero() const;
    std::string TableroAString() const;
    Pos GetColPrimeraFichaDisponible(TipoPieza tipo) const;
    void MoverFicha(TipoPieza tipo, Pos destino);
    int checkwinner() const;
    int checkempate() const;
    void _checkwinnable();
    bool casilla_vacia(Pos pos) const;
    MovimientoCoords MoverFichaDevuelveCoords(TipoPieza tipo, Pos destino);
    
    // Acceso al estado
    const Estado& estado() const { return _estado; }
    
    Estado _estado; // Estado del tablero

    void AsignarFicha(TipoPieza tipo, Pos destino); // Asigna una ficha a una posición específica en el tablero
    void ActualizarTableroDesdeString(const String &estado, Tablero &tablero); // Actualiza el tablero desde un string recibido
    std::string ArrayToString(const std::array<double, 3>& arr);
    std::array<double, 3> StringToArray(const std::string& s);
    std::string MovimientoCoordsToString(const MovimientoCoords& coords);
    MovimientoCoords StringToMovimientoCoords(const std::string& mensaje);
};

#endif // TABLERO_H