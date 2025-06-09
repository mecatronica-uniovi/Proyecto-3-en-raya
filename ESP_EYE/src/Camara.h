#ifndef CAMARA_H
#define CAMARA_H
#include "esp_camera.h"
#include <Arduino.h>
// Pines de cámara
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 4
#define SIOD_GPIO_NUM 18
#define SIOC_GPIO_NUM 23
#define Y9_GPIO_NUM 36
#define Y8_GPIO_NUM 37
#define Y7_GPIO_NUM 38
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 35
#define Y4_GPIO_NUM 14
#define Y3_GPIO_NUM 13
#define Y2_GPIO_NUM 34
#define VSYNC_GPIO_NUM 5
#define HREF_GPIO_NUM 27
#define PCLK_GPIO_NUM 25

#define LED_GPIO_NUM 22

// Variable para almacenar el tiempo de la última captura

const unsigned long MAX_IMAGE_AGE = 1000; // Tiempo máximo en milisegundos para considerar una imagen "reciente"

class Camara
{
public:
    unsigned long lastCapture = 0;
    camera_fb_t *fb = NULL; // Variable para almacenar el fotograma capturado
    camera_config_t config; // Configuración de la cámara
    // Métodos
    Camara();
    void InitCamara();
    camera_fb_t *get_latest_frame();
};

#endif // CAMARA_H