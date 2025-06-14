#include "Encoder.h"

Encoder::Encoder(int csPin) : _cs(csPin) {}

void Encoder::begin() {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);  // Deseleccionar por defecto
}

int Encoder::id() {
    return _cs;
}

float Encoder::leerGrados() {
    int cont = 0;
    uint8_t recieved = 0xA5;
    float deg = 0.00;
    uint16_t ABSposition = 0;
    uint8_t temp[2];

    digitalWrite(_cs, LOW);
    SPI_T(0x10);
    delayMicroseconds(25);

    recieved = SPI_T(0x00);
    delayMicroseconds(25);

    while (recieved != 0x10 && cont < MAX_COUNTER) {
        recieved = SPI_T(0x00);
        delayMicroseconds(25);
        cont++;
    }

    if (cont >= MAX_COUNTER) {
        Serial.print(_cs);
        Serial.println(" no responde.");
        digitalWrite(_cs, HIGH);
        return -1;
    }

    temp[0] = SPI_T(0x00);  // MSB
    delayMicroseconds(25);
    temp[1] = SPI_T(0x00);  // LSB
    digitalWrite(_cs, HIGH);

    temp[0] &= ~0xF0;
    ABSposition = (temp[0] << 8) + temp[1];

    deg = ABSposition * 0.08789;
    return deg;
}

bool Encoder::setAMT203Zero()
{
    int cont = 0;
    uint8_t recieved = 0x00;

    SPI_T(0x70); // Enviar comando set_zero_point
    delayMicroseconds(25);

    // Enviar nop (0x00) hasta que se reciba 0x80 o se alcance el límite
    recieved = SPI_T(0x00);
    delayMicroseconds(25);

    while (recieved != 0x80 && cont < MAX_COUNTER)
    {
        recieved = SPI_T(0x00);
        delayMicroseconds(25);
        cont++;
    }

    // Analizar el resultado
    if (recieved == 0x80)
    {
        Serial.print("Cero establecido con éxito en encoder ");
        Serial.println(_cs);
        return true;
    }
    else
    {
        Serial.print("Fallo al establecer cero en encoder ");
        Serial.println(_cs);
        return false;
    }
}

uint8_t Encoder::SPI_T(uint8_t msg)
{
    digitalWrite(_cs, LOW);
    uint8_t response = SPI.transfer(msg);
    digitalWrite(_cs, HIGH);
    return response;
}