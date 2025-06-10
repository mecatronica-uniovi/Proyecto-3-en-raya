#include "Motores.h"
#include "configuracion.h"

// Variables estáticas para interrupciones
volatile bool Motores::fdcInterruptTriggered = false;
Motores* Motores::activeInstance = nullptr;

Motores::Motores() : regulador(nullptr) {
    _pos = 0;
    if (!_spi_initialized) {
        SPI.begin();
        SPI.setBitOrder(MSBFIRST);
        SPI.setDataMode(SPI_MODE0);
        SPI.setClockDivider(SPI_CLOCK_DIV32);
        _spi_initialized = true;
    }
}

Motores::~Motores() {
    if (_spi_initialized) {
        SPI.end();
    }
}

void Motores::configurarPines(int in1, int in2, int en) {
    _in1 = in1;
    _in2 = in2;
    _en = en;
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(en, OUTPUT);
}

void Motores::asignarFDC(int fdc1, int fdc2) {
    _fdc1 = fdc1;
    _fdc2 = fdc2;
    
    pinMode(fdc1, INPUT_PULLUP);
    pinMode(fdc2, INPUT_PULLUP);
    
    // Configurar interrupciones
    if (fdc1 != -1) {
        attachInterrupt(digitalPinToInterrupt(fdc1), ISRWrapper, FALLING);
    }
    if (fdc2 != -1) {
        attachInterrupt(digitalPinToInterrupt(fdc2), ISRWrapper, FALLING);
    }
}

void Motores::handleFDCInterrupt(int fdcPin) {
    if ((fdcPin == _fdc1 && digitalRead(_fdc1) == LOW) || 
        (fdcPin == _fdc2 && digitalRead(_fdc2) == LOW)) {
        detener();
        fdcInterruptTriggered = true;
    }
}

void Motores::ISRWrapper() {
    if (activeInstance) {
        activeInstance->handleFDCInterrupt(digitalPinToInterrupt(activeInstance->_fdc1));
    }
}

void Motores::asignarEncoder(int cs_encoder) {
    _cs_encoder = cs_encoder;
    pinMode(cs_encoder, OUTPUT);
    digitalWrite(cs_encoder, HIGH);
}

uint8_t Motores::SPI_T(uint8_t msg, int cs) {
    _cs_encoder = cs;
    digitalWrite(_cs_encoder, LOW);
    uint8_t response = SPI.transfer(msg);
    digitalWrite(_cs_encoder, HIGH);
    return response;
}

float Motores::leerEncoder() {
    int count = 0;
    uint8_t received = 0xA5;
    uint16_t ABSposition = 0;
    uint8_t temp[2];

    SPI.begin();
    digitalWrite(_cs_encoder, LOW);
    SPI_T(0x10, _cs_encoder);
    delayMicroseconds(25);

    received = SPI_T(0x00, _cs_encoder);
    delayMicroseconds(25);

    while (received != 0x10 && count < 500) {
        received = SPI_T(0x00, _cs_encoder);
        delayMicroseconds(25);
        count++;
    }

    if (count >= 500) {
        Serial.println("Encoder not responding");
        SPI.end();
        return -1;
    }

    temp[0] = SPI_T(0x00, _cs_encoder);
    delayMicroseconds(25);
    temp[1] = SPI_T(0x00, _cs_encoder);
    digitalWrite(_cs_encoder, HIGH);
    SPI.end();

    temp[0] &= ~0xF0;
    ABSposition = temp[0] << 8;
    ABSposition += temp[1];

    _pos = ABSposition * 0.08789; // Actualizar _pos
    return _pos;
}

void Motores::moverAGrados(double gradosRelativos) {
    // Verificar interrupciones activadas
    if (fdcInterruptTriggered) {
        fdcInterruptTriggered = false;
        return;
    }

    // Verificar finales de carrera
    if ((gradosRelativos < 0 && _fdc1 != -1 && digitalRead(_fdc1) == LOW) ||
        (gradosRelativos > 0 && _fdc2 != -1 && digitalRead(_fdc2) == LOW)) {
        detener();
        return;
    }

    activeInstance = this;
    
    float inicio = leerEncoder();
    if (inicio < 0) {
        activeInstance = nullptr;
        return;
    }

    float destino = fmod(inicio + gradosRelativos + 360.0, 360.0);
    bool sentidoHorario = gradosRelativos >= 0;

    digitalWrite(_in1, sentidoHorario ? HIGH : LOW);
    digitalWrite(_in2, sentidoHorario ? LOW : HIGH);

    while (!fdcInterruptTriggered) {
        float actual = leerEncoder();
        if (actual < 0) break;

        // Cálculo de error
        float error = 0;
        if (sentidoHorario) {
            error = (destino > inicio) ? (destino - actual) : 
                   ((actual < inicio) ? (destino + 360.0 - actual) : (destino - actual));
        } else {
            error = (destino < inicio) ? (destino - actual) : 
                   ((actual > inicio) ? (destino - 360.0 - actual) : (destino - actual));
        }

        // Control PID
        int pwm = PWM_VELOCIDAD;
        if (regulador) {
            pwm = constrain(abs(regulador->calcularUk(error)), PWM_MIN, PWM_MAX);
            
            // Ajustar dirección si es necesario
            bool nuevoSentido = error >= 0;
            if (nuevoSentido != sentidoHorario) {
                sentidoHorario = nuevoSentido;
                digitalWrite(_in1, sentidoHorario ? HIGH : LOW);
                digitalWrite(_in2, sentidoHorario ? LOW : HIGH);
            }
        }

        analogWrite(_en, pwm);

        // Condición de parada por precisión
        if (abs(error) < 1.0) break;

        // Verificación periódica de finales de carrera
        if ((_fdc1 != -1 && digitalRead(_fdc1) == LOW && !sentidoHorario) ||
            (_fdc2 != -1 && digitalRead(_fdc2) == LOW && sentidoHorario)) {
            detener();
            break;
        }

        delay(5);
    }

    detener();
    activeInstance = nullptr;
    fdcInterruptTriggered = false;
}

void Motores::mover() {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    if (_en >= 0 && _en <= 33) {
        analogWrite(_en, PWM_VELOCIDAD);
    }
}

void Motores::moverPWM(float pwm) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    if (_en >= 0 && _en <= 33) {
        analogWrite(_en, pwm);
    }
}

void Motores::detener() {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
    analogWrite(_en, 0);
}

void Motores::setRegulador(RegPID* reg) {
    regulador = reg;
}

void Motores::configurarPinza(int pin_servo, int angulo_abierto, int angulo_cerrado) {
    _angulo_abierto = angulo_abierto;
    _angulo_cerrado = angulo_cerrado;
    _servoPinza.attach(pin_servo);
    _servoPinza.write(angulo_abierto);
    _pinza_abierta = true;
}

void Motores::moverPinza() {
    if (_pinza_abierta) {
        _servoPinza.write(_angulo_cerrado);
    } else {
        _servoPinza.write(_angulo_abierto);
    }
    _pinza_abierta = !_pinza_abierta;
}