// En este fichero se va a integrar el uso del Anillo LED, la recepción de datos desde MATLAB y la captura de
// imágenes desde la cámara ESP-EYE.

#include <Arduino.h>
#include <WiFi.h>
#include "Camara.h"
#include <algorithm>
#include "AnilloLed.h"
#include "Tablero.h"
#include "IA.h"
#include "CoordenadasUtils.h"

#include <WiFiClient.h>
#include <MyServer.h>

// const char *ssid = "OPPO Carla";         // Nombre de la red WiFi
// const char *password = "carlaflobeto";   // Contraseña de la red WiFi
// const char *serverIP = "192.168.0.102"; // Dirección IP de MATLAB
// const int serverPort = 5000; // Puerto donde escuchará MATLAB
// const char *ssid = "Livebox6-4EAF";
// const char *password = "P92LZTQvFHcF";
// Objeto servidor WiFi
// WiFiServer server(serverPort);
AnilloLed anilloLed(LED_PIN, NUM_LEDS); // Instancia del anillo LED
// VARIABLES DE JUEGO
Tablero tablero;           // Instancia del tablero
TipoPieza turno = PIEZA_O; // Turno actual, empieza el jugador O
IA ia;                     // Instancia de la IA para el juego
int winner = -2;
// Conversión de TipoPieza a Ganador
Ganador tipoPiezaToGanador(TipoPieza pieza)
{
    switch (pieza)
    {
    case PIEZA_O:
        return PLAYER_O;
    case PIEZA_X:
        return PLAYER_X;
    default:
        return NO_PLAYER;
    }
}

// CAMARA
Camara camara; // Instancia de la cámara

void setup()
{
    Serial.begin(115200);
    mutex_setup(); // Se inicializa el mutex y se dejan 2 segundos para que todo se prepare
    cli.setTimeout(2000);
    delay(1000);
    xTaskCreate(mantener_wifi, "Mantiene la conexión WiFi activa", 20000, NULL, 5, NULL);
    xTaskCreate(recibir_mensaje, "Recibe e imprime por pantalla los datos enviados por el cliente", 20000, NULL, 5, NULL);

    // Configuración de la cámara
    camara.InitCamara();
    // // Conexión WiFi
    // WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     delay(1000);
    //     Serial.print(".");
    // }
    // // Mostrar Dirección IP
    // Serial.print("Dirección IP: ");
    // Serial.println(WiFi.localIP());
    // Configuración del anillo LED
    anilloLed.Init(); // Inicializar el anillo LED
    // server.begin();
    // Serial.printf("Servidor iniciado en puerto %d\n", serverPort);
    tablero.InitTablero(); // Inicializar el tablero
    Serial.println("Tablero inicializado: ");
    tablero.ShowTablero(); // Mostrar el estado inicial del tablero
}

void loop()
{
    // WiFiClient client = server.available(); // Verificar si hay un cliente disponible
    //  if (!client)
    //      return; // Si no hay cliente, salir del loop
    //  Serial.println("Cliente conectado!");
    //  while (client.connected())
    //  {
    //  if (client.available())
    //  {
    //  mensaje = client.readStringUntil('\n'); // Leer el mensaje del cliente
    //  Serial.println(mensaje);
    if (mensaje.startsWith("TURNO_IA"))
    {
        Serial.println("Recibido el mensaje TURNO_IA");
        turno = PIEZA_O; // Cambiar turno a IA
        // Realizar una captura para conocer el estado del tablero
        camara.fb = camara.get_latest_frame(); // Obtener el fotograma más reciente

        if (!camara.fb)
        {
            Serial.println("Error al capturar la imagen");
            // client.write("E", 1); // Enviar un error al cliente
            enviar("Interfaz-ErrorCaptura\n");
            // continue;
        }

        // Enviar tamaño de la imagen primero
        // uint32_t imgSize = camara.fb->len;
        // if (client.write((uint8_t *)&imgSize, sizeof(imgSize)) != sizeof(imgSize))
        // {
        //     Serial.println("Error al enviar tamaño de la imagen");
        //     esp_camera_fb_return(camara.fb); // Liberar memoria
        //     break;
        // }

        // Enviar la imagen completa a MATLAB para hacer el procesamiento
        // if (client.write(camara.fb->buf, camara.fb->len) != camara.fb->len)
        // {
        //     Serial.println("Error al enviar la imagen");
        // }
        // else
        // {
        //     Serial.println("Imagen enviada correctamente");
        //     camara.lastCapture = millis(); // Actualizar el tiempo de la última captura
        // }
        // esp_camera_fb_return(camara.fb); // Liberar memoria
        // camara.fb = NULL;
    }

    else if (mensaje.startsWith("TABLERO:"))
    {
        turno = PIEZA_O;                                       // Cambiar turno a IA
        String estado = mensaje.substring(8);                  // Eliminar la cabecera "TABLERO:"
        tablero.ActualizarTableroDesdeString(estado, tablero); // Actualizar el tablero con el estado recibido

        // Una vez actualizado el tablero, la IA juega
        // Comprobar si alguien ha ganado
        winner = tablero.checkwinner();
        switch (winner)
        {
        case PLAYER_O:
        {
            Serial.println("Jugador O ha ganado!");
            // client.println("O_WIN"); // Gana la IA
            enviar("Interfaz-O_WIN");
            enviar("M5-O_WIN");
            // Encender LEDs en rojo
            anilloLed.SetColor(CRGB::Red); // Encender LEDs en rojo
            break;
        }
        case PLAYER_X:
        {
            Serial.println("Jugador X ha ganado!");
            enviar("Interfaz-X_WIN");
            enviar("M5-X_WIN");
            // Encender LEDs en verde

            break;
        }
        case NO_PLAYER:
        {
            winner = tablero.checkempate(); // Comprobar si hay empate
            if (winner == -1)
            {
                Serial.println("Empate!");
                enviar("Interfaz-EMPATE");
                enviar("M5-EMPATE");
                // Encender LEDs en azul
                anilloLed.SetColor(CRGB::Blue); // Encender LEDs en azul
                return;                         // Salir del loop si hay empate
            }
            Serial.println("Nadie ha ganado aún");
            // En primer lugar, comprobar si se puede ganar
            tablero._checkwinnable();
            Pos jugada = ia.ia(tipoPiezaToGanador(turno), tablero); // IA juega con PIEZA_X
            tablero.MoverFicha(PIEZA_X, jugada);                    // Mover la ficha que le toca al robot
            Serial.println("IA ha jugado:");
            tablero.ShowTablero(); // Mostrar el estado del tablero después de la jugada
            // Convertir coordenadas a formato XYZ
            MovimientoCoords coords = tablero.MoverFichaDevuelveCoords(PIEZA_X, jugada);
            std::string respuesta = tablero.MovimientoCoordsToString(coords); // Convertir a string
            enviar(((respuesta + "\n").c_str()));                             // Enviar la jugada de la IA al cliente
            Serial.println(respuesta.c_str());
            Serial.println("Turno Jugador O:");
            break;
        }
        }
        turno = PIEZA_X; // Cambiar turno al jugador X
    }

    else if (mensaje.startsWith("RESET"))
    {
        tablero.InitTablero(); // Reiniciar el tablero
        tablero.ShowTablero(); // Mostrar el estado del tablero después del reinicio
        Serial.println("Tablero reiniciado");
        String respuesta = "Matlab-RESET_OK";
        enviar(respuesta);
        // client.println(respuesta); // Enviar respuesta al cliente
    }

    // winner = tablero.checkwinner(); // Comprobar si alguien ha ganado
    // if (winner != -2)
    // {
    //     // Si hay un ganador, encender LEDs en rojo o verde según el ganador
    //     if (winner == PLAYER_O)
    //     {
    //         // client.println("O_WIN"); // Gana la IA
    //         Serial.println("Jugador O ha ganado!");
    //         anilloLed.SetColor(CRGB::Red); // Encender LEDs en rojo
    //         winner = -2;                   // Reiniciar el ganador para permitir un nuevo juego
    //         tablero.InitTablero();         // Reiniciar el tablero
    //         tablero.ShowTablero();         // Mostrar el estado del tablero después del reinicio
    //         delay(1000);
    //         anilloLed.SetColor(CRGB::Moccasin); // Apagar los LEDs
    //     }
    //     else if (winner == PLAYER_X)
    //     {
    //         // client.println("X_WIN"); // Gana la IA
    //         Serial.println("Jugador X ha ganado!");
    //         anilloLed.SetColor(CRGB::Green); // Encender LEDs en verde
    //         winner = -2;                     // Reiniciar el ganador para permitir un nuevo juego
    //         tablero.InitTablero();           // Reiniciar el tablero
    //         tablero.ShowTablero();           // Mostrar el estado del tablero después del reinicio
    //         delay(1000);
    //         anilloLed.SetColor(CRGB::Moccasin); // Apagar los LEDs
    //     }
    //     else if (winner == NO_PLAYER)
    //     {
    //         winner = tablero.checkempate(); // Comprobar si hay empate
    //         if (winner == -1)
    //         {
    //             // client.println("EMPATE"); // Enviar mensaje de empate al cliente
    //             Serial.println("Empate!");
    //             anilloLed.SetColor(CRGB::Blue); // Encender LEDs en azul
    //             winner = -2;                    // Reiniciar el ganador para permitir un nuevo juego
    //             tablero.InitTablero();          // Reiniciar el tablero
    //             tablero.ShowTablero();          // Mostrar el estado del tablero después del reinicio
    //             delay(1000);
    //             anilloLed.SetColor(CRGB::Moccasin); // Apagar los LEDs
    //         }
    //     }
    //     FastLED.show(); // Mostrar los LEDs encendidos
    // }

    delay(1000);
}
