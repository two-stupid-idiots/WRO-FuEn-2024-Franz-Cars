#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include "Logger.h"
#include <Arduino.h>

class ServoController {
  public:
    void init(void);
    void set(int value);
    void test(void);

  private:
    #define SERVO_ENL_PIN  9
    #define SERVO_IN1_PIN 10
    #define SERVO_IN2_PIN 11
};

extern ServoController servo;

#endif