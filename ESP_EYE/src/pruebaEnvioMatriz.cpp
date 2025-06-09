#include <Arduino.h>
#include <WiFi.h>
#include "Tablero.h"
#include "IA.h"
#include "CoordenadasUtils.h"
#include <array>
const char *ssid = "OPPO Carla";
const char *password = "carlaflobeto";
const char *serverIP = "192.168.94.203"; // IP del PC con MATLAB
const int serverPort = 5000;             // Puerto donde escucha MATLAB

WiFiServer server(serverPort);

Tablero tablero; // Instancia del tablero

// void ActualizarTableroDesdeString(const String &estado)
// {
//     int index = 0;
//     for (int fila = 0; fila < 5; fila++)
//     {
//         for (int col = 0; col < 5; col++)
//         {
//             // Saltar saltos de línea
//             Serial.printf("Procesando fila %d, columna %d, índice %d\n", fila, col, index);
//             char valor = estado[index++];

//             Pos pos = {fila, col};

//             switch (valor)
//             {
//             case 'X':
//                 tablero.AsignarFicha(PIEZA_X, pos);
//                 Serial.printf("Ficha X movida a (%d, %d)\n", fila, col);
//                 break;
//             case 'O':
//                 tablero.AsignarFicha(PIEZA_O, pos);
//                 Serial.printf("Ficha O movida a (%d, %d)\n", fila, col);
//                 break;
//             case ' ':
//                 tablero.AsignarFicha(NO_PIEZA, pos);
//                 Serial.printf("Casilla vacía en (%d, %d)\n", fila, col);
//                 break;
//             }
//         }
//     }
//     // printf("Estado del tablero:\n%s\n", tablero.TableroAString());
//     tablero.ShowTablero(); // Mostrar el estado del tablero después de la actualización
// }

void setup()
{
    Serial.begin(115200);

    // Conexión WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConectado a WiFi");
    // Mostrar Dirección IP
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());

    // Crear servidor
    server.begin();
    Serial.printf("Servidor iniciado en puerto %d\n", serverPort);

    tablero.InitTablero(); // Inicializar el tablero
    Serial.println("Tablero inicializado: ");
    tablero.ShowTablero(); // Mostrar el estado inicial del tablero
}

void loop()
{
    WiFiClient client = server.available();
    if (client)
    {
        Serial.println("Cliente conectado");
        while (client.connected())
        {
            if (client.available())
            {
                String estadoTablero = client.readString(); // Se espera una matriz tipo string de 5x5
                Serial.println();                            // Imprimir el estado del tablero recibido
                //ActualizarTableroDesdeString(estadoTablero); // Actualizar el tablero con el estado recibido
                tablero.ActualizarTableroDesdeString(estadoTablero, tablero); // Actualizar el tablero con el estado recibido
                // Pos jugada = IA::ia(PLAYER_X, tablero);

                // tablero.MoverFicha(PIEZA_X, jugada); // Mover la ficha que le toca al robot

                // std::array<double,3> coords = CoordenadasUtils::ConvertirCoordenadas(jugada); // Convertir coordenadas a formato XYZ
                /*String respuesta =
                    String(coords[0]) + "," +
                    String(coords[1]) + "," +
                    String(coords[2]) + "|" +
                    String(coords[3]) + "," +
                    String(coords[4]) + "," +
                    String(coords[5]);*/
                // client.println(respuesta); // Enviar respuesta al cliente
                // Serial.println("Coordenadas enviadas: " + respuesta);
                client.flush(); // Limpiar el buffer del cliente

                // tablero.ShowTablero(); // Mostrar el estado del tablero después de la jugada
            }
        }
        client.stop();
        Serial.println("Cliente desconectado");
    }
}
