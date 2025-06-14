#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <SPI.h>

class Encoder {
public:
    Encoder(int csPin);
    void begin();
    float leerGrados();
    int id();

    bool setAMT203Zero();
    uint8_t SPI_T(uint8_t msg);

private:
    int _cs;

    const uint8_t CMD_SET_ZERO = 0x70;
    const uint8_t CMD_NOP = 0x00;
    const uint8_t RESP_OK = 0x80;

    const int MAX_COUNTER = 1000; // Código original 500
};

#endif
