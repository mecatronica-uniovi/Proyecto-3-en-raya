#include <Myserver.h>
// Variables para el hilo de conexión WiFi
// Red WiFi 
const char* ssid = "Interfaz";           
const char* password = "10414538";     

static SemaphoreHandle_t mutex = NULL;  //Variable que contendrá el mutex (No utilizado)

WiFiClient cli;
int modo_func = 0;
bool J_check =0;
bool R_check = 0;
String board;
bool new_tablero = 0;

int turno_actual = casilla_jugador;
Tablero Tablero_act;

//Funcion para actualizar el tablero cuando llegue uno nuevo
void update_tablero (){
    int index = 0;
    for (int fila = 4; fila >-1; fila--)
    {
        for (int col = 4; col > -1; col--)
        {
            char valor = board[index++];
            if (col == 1 || col == 2 || col == 3)
            {
                if (fila == 1 || fila == 2 || fila == 3)
                {
                    switch (valor)
                    {
                    case 'X':
                        Tablero_act.jugador_col.push_back(col-1);
                        Serial.printf("Jugador col: %d", col);
                        Tablero_act.jugador_fila.push_back(fila-1);
                        Serial.printf("Jugador fila: %d", fila);
                        break;
                    case 'O':
                        Tablero_act.robot_col.push_back(col-1);
                        Serial.printf("Robot col: %d", col);
                        Tablero_act.robot_fila.push_back(fila-1);
                        Serial.printf("Robot fila: %d", fila);
                        break;
                    case ' ':
                        break;
                    }
                }
            }
        }
    }
}


//Funcion para el mutex (No utilizada)
void mutex_setup(){ 
    mutex = xSemaphoreCreateMutex(); 
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

//Funcion que comprueba si llega algun mensaje y lo interpreta. En un primer momento establece la conexion con el servidor
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
                        String texto1 = "<<NAMEM5>>\n";
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
                        String texto = "<<NAMEM5>>\n";
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
                    }

                    if (recv=="AUTO"){
                        modo_func = 2;
                    }
                    if (recv=="MANUAL")
                    {
                        modo_func = 1;
                    }
                    if (recv=="MAIN")
                    {
                        modo_func = 0;
                    }
                    if (recv=="PULSADOR" && turno_actual==casilla_jugador)
                    {
                        J_check=1;
                    }
                    if(recv == "TURNO_J" && turno_actual == casilla_robot){
                        R_check=1;
                    }
                    if (recv.startsWith("TABLERO:")){
                        recv = recv.substring(8);
                        board = recv;
                        Serial.print(board);
                        new_tablero = 1;
                    }
                    if (recv == "VICTORIA_IA")
                    {
                        resul = derrota;
                    }
                    if (recv == "VICTORIA_J")
                    {
                        resul = victoria;
                    }
                    if (recv == "EMPATE")
                    {
                        resul = empate;
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

//Funcion para enviar un mensaje a otro cliente
void enviar(String texto_env){
    cli.write((const uint8_t*)texto_env.c_str(), texto_env.length());
}

