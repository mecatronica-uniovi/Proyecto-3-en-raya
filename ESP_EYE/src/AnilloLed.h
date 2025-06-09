#ifndef ANILLOLED_H
#define ANILLOLED_H
#include <FastLED.h>

// PINES ANILLO LED
#define LED_PIN 19 // GPIO del ESP-EYE conectado a DIN del anillo
// PARÁMETROS ANILLO LED
#define NUM_LEDS 12    // Número total de LEDs en el anillo
#define BRIGHTNESS 255 // Brillo máximo
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB // La mayoría de WS2812B usan GRB

class AnilloLed {
public:
    CRGB leds[NUM_LEDS];
    AnilloLed(int pin, int numLeds); // Constructor que inicializa el anillo LED con el pin y número de LEDs
    void Init(); // Método para inicializar el anillo LED
    void SetColor(CRGB color);  // Método para establecer el color de un LED específico
};

#endif // ANILLOLED_H