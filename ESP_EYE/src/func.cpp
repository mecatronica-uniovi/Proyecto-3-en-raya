#include <Myserver.h>
const uint16_t matlabport = 5000;
WiFiServer matlabServer(matlabport);
// Variables para el hilo de conexión WiFi
// Red WiFi (debes cambiar esto según tu configuración)
const char *ssid = "Interfaz";     // SSID de tu router TL-WR802N
const char *password = "10414538"; // Tu clave WiFi
String mensaje;
static SemaphoreHandle_t mutex = NULL; // Variable que contendrá el mutex para proteger las medidas

WiFiClient esp32s3_cli;
WiFiClient matlabClient;



IPAddress local_IP(192, 168, 0, 45);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // Opcional
IPAddress secondaryDNS(8, 8, 4, 4); // Opcional

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
        matlabServer.begin(); // Inicia el servidor de MATLAB
        Serial.println("[WIFI] Conectado");
    }
}

void recibir_mensaje(void *parameter)
{
    enum EstadoCliente
    {
        DESCONECTADO,
        CONECTANDO,
        ESPERANDO_RESPUESTA,
        CONECTADO
    };
    EstadoCliente estado = DESCONECTADO;
    unsigned long tiempoUltimoIntento = 0;

    for (;;)
    {
        unsigned long ahora = millis();

        switch (estado)
        {
        case DESCONECTADO:
            if (ahora - tiempoUltimoIntento > 2000)
            { // intenta cada 2s
                if (esp32s3_cli.connect("192.168.0.50", 55355))
                {
                    Serial.println("Conectado al servidor");
                    delay(3000);
                    String texto1 = "<<NAMECamara>>\n";
                    esp32s3_cli.write((const uint8_t *)texto1.c_str(), texto1.length());
                    estado = ESPERANDO_RESPUESTA;
                }
                else
                {
                    Serial.println("Fallo en conexión");
                }
                tiempoUltimoIntento = ahora;
            }
            break;

        case ESPERANDO_RESPUESTA:
            if (esp32s3_cli.available())
            {
                String recv = esp32s3_cli.readStringUntil('\n');
                Serial.print("RECV: ");
                Serial.println(recv);
                if (recv == "<<NAME>>")
                {
                    String texto = "<<NAMECamara>>\n";
                    esp32s3_cli.write((const uint8_t *)texto.c_str(), texto.length());
                    estado = CONECTADO;
                }
                Serial.print("Te saludo\n");
                enviar("Te saludo\n");
            }
            break;

        case CONECTADO:
            if (esp32s3_cli.available())
            {
                String recv = esp32s3_cli.readStringUntil('\n');
                Serial.print("MENSAJE: ");
                Serial.print(recv);

                if (recv == "<<NAME>>")
                {
                    Serial.println("Solicitud de nombre recibida estando ya conectado. Ignorando.");
                    // Opcionalmente podrías reenviar el nombre si quieres, pero no es obligatorio.
                }
                Serial.print("Estoy antes de verificar el mensaje");
                if (recv.startsWith("TURNO_IA"))
                {
                    Serial.println("Estoy despues de verificar el mensaje");
                    for (int i = 0; i < recv.length(); i++)
                    {
                        Serial.print(i);
                        Serial.print(": ");
                        Serial.println((int)recv[i]); // Imprime el código ASCII
                    }
                    Serial.println("Termine verificar mensaje, saliendo");
                    mensaje = recv;
                    // Serial.println(mensaje);
                }
                if (recv.startsWith("TABLERO:"))
                {
                    Serial.println("Recibido TABLERO");
                    mensaje = recv+"\n";
                }
                if (recv.startsWith("RESET"))
                {
                    mensaje = recv+"\n";
                }

                recv = "";
            }

            if (!esp32s3_cli.connected())
            {
                Serial.println("Se perdió conexión");
                estado = DESCONECTADO;
            }
            break;

        default:
            estado = DESCONECTADO;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void enviar(String texto_env)
{
    esp32s3_cli.write((const uint8_t *)texto_env.c_str(), texto_env.length());
}


void recibir_mensaje_matlab(void *parameter) {
    for (;;) {
        if (!matlabClient || !matlabClient.connected()) {
            matlabClient = matlabServer.available();
            if (matlabClient) {
                Serial.println("[MATLAB] Cliente conectado.");
            }
        }

        if (matlabClient && matlabClient.connected() && matlabClient.available()) {
            String msg = matlabClient.readStringUntil('\n');
            Serial.print("[MATLAB] Recibido: ");
            Serial.println(msg);

            if(msg.startsWith("Camara-TURNO_IA"))
            {
                mensaje = "TURNO_IA\n";
            }

            // Ejemplo: respuesta de prueba
            matlabClient.println("Recibido por la cámara: " + msg);
            msg = ""; // Limpiar el mensaje después de procesarlo
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}