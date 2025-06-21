#include <Myserver.h>
// Variables para el hilo de conexión WiFi
// Red WiFi (debes cambiar esto según tu configuración)
const char* ssid = "Interfaz";           // SSID de tu router TL-WR802N
const char* password = "10414538";     // Tu clave WiFi

static SemaphoreHandle_t mutex = NULL;  //Variable que contendrá el mutex para proteger las medidas

WiFiClient cli;
int modo_func = 0;
bool inicio_luces=0;
bool reinicio=0;
bool turno_j=0;
bool turno_ia=0;

void mutex_setup(){ //Función que genera el mutex
    mutex = xSemaphoreCreateMutex(); //Creación del mutex
}

void mantener_wifi(void * parameter){ //Función que conecta WiFi y mantiene la conexión activa
    for(;;){
        if(WiFi.status() == WL_CONNECTED){
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }

        Serial.println("[WIFI] Conectando de nuevo");
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        unsigned long tiempo_intento = millis(); //Empieza a contar el tiempo que lleva intentando la conexión

        // Debe seguir buscando mientras no se conecte o no se haya llegado al timeout
        while (WiFi.status() != WL_CONNECTED && 
                millis() - tiempo_intento < Timeout_Wifi){}
        // En caso de conexión fallida o de llegar al timeout esperar un tiempo y reintentar
        if(WiFi.status() != WL_CONNECTED){
            Serial.println("[WIFI] CONEXIÓN FALLIDA");
            vTaskDelay(Tiempo_recuperacion / portTICK_PERIOD_MS);
			  continue;
        }
        Serial.println("[WIFI] Conectado");
    }
}

void recibir_mensaje(void * parameter){
    enum EstadoCliente { DESCONECTADO, CONECTANDO, ESPERANDO_RESPUESTA, CONECTADO };
    EstadoCliente estado = DESCONECTADO;
    unsigned long tiempoUltimoIntento = 0;

    for(;;){
        unsigned long ahora = millis();

        switch (estado) {
            case DESCONECTADO:
                if (ahora - tiempoUltimoIntento > 2000) {  // intenta cada 2s
                    if (cli.connect("192.168.0.50", 55355)) {
                        Serial.println("Conectado al servidor");
                        delay(3000);
                        String texto1 = "<<NAMEINTERFAZ>>\n";
                        cli.write((const uint8_t*)texto1.c_str(), texto1.length());
                        estado = ESPERANDO_RESPUESTA;
                    } else {
                        Serial.println("Fallo en conexión");
                    }
                tiempoUltimoIntento = ahora;
                }
            break;

            case ESPERANDO_RESPUESTA:
                if (cli.available()) {
                    String recv = cli.readStringUntil('\n');
                    Serial.print("RECV: ");
                    Serial.println(recv);
                    if (recv == "<<NAME>>") {
                        String texto = "<<NAMEINTERFAZ>>\n";
                        cli.write((const uint8_t*)texto.c_str(), texto.length());
                        estado = CONECTADO;
                    }
                    Serial.print("Te saludo\n");
                    enviar("Te saludo\n");
                }
            break;

            case CONECTADO:
                if (cli.available()) {
                    String recv = cli.readStringUntil('\n');
                    Serial.print("MENSAJE: ");
                    Serial.println(recv);

                    if (recv == "<<NAME>>") {
                        Serial.println("Solicitud de nombre recibida estando ya conectado. Ignorando.");
                         // Opcionalmente podrías reenviar el nombre si quieres, pero no es obligatorio.
                    }

                    if(recv=="INICIO")
                    {
                        inicio_luces = 1;
                    }
                    else if(recv=="REINICIO")
                    {
                        reinicio = 1;
                    }
                    else if(recv=="TURNO_J")
                    {
                        turno_j = 1;
                    }
                    else if(recv=="TURNO_IA")
                    {
                        turno_ia = 1;
                    }
                }


                if (!cli.connected()) {
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

void enviar(String texto_env){
    cli.write((const uint8_t*)texto_env.c_str(), texto_env.length());
}

