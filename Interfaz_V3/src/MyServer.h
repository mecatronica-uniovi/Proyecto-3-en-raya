#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <vector>

extern int modo_func;
extern bool inicio_luces;
extern bool reinicio;
extern bool turno_j;
extern bool turno_ia;
extern WiFiClient cli;

#define Timeout_Wifi 20000 // Timeout en ms. Por defecto 20 segundos.
#define Tiempo_recuperacion 30000 // Tiempo de espera en ms para intentar recuperar la conexión. Por defecto 30 segundos.

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