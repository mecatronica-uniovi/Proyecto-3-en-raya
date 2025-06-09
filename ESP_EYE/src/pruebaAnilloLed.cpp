// #include <FastLED.h>

// // Configura aquí los parámetros de tu anillo
// #define LED_PIN     19         // GPIO del ESP-EYE conectado a DIN del anillo
// #define NUM_LEDS    12         // Número total de LEDs en el anillo
// #define BRIGHTNESS  255        // Brillo máximo (puedes reducir si no quieres tanto consumo)
// #define LED_TYPE    WS2812B
// #define COLOR_ORDER GRB        // La mayoría de WS2812B usan GRB

// CRGB leds[NUM_LEDS];

// void setup() {
//   FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
//   FastLED.setBrightness(BRIGHTNESS);
//   Serial.begin(115200);
//   delay(1000); // Espera para estabilizar la conexión serial
//   pinMode(LED_PIN, OUTPUT); // Configura el pin del LED como salida
//   for (int i = 0; i < NUM_LEDS; i++) {
//     leds[i] = CRGB::White;
//   }
//   FastLED.show();
  
//   Serial.println("Finalizado setup...");
// }

// void loop() {
//  // Enciende todos los LEDs en blanco
  
//   //delay(1000); // Espera 1 segundo para ver los LEDs encendidos
//   // Nada en el loop: los LEDs se quedan encendidos en blanco
// }
