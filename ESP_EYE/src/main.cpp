/* #include <Arduino.h>
#include <WiFi.h>

// Configuración WiFi
const char* ssid = "OPPO Carla";
const char* password = "carlaflobeto";

// Puerto para el servidor TCP
const uint16_t TCP_PORT = 8080;

// Crear instancia de servidor WiFi
WiFiServer tcpServer(TCP_PORT);
WiFiClient tcpClient;

void setup() {
  // Inicializar puerto serie
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nESP32 Socket Server para MATLAB");
  
  // Conectar a WiFi
  Serial.printf("Conectando a %s ", ssid);
  WiFi.begin(ssid, password);
  
  // Esperar a que se conecte WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Mostrar IP asignada
  Serial.println();
  Serial.print("Conectado! Dirección IP: ");
  Serial.println(WiFi.localIP());
  
  // Iniciar servidor TCP
  tcpServer.begin();
  Serial.printf("Servidor TCP iniciado en puerto %d\n", TCP_PORT);
  Serial.println("Esperando conexiones de MATLAB...");
}

void loop() {
  // Comprobar si hay clientes TCP nuevos
  if (tcpServer.hasClient()) {
    // Si ya existe un cliente conectado, desconectarlo
    if (tcpClient) {
      tcpClient.stop();
      Serial.println("Cliente previo desconectado");
    }
    
    // Conectar con el nuevo cliente
    tcpClient = tcpServer.available();
    Serial.println("Nuevo cliente conectado");
  }
  
  // Comprobar si hay datos disponibles del cliente
  if (tcpClient && tcpClient.connected()) {
    if (tcpClient.available()) {
      // Leer el mensaje enviado desde MATLAB
      String message = tcpClient.readStringUntil('\n');
      // Mostrar el mensaje recibido
      Serial.print("Mensaje recibido de MATLAB: ");
      Serial.println(message);
      
      // Enviar confirmación de vuelta a MATLAB
      tcpClient.println("ESP32 recibió: " + message);
    }
  }
  
  // Pequeña pausa para estabilidad
  delay(10);
} */


/* #include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("Iniciando ESP");
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  digitalWrite(21, LOW); // Led blanco
  digitalWrite(22, HIGH);
  delay(1000);
  Serial.println("Leds encendidos");
}

void loop()
{

} */

#include <Arduino.h>
#include <WiFi.h>
#include "esp_camera.h"

const char* ssid = "OPPO Carla";
const char* password = "carlaflobeto";
const char* serverIP = "192.168.201.203";  // IP de tu PC con MATLAB
const int serverPort = 5000;  // Puerto donde escuchará MATLAB

// Pines de cámara
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  4
#define SIOD_GPIO_NUM  18
#define SIOC_GPIO_NUM  23

#define Y9_GPIO_NUM    36
#define Y8_GPIO_NUM    37
#define Y7_GPIO_NUM    38
#define Y6_GPIO_NUM    39
#define Y5_GPIO_NUM    35
#define Y4_GPIO_NUM    14
#define Y3_GPIO_NUM    13
#define Y2_GPIO_NUM    34
#define VSYNC_GPIO_NUM 5
#define HREF_GPIO_NUM  27
#define PCLK_GPIO_NUM  25

#define LED_GPIO_NUM 22

WiFiServer server(serverPort);

void setup() {
    Serial.begin(115200);


    // Configuración de la cámara

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_UXGA;
    config.pixel_format = PIXFORMAT_JPEG;  // for streaming
    //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1;


    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Error al inicializar la cámara (0x%x)", err);
        return;
    }

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
    WiFiClient client = server.available();  // Espera conexión de MATLAB
    if (client) {
        Serial.println("Cliente conectado!");

        while (client.connected()) {
            if (client.available()) {
                char request = client.read();  // Espera una petición
                if (request == 'C') {  // MATLAB envía 'C' para capturar imagen
                    Serial.println("Capturando imagen...");
                    camera_fb_t *fb = esp_camera_fb_get();
                    if (!fb) {
                        Serial.println("Error al capturar la imagen");
                        continue;
                    }

                    // Enviar tamaño de la imagen primero
                    uint32_t imgSize = fb->len;
                    client.write((uint8_t*)&imgSize, sizeof(imgSize));

                    // Enviar la imagen en sí
                    client.write(fb->buf, fb->len);
                    Serial.println("Imagen enviada a MATLAB");

                    esp_camera_fb_return(fb);  // Liberar memoria
                }
            }
        }
        client.stop();
        Serial.println("Cliente desconectado");
    }
}