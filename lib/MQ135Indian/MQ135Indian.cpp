#include "MQ135Indian.h"


MQ135Indian::MQ135Indian(uint8_t pin) {
  _pin = pin;
}


float getScalingFactor(float t, float h) {
  return 1.6979f-0.012f*t-0.00612f*h;
}


float MQ135Indian::getResistance() {
  int val = analogRead(_pin);
  return ((1023./(float)val) - 1.)*RLOAD;
}


float MQ135Indian::getCorrectedResistance(float t, float h) {
  return getResistance()/getScalingFactor(t, h);
}



float MQ135Indian::getCorrectedPPM(float t, float h) {
  return PARAI * pow((getCorrectedResistance(t, h)/RZEROI), PARBI);
}



float MQ135Indian::getCorrectedRZero(float t, float h) {
  return getCorrectedResistance(t, h) * pow((PARAI/ATMOCO2), (1./PARBI));
}
