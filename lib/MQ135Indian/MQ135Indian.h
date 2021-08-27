 // see https://www.researchgate.net/publication/328875972_Influence_of_Temperature_and_Humidity_on_the_Output_Resistance_Ratio_of_the_MQ-135_Sensor
 
 #include "Arduino.h"

/// The load resistance on the board
#define RLOAD 3300
/// Calibration resistance at atmospheric CO2 level
#define RZEROI 21763.35 
/// Parameters for calculating ppm of CO2 from sensor resistance
#define PARAI 121.4517f
#define PARBI -2.78054f


/// Atmospheric CO2 level for calibration purposes
#define ATMOCO2 397.13

class MQ135Indian {
 private:
  uint8_t _pin;
 private:
  float getResistance();

 public:
  MQ135Indian(uint8_t pin);
  float getCorrectedResistance(float t, float h);
  float getCorrectedPPM(float t, float h);
  float getCorrectedRZero(float t, float h);
};

