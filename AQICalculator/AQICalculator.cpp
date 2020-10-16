#include "AQICalculator.h"

#define  AMOUNT_OF_LEVELS 6
#define  MAX_INDEX 500

 uint16_t index_grid_[AMOUNT_OF_LEVELS][2]  = {{0, 51}, {51, 100}, {101, 150}, {151, 200}, {201, 300}, {301, 500}};
 uint16_t pm2_5_calculation_grid_[AMOUNT_OF_LEVELS][2]  = {{0, 12}, {13, 35}, {36, 55}, {56, 150}, {151, 250}, {251, 500}};
 uint16_t pm10_0_calculation_grid_[AMOUNT_OF_LEVELS][2]  = {{0, 54}, {55, 154},  {155, 254}, {255, 354}, {355, 424}, {425, 604}};

AQICalculator:: AQICalculator()   = default;



double correctValue(uint16_t value, uint8_t humidity) {
    //this magic equation is from "PURPLEAIR PM2.5 U.S. CORRECTION AND PERFORMANCE DURING SMOKE EVENTS"
    //it corrects PMS5003 values to match US official
    return 0.534 * value - 0.0844 * humidity + 5.604;
}

int get_grid_index_(uint16_t value, uint16_t array[AMOUNT_OF_LEVELS][2]) {
    for (int i = 0; i < AMOUNT_OF_LEVELS; i++) {
        if (value >= array[i][0] && value <= array[i][1]) {
            return i;
        }
    }
    return -1;
}

double calculate_index_(double value, uint16_t array[AMOUNT_OF_LEVELS][2]) {
    uint16_t grid_index = get_grid_index_(value, array);
    if(grid_index == -1) return MAX_INDEX;
    uint16_t aqi_lo = index_grid_[grid_index][0];
    uint16_t aqi_hi = index_grid_[grid_index][1];
    uint16_t conc_lo = array[grid_index][0];
    uint16_t conc_hi = array[grid_index][1];

    return ((double)(aqi_hi - aqi_lo) / (conc_hi - conc_lo)) * (value - conc_lo) + aqi_lo;
}


uint16_t AQICalculator::getAqi(uint16_t pm2_5_value, uint16_t pm10_0_value, uint8_t humidity)  {
    uint16_t pm2_5_index = lround(calculate_index_(correctValue(pm2_5_value,humidity) , pm2_5_calculation_grid_));
    uint16_t pm10_0_index = lround(calculate_index_(correctValue(pm10_0_value,humidity), pm10_0_calculation_grid_));
   // Serial.println("pm2_5_index " + String(pm2_5_index));
   // Serial.println("pm10_0_index " + String(pm10_0_index));
    return (pm2_5_index < pm10_0_index) ? pm10_0_index : pm2_5_index;
}


