// /*



// CÓDIGO QUE REALIZA UNA CAPTURA DE IMAGEN CUANDO SE RECIBE EL CARÁCTER 'C' POR WIFI Y ENVÍA LA IMAGEN A MATLAB

// INCLUYENDO EL ANILLO LED

// */

// #include <Arduino.h>
// #include <WiFi.h>
// #include "esp_camera.h"
// #include <algorithm>
// #include <FastLED.h>

// const char *ssid = "OPPO Carla";         // Nombre de la red WiFi
// const char *password = "carlaflobeto";   // Contraseña de la red WiFi
// const char *serverIP = "192.168.94.203"; // Dirección IP de MATLAB
// const int serverPort = 5000;             // Puerto donde escuchará MATLAB

// // Pines de cámara
// #define PWDN_GPIO_NUM -1
// #define RESET_GPIO_NUM -1
// #define XCLK_GPIO_NUM 4
// #define SIOD_GPIO_NUM 18
// #define SIOC_GPIO_NUM 23
// #define Y9_GPIO_NUM 36
// #define Y8_GPIO_NUM 37
// #define Y7_GPIO_NUM 38
// #define Y6_GPIO_NUM 39
// #define Y5_GPIO_NUM 35
// #define Y4_GPIO_NUM 14
// #define Y3_GPIO_NUM 13
// #define Y2_GPIO_NUM 34
// #define VSYNC_GPIO_NUM 5
// #define HREF_GPIO_NUM 27
// #define PCLK_GPIO_NUM 25

// #define LED_GPIO_NUM 22

// // PINES ANILLO LED
// #define LED_PIN 19 // GPIO del ESP-EYE conectado a DIN del anillo

// // PARÁMETROS ANILLO LED

// #define NUM_LEDS 12    // Número total de LEDs en el anillo
// #define BRIGHTNESS 255 // Brillo máximo
// #define LED_TYPE WS2812B
// #define COLOR_ORDER GRB // La mayoría de WS2812B usan GRB

// // Objeto de anillo LED
// CRGB leds[NUM_LEDS];

// // Objeto servidor WiFi
// WiFiServer server(serverPort);
// // Variable para almacenar el tiempo de la última captura
// unsigned long lastCapture = 0;
// const unsigned long MAX_IMAGE_AGE = 1000; // Tiempo máximo en milisegundos para considerar una imagen "reciente"

// camera_fb_t *fb = NULL; // Variable para almacenar el fotograma capturado
// camera_config_t config; // Configuración de la cámara

// void setup()
// {
//     Serial.begin(115200);
//     // Configuración de la cámara
//     config.ledc_channel = LEDC_CHANNEL_0;
//     config.ledc_timer = LEDC_TIMER_0;
//     config.pin_d0 = Y2_GPIO_NUM;
//     config.pin_d1 = Y3_GPIO_NUM;
//     config.pin_d2 = Y4_GPIO_NUM;
//     config.pin_d3 = Y5_GPIO_NUM;
//     config.pin_d4 = Y6_GPIO_NUM;
//     config.pin_d5 = Y7_GPIO_NUM;
//     config.pin_d6 = Y8_GPIO_NUM;
//     config.pin_d7 = Y9_GPIO_NUM;
//     config.pin_xclk = XCLK_GPIO_NUM;
//     config.pin_pclk = PCLK_GPIO_NUM;
//     config.pin_vsync = VSYNC_GPIO_NUM;
//     config.pin_href = HREF_GPIO_NUM;
//     config.pin_sccb_sda = SIOD_GPIO_NUM;
//     config.pin_sccb_scl = SIOC_GPIO_NUM;
//     config.pin_pwdn = PWDN_GPIO_NUM;
//     config.pin_reset = RESET_GPIO_NUM;
//     config.xclk_freq_hz = 20000000;
//     config.frame_size = FRAMESIZE_UXGA;
//     config.pixel_format = PIXFORMAT_JPEG; // for streaming
//     // config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
//     config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
//     config.fb_location = CAMERA_FB_IN_PSRAM;
//     config.jpeg_quality = 10; // calidad de la imagen (0-63, menor es mejor)
//     config.fb_count = 1;      // número de buffers de fotogramas (1-2)
//     pinMode(13, INPUT_PULLUP);
//     pinMode(14, INPUT_PULLUP);
//     esp_err_t err = esp_camera_init(&config);
//     if (err != ESP_OK)
//     {
//         Serial.printf("Error al inicializar la cámara (0x%x)", err);
//         return;
//     }
//     // Conexión WiFi
//     WiFi.begin(ssid, password);
//     while (WiFi.status() != WL_CONNECTED)
//     {
//         delay(1000);
//         Serial.print(".");
//     }
//     Serial.println("\nConectado a WiFi");
//     // Mostrar Dirección IP
//     Serial.print("Dirección IP: ");
//     Serial.println(WiFi.localIP());
//     // Crear servidor
//     // Configuración del anillo LED
//     FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
//     FastLED.setBrightness(BRIGHTNESS);
//     // Encender todos los LEDs en blanco
//     for (int i = 0; i < NUM_LEDS; i++)
//     {
//         leds[i] = CRGB::Moccasin;
//     }
//     FastLED.show(); // Mostrar los LEDs encendidos
//     server.begin();
//     Serial.printf("Servidor iniciado en puerto %d\n", serverPort);
// }

// // Función para obtener siempre la imagen más reciente
// camera_fb_t *get_latest_frame()
// {
//     camera_fb_t *fb = NULL;

//     // Si tenemos fb_count > 1, necesitamos vaciar la cola
//     for (int i = 0; i < config.fb_count - 1; i++)
//     {
//         fb = esp_camera_fb_get();
//         if (fb)
//         {
//             // Si no es el último frame, lo liberamos
//             esp_camera_fb_return(fb);
//             fb = NULL;
//         }
//     }

//     // Obtenemos el último frame
//     fb = esp_camera_fb_get();

//     // Verificamos la "edad" de la imagen usando el timestamp
//     if (fb)
//     {
//         // Comprobamos si la imagen es reciente usando el timestamp
//         unsigned long currentTime = millis();
//         unsigned long imageTime = fb->timestamp.tv_sec * 1000 + fb->timestamp.tv_usec / 1000;
//         unsigned long imageAge = currentTime - imageTime;

//         Serial.printf("Edad de la imagen: %lu ms\n", imageAge);

//         // Si la imagen es demasiado antigua, capturamos una nueva
//         if (imageAge > MAX_IMAGE_AGE)
//         {
//             Serial.println("Imagen demasiado antigua, capturando una nueva");
//             esp_camera_fb_return(fb);

//             // Esperamos un poco para que la cámara se actualice
//             delay(100);

//             // Intentamos de nuevo
//             return get_latest_frame();
//         }
//     }

//     return fb;
// }

// void loop()
// {
//     WiFiClient client = server.available(); // Verificar si hay un cliente disponible
//     if (!client)
//         return; // Si no hay cliente, salir del loop
//     Serial.println("Cliente conectado!");
//     while (client.connected())
//     {
//         if (client.available())
//         {
//             char request = client.read(); // Leer la petición del cliente
//             // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//             if (request == 'C') //Aquí sólo habría que cambiar el mensaje 'C' por el que envíe interfaz !!!!!!!!!!!
//             {                                         // Si la petición es 'C', capturar imagen
//                 camera_fb_t *fb = get_latest_frame(); // Obtener el fotograma más reciente

//                 if (!fb)
//                 {
//                     Serial.println("Error al capturar la imagen");
//                     client.write("E", 1); // Enviar un error al cliente
//                     continue;
//                 }

//                 // Enviar tamaño de la imagen primero
//                 uint32_t imgSize = fb->len;
//                 if (client.write((uint8_t *)&imgSize, sizeof(imgSize)) != sizeof(imgSize))
//                 {
//                     Serial.println("Error al enviar tamaño de la imagen");
//                     esp_camera_fb_return(fb); // Liberar memoria
//                     break;
//                 }

//                 // Enviar la imagen completa
//                 if (client.write(fb->buf, fb->len) != fb->len)
//                 {
//                     Serial.println("Error al enviar la imagen");
//                 }
//                 else
//                 {
//                     Serial.println("Imagen enviada correctamente");
//                     lastCapture = millis(); // Actualizar el tiempo de la última captura
//                 }

//                 esp_camera_fb_return(fb); // Liberar memoria
//                 fb = NULL;
//             }
//         }
//     }
// }