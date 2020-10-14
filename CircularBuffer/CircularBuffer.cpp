#include "CircularBuffer.h"


CircularBuffer:: CircularBuffer() = default;

void CircularBuffer::add(uint16_t value) {
    _buffer[_position] = value;
    _position = (_position + 1) % MAX_LENGTH;
    _length++;
    if(_length > MAX_LENGTH)
        _length = MAX_LENGTH;
}

uint16_t CircularBuffer::getAverage() {
    uint16_t sum = 0;
    for (int i = 0; i < _length; i++) {
        Serial.println(_buffer[i]);
        sum += _buffer[i];
    }
    return lround(1.0*sum/_length);
}