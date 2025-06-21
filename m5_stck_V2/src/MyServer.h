#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <vector>
#undef min
#include <iostream>
#include <vector>

struct Tablero
{
    std::vector<int> jugador_fila;
    std::vector<int> robot_fila;
    std::vector<int> jugador_col;
    std::vector<int> robot_col;
};

extern Tablero Tablero_act;

enum resultado
{
  sin_resultado,
  victoria,
  derrota,
  empate,
};

extern resultado resul;

extern int modo_func;
extern WiFiClient cli;
extern bool J_check;
const int casilla_vacia = 0;
const int casilla_robot = 1;
const int casilla_jugador = 2;
extern int turno_actual;
extern bool R_check;
extern bool new_tablero;
extern String board;

#define Timeout_Wifi 20000
#define Tiempo_recuperacion 30000

// Comprueba si algún cliente nuevo se ha intentado conectar. En caso afirmativo,
// envía el mensaje "<<NAME>>\n" para que se identifique, y devuelve true
bool CheckNewConn();

// Comprueba si algún cliente ha enviado un mensaje. En caso afirmativo, devuelve el
// índice del cliente en la tabla _conn, y se puede obtener el contenido en el campo
// recv de dicha tabla. Si el mensaje es de identificación, asocia el nombre al
// cliente. Si no hay mensaje nuevo, o el mensaje era de identificación, devuelve -1.
int CheckReceive();

// Envía mensaje a un cliente a partir de su índice en la tabla (ojo, el índice puede
// cambiar al eliminarse elementos de la tabla, usar solamente tras CheckReceive)
int SendTo(int j, const char* txt);

// Envía mensaje a un cliente a partir de su nombre identificativo
int SendTo(const char* name, const char* txt);

// Envía mensaje a todos los clientes cuyo nombre identificativo empiece por name
// (si name es "", envía a todos)
int SendToAllStartingWith(const char* name, const char* txt);

// Funciones para los hilos
void mutex_setup();
void mantener_wifi(void * parameter);
void recibir_mensaje(void * parameter);
void enviar(String texto_env);
void update_tablero();