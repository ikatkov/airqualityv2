#pragma once
#include "Arduino.h"


#define MAX_LENGTH 15

class CircularBuffer {
public:
    CircularBuffer();
    void add(float value);
    float getAverage();

private:
    float _buffer[MAX_LENGTH];
    uint8_t _position = 0;
    uint8_t _length = 0;
};


