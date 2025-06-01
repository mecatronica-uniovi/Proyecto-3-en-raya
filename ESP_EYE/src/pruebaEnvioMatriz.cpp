#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "OPPO Carla";
const char* password = "carlaflobeto";
const char* serverIP = "192.168.94.203";  // IP del PC con MATLAB
const int serverPort = 5000;  // Puerto donde escucha MATLAB

WiFiServer server(serverPort);

void setup() {
    Serial.begin(115200);

    // Conexión WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
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
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        Serial.println("Cliente conectado");
        while (client.connected()) {
            if (client.available()) {
                String request = client.readStringUntil('\r');
                Serial.println("Petición recibida: ");
                Serial.print(request);
                client.flush();
            }
        }
        client.stop();
        Serial.println("Cliente desconectado");
    }
    else {
        //Serial.println("Esperando cliente...");
    }
}

