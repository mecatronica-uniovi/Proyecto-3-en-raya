#include "Tablero.h"
#include "IA.h"
#include <iostream>
#include <cstring>

// Conversión de TipoPieza a Ganador
Ganador tipoPiezaToGanador(TipoPieza pieza) {
    switch (pieza) {
        case PIEZA_O: return PLAYER_O;
        case PIEZA_X: return PLAYER_X;
        default: return NO_PLAYER;
    }
}

int main() {
    char input[5];
    char *cmd;
    TipoPieza turno = PIEZA_O; // El humano juega O y va primero
    int winner = NO_PLAYER;
    Tablero juego;
    juego.InitTablero();
    juego.ShowTablero();
    Pos dest;
    IA robot;

    while (1) {
        if (turno == PIEZA_O) {
            juego._checkwinnable();
            std::cout << "\n Introduzca su movimiento, en formato Fila;Columna (Ej. 1;1 para esquina superior izquierda): ";
            std::cin.getline(input, 5);
            std::cout << "\n";
            if (strstr(input, "r") != NULL) {
                std::cout << "Reiniciando tablero\n";
                juego.InitTablero();
                juego.ShowTablero();
                turno = PIEZA_O; // Reinicia el turno como en el procedural
                continue;
            } else {
                cmd = input;
                dest.row = atoi(cmd);
                cmd = strstr(cmd, ";");
                dest.col = atoi(cmd + 1);
            }
            if (juego.casilla_vacia(dest) == 0) {
                std::cout << "Movimiento no valido, casilla ocupada o no valida\n";
                juego.ShowTablero();
                continue;
            }
        } else {
            juego._checkwinnable();
            dest = robot.ia(tipoPiezaToGanador(turno), juego); // IA espera Ganador
        }

        MovimientoCoords coords = juego.MoverFichaDevuelveCoords(turno, dest);
        std::cout << "El movimiento que ha realizado el " << (turno == PIEZA_O ? "jugador" : "robot") << " va desde ("
                  << coords.origen_xyz[0] << ", " << coords.origen_xyz[1] << ", " << coords.origen_xyz[2]
                  << ") a (" << coords.destino_xyz[0] << ", " << coords.destino_xyz[1] << ", " << coords.destino_xyz[2] << ")\n";

        juego.ShowTablero();

        // Cambia el turno
        switch (turno) {
            case PIEZA_O: turno = PIEZA_X; break;
            case PIEZA_X: turno = PIEZA_O; break;
            default: break;
        }

        winner = juego.checkwinner();
        switch (winner) {
            case PLAYER_O:
                std::cout << "Victoria jugador O\n";
                std::cout << "Reiniciando tablero\n";
                juego.InitTablero();
                juego.ShowTablero();
                turno = PIEZA_O;
                break;
            case PLAYER_X:
                std::cout << "Victoria jugador X\n";
                std::cout << "Reiniciando tablero\n";
                juego.InitTablero();
                juego.ShowTablero();
                turno = PIEZA_O;
                break;
            case NO_PLAYER:
                if (juego.checkempate()) {
                    std::cout << "Empate\n";
                    juego.InitTablero();
                    juego.ShowTablero();
                    turno = PIEZA_O;
                }
                break;
            default:
                break;
        }
    }
}
