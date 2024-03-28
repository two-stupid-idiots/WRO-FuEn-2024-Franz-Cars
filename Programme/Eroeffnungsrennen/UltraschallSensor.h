#ifndef ULTRASCHALLSENSOR_H
#define ULTRASCHALLSENSOR_H

#include <Arduino.h>
#include "Logger.h"

enum class side {
  left,
  front,
  right
};

class UltraschallSensor {
  public:
    void init(void);
    int getDistance(side side);
    void test(void);

  private:
    #define LEFT_TRIG_PIN  13
    #define LEFT_ECHO_PIN  12

    #define FRONT_TRIG_PIN  5
    #define FRONT_ECHO_PIN  4

    #define RIGHT_TRIG_PIN  3
    #define RIGHT_ECHO_PIN  2

    int getUltrasonicData(int TRIG_PIN, int ECHO_PIN);
};

extern UltraschallSensor ir;

#endif