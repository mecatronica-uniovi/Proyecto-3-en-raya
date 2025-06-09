#include "AnilloLed.h"

AnilloLed::AnilloLed(int pin, int numLeds)
{
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

void AnilloLed::Init()
{
    // Encender todos los LEDs en blanco
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Moccasin;
    }
    FastLED.show(); // Mostrar los LEDs encendidos
}

void AnilloLed::SetColor(CRGB color)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = color;
    }
    FastLED.show(); // Mostrar los LEDs encendidos
}
