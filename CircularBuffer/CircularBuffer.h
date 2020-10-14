#pragma once
#include "Arduino.h"


#define MAX_LENGTH 15

class CircularBuffer {
public:
    CircularBuffer();
    void add(uint16_t value);
    uint16_t getAverage();

private:
    uint16_t _buffer[MAX_LENGTH];
    uint8_t _position = 0;
    uint8_t _length = 0;
};


