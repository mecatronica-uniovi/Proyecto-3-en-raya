#include "Camara.h"
Camara::Camara()
{
    // Constructor vacío, la configuración se realiza en InitCamara
}

void Camara::InitCamara()
{
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
    config.pixel_format = PIXFORMAT_JPEG; // for streaming
    // config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 10; // calidad de la imagen (0-63, menor es mejor)
    config.fb_count = 1;      // número de buffers de fotogramas (1-2)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Error al inicializar la cámara (0x%x)", err);
        return;
    }
}

// Función para obtener siempre la imagen más reciente
camera_fb_t *Camara::get_latest_frame()
{
    Camara camera;
    camera.fb = NULL;

    // Si tenemos fb_count > 1, necesitamos vaciar la cola
    for (int i = 0; i < config.fb_count - 1; i++)
    {
        camera.fb = esp_camera_fb_get();
        if (camera.fb)
        {
            // Si no es el último frame, lo liberamos
            esp_camera_fb_return(camera.fb);
            camera.fb = NULL;
        }
    }

    // Obtenemos el último frame
    camera.fb = esp_camera_fb_get();

    // Verificamos la "edad" de la imagen usando el timestamp
    if (camera.fb)
    {
        // Comprobamos si la imagen es reciente usando el timestamp
        unsigned long currentTime = millis();
        unsigned long imageTime = camera.fb->timestamp.tv_sec * 1000 + camera.fb->timestamp.tv_usec / 1000;
        unsigned long imageAge = currentTime - imageTime;

        Serial.printf("Edad de la imagen: %lu ms\n", imageAge);

        // Si la imagen es demasiado antigua, capturamos una nueva
        if (imageAge > MAX_IMAGE_AGE)
        {
            Serial.println("Imagen demasiado antigua, capturando una nueva");
            esp_camera_fb_return(camera.fb);
            camera.fb = NULL;

            // Esperamos un poco para que la cámara se actualice
            delay(100);

            // Intentamos de nuevo
            return get_latest_frame();
        }
    }

    return camera.fb;
}
