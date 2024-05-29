#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include "logger.h"
#include <Arduino.h>

class ColorSensor {
  public:
    void init(void);
    int getRedFrequency(void);
    int getGreenFrequency(void);
    int getBlueFrequency(void);
    int getNoFilter(void);
    void test(void);
  private:
    #define S0 28
    #define S1 30
    #define S2 24
    #define S3 26
    #define OUT 22
};

extern ColorSensor color;

#endif