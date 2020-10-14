#pragma once
#include "Arduino.h"


#define MAX_LENGTH 15

class CircularBuffer {
public:
    CircularBuffer();
    void add(uint16_t value);
    uint16_t getAverage();
};


