#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"


class TCS34725 {
   public:
    bool begin() {
      if (tcs.begin()) {
        Serial.println("Found TCS3472 sensor");
        _ok = 1;
      } else {
        Serial.println("No TCS3472 found ... check your connections");
        return 0;
      }
      return _ok;
    }
    Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);


    bool state() {
        return _ok;
    }


    void attachCalibrate(uint16_t* r, uint16_t* g, uint16_t* b) {
        _maxr = r;
        _maxb = b;
        _maxg = g;
    }

    void calibrate() {
        if (!_maxr) return;
        readSensorData();
        *_maxr = _r;
        *_maxg = _g;
        *_maxb = _b;
    }


    void readSensorData() {
        tcs.getRawData(&_r, &_g, &_b, &_c);
    }

    // calibrated
    uint32_t getRGB() {
        readSensorData();
        return ((uint32_t)_r << 16) | (_g << 8) | (_b << 0);
    }
    uint8_t getRed() {
        return _getColor(_r, _maxr);
    }
    uint8_t getGreen() {
        return _getColor(_g, _maxg);
    }
    uint8_t getBlue() {
        return _getColor(_b, _maxb);
    }

   private:
    uint16_t* _maxr = nullptr;
    uint16_t* _maxg = nullptr;
    uint16_t* _maxb = nullptr;
    bool _ok = 0;
    uint16_t _r, _g, _b, _c;

    uint8_t _getColor(uint16_t colVal, uint16_t* cal) {
        if (!_ok) return 0;
        uint16_t v = colVal;
        if (cal && *cal) v = (uint32_t)v * 255 / (*cal);
        if (v > 255) v = 255;
        return ((uint32_t)v * v + 255) >> 8;
    }
};