#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include "logger.h"
#include <Arduino.h>

enum class dir {
  right,
  left,
  null
};

class ColorSensor {
  public:
    void init(void);
    int getRedFrequency(void);
    int getGreenFrequency(void);
    int getBlueFrequency(void);
    int getNoFilter(void);
    void test(void);
    void startCalibrate(void);
    dir isLine(void); 
  private:
    #define S0 28
    #define S1 30
    #define S2 24
    #define S3 26
    #define OUT 22
    long calibrate_raw[4];
    long calibrate_orange_raw[4][4];
    long calibrate_blue_raw[4][4];

    int calibrate_off[4];
    int calibrate_orange_off[4][4];
    int calibrate_blue_off[4][4];

    int orange_raw[4];
    int blue_raw[4];
    
    int orange_off[4] = {23, 31, 24, 9};
    int blue_off[4] = {54, 43, 23, 12};

    int calibrate_up[4];
    int calibrate_low[4];
    int orange_up[4];
    int orange_low[4];
    int blue_up[4];
    int blue_low[4];

    int now[4];

    void calibrate(char mode, int corners, int runtime);
};

extern ColorSensor color;

#endif