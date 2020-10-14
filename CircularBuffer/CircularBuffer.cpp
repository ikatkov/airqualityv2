#include "CircularBuffer.h"

 uint16_t buffer[MAX_LENGTH];
 uint8_t position = 0;
 uint8_t length = 0;


CircularBuffer:: CircularBuffer() = default;

void CircularBuffer::add(uint16_t value) {
    buffer[position] = value;
    position = (position + 1) % MAX_LENGTH;
    length++;
    if(length > MAX_LENGTH)
        length = MAX_LENGTH;
}

uint16_t CircularBuffer::getAverage() {
    uint16_t sum = 0;
    for (int i = 0; i < length; i++) {
        Serial.println(buffer[i]);
        sum += buffer[i];
    }
    return lround(1.0*sum/length);
}