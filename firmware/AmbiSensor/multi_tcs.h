#pragma once
#include <Arduino.h>

#include "tcs34725.h"
#define TCA_ADDR 0x70
#define SENSOR_AMOUNT 8

class MultiTcs34725 {
   public:
    void begin() {
        for (uint8_t i = 0; i < SENSOR_AMOUNT; i++) {
            _select(i);
            get(i).begin();
        }
    }
    TCS34725& get(uint8_t i) {
        _select(i);
        return rgb[i];
    }

    TCS34725 rgb[SENSOR_AMOUNT];

   private:
    void _select(uint8_t ch) {
        Wire.beginTransmission(TCA_ADDR);
        Wire.write(1 << ch);
        Wire.endTransmission();
    }
};