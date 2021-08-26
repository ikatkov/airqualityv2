#include "CircularBuffer.h"


CircularBuffer:: CircularBuffer() = default;

void CircularBuffer::add(float value) {
    _buffer[_position] = value;
    _position = (_position + 1) % MAX_LENGTH;
    _length++;
    if(_length > MAX_LENGTH)
        _length = MAX_LENGTH;
}

float CircularBuffer::getAverage() {
    if(_length == 0) return 0;
    float sum = 0;
    for (int i = 0; i < _length; i++) {
        sum += _buffer[i];
    }
    return sum/(float)_length;
}