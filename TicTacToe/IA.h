#include "Tablero.h"

class IA {
public:
    // Métodos de IA (idénticos a tus funciones originales)
    static Pos ia(Ganador player, const Tablero& tablero);
    static Pos win_now(Ganador player, const Tablero& tablero);
    static Pos pick_winable(Ganador player, const Tablero& tablero);
    static Pos pick_nopieza(const Tablero& tablero);
    static Pos ia_Nueva(Ganador player, const Tablero& tablero);
};

