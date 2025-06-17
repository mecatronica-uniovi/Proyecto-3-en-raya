#include <Myserver.h>
// Variables para el hilo de conexión WiFi
// Red WiFi (debes cambiar esto según tu configuración)
const char *ssid = "Interfaz";     // SSID de tu router TL-WR802N
const char *password = "10414538"; // Tu clave WiFi
IPAddress local_IP(192, 168, 0, 50);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // Opcional
IPAddress secondaryDNS(8, 8, 4, 4); // Opcional

static SemaphoreHandle_t mutex = NULL; // Variable que contendrá el mutex para proteger las medidas

void mutex_setup()
{                                    // Función que genera el mutex
    mutex = xSemaphoreCreateMutex(); // Creación del mutex
}

void mantener_wifi(void *parameter)
{ // Función que conecta WiFi y mantiene la conexión activa
    for (;;)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }

        Serial.println("[WIFI] Conectando de nuevo");
        WiFi.mode(WIFI_STA);
        if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
        {
            Serial.println("Error al configurar IP fija");
        }
        WiFi.begin(ssid, password);
        unsigned long tiempo_intento = millis(); // Empieza a contar el tiempo que lleva intentando la conexión

        // Debe seguir buscando mientras no se conecte o no se haya llegado al timeout
        while (WiFi.status() != WL_CONNECTED &&
               millis() - tiempo_intento < Timeout_Wifi)
        {
        }
        // En caso de conexión fallida o de llegar al timeout esperar un tiempo y reintentar
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("[WIFI] CONEXIÓN FALLIDA");
            vTaskDelay(Tiempo_recuperacion / portTICK_PERIOD_MS);
            continue;
        }

        _server.begin(); // Iniciación del servidor socket
        Serial.println("[WIFI] Conectada");
    }
}

void recibir_mensaje(void *parameter)
{
    for (;;)
    {
        CheckNewConn();
        int j = CheckReceive();
        if (j >= 0)
        {
            String txt;
            if (_conn[j].recv.startsWith("Interfaz-"))
            {
                int ind = _conn[j].recv.indexOf("\n") - 9;
                if (ind != -1)
                {
                    txt = _conn[j].recv.substring(9, ind); // ← CORRECTO
                    SendTo("Interfaz", (txt + "\n").c_str());
                }
                _conn[j].recv = "";
            }
            if (_conn[j].recv.startsWith("M5-"))
            {
                printf("Recibido mensaje de M5: %s\n", _conn[j].recv.c_str());
                int ind = _conn[j].recv.indexOf("\n") - 3;
                if (ind != -1)
                {
                    txt = _conn[j].recv.substring(3, ind); // ← CORRECTO
                    printf("1\n");
                    SendTo("M5", (txt + "\n").c_str());
                }
                _conn[j].recv = "";
            }
            if (_conn[j].recv.startsWith("Camara-"))
            {
                printf("Recibido mensaje de Camara: %s\n", _conn[j].recv.c_str());
                int ind = _conn[j].recv.indexOf("\n") - 7;
                if (ind != -1)
                {
                    txt = _conn[j].recv.substring(7, ind); // ← CORRECTO
                    printf("2\n");
                    SendTo("Camara", (txt + "\n").c_str());
                }
                _conn[j].recv = "";
            }
            if (_conn[j].recv.startsWith("Matlab-"))
            {
                printf("Recibido mensaje de Matlab: %s\n", _conn[j].recv.c_str());
                int ind = _conn[j].recv.indexOf("\n") - 7;
                if (ind != -1)
                {
                    txt = _conn[j].recv.substring(7, ind); // ← CORRECTO
                    printf("3\n");
                    SendTo("Matlab", (txt + "\n").c_str());
                }
                _conn[j].recv = "";
            }
            _conn[j].recv = "";
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}
