#ifndef TABLERO_H
#define TABLERO_H
#include <Arduino.h>
#include <array>
#include <string>

// Definiciones constantes
constexpr int I_ROW_O = 0; // fila inicial de los circulos azules
constexpr int I_ROW_X = 4; // fila inicial de las cruces rojas

// Enumeraciones (sin cambios)
enum TipoPieza { PIEZA_O, PIEZA_X, NO_PIEZA, CASILLA_NO_VALIDA };
enum Ganador { PLAYER_O, PLAYER_X, NO_PLAYER, BOTH_PLAYERS };

// Estructura Pos (sin cambios)
struct Pos {
    int row, col;
};

// Estructura MovimientoCoords (sin cambios)
struct MovimientoCoords {
    std::array<double, 3> origen_xyz;
    std::array<double, 3> destino_xyz;
};

class Tablero {
public:
    // Estado del tablero (idéntico a tu struct original)
    struct Estado {
        TipoPieza datos[5][5];
        Ganador columnas[3];
        Ganador filas[3];
        Ganador diagonales[2];
    };

    // Constructor e inicialización
    Tablero();
    
    // Métodos públicos (equivalente a tus funciones originales)
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
    
    Estado _estado;  // Reemplaza la variable global _tablero

    void AsignarFicha(TipoPieza tipo, Pos destino); // Asigna una ficha a una posición específica en el tablero
    void ActualizarTableroDesdeString(const String &estado, Tablero &tablero); // Actualiza el tablero desde un string recibido
};

#endif // TABLERO_H