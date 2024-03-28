#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include "Logger.h"
#include <Arduino.h>

class MotorController {
  public:
    void init(void);
    void run(int speed);
    void test(void);

  private:
    #define MOTOR_ENL_PIN 6
    #define MOTOR_IN1_PIN 7
    #define MOTOR_IN2_PIN 8
};

extern MotorController motor;

#endif