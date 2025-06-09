#include "CoordenadasUtils.h"

std::array<double, 3> CoordenadasUtils::ConvertirCoordenadas(Pos pos) {
    std::array<double, 3> pos_xyz;
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