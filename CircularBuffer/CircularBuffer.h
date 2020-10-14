#pragma once
#include "Arduino.h"


#define MAX_LENGTH 15

class CircularBuffer {
public:
    CircularBuffer();
    static void add(uint16_t value);
    static uint16_t getAverage();

};


