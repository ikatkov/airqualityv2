#pragma once
#include "Arduino.h"

// this code is copied from https://github.com/esphome/esphome/blob/dev/esphome/components/hm3301/aqi_calculator.cpp

class AQICalculator {
public:
    AQICalculator();
    static uint16_t getAqi(uint16_t pm2_5_value, uint16_t pm10_0_value, uint8_t humidity);
};



struct pms5003data {
    uint16_t framelen;
    uint16_t pm10_standard, pm25_standard, pm100_standard;
    uint16_t pm10_env, pm25_env, pm100_env;
    uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
    uint16_t unused;
    uint16_t checksum;
};