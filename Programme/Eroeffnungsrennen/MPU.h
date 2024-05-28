#ifndef MPU_H
#define MPU_H

#include "logger.h"
#include <Arduino.h>
#include <Wire.h>

class MPU {
  public:
    void init(void);
    void scan(void);
    void readRawData(void);
    void calculateAngle(void);
    void test(void);
    float getRoll(void) const;
    float getPitch(void) const;
    float getYaw(void) const;
    float getTemp(void) const;
  private:
    #define MPU_ADDR 0x68
    #define PWR_MGMT_1 0x6B
    #define GYRO_CONFIG 0x1B
    #define ACCEL_CONFIG 0x1C
    #define ACCEL_XOUT_H 0x3B
    #define GYRO_XOUT_H 0x43
    unsigned long lastTime;
    float deltaTime;
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
    float gyroX_bias, gyroY_bias, gyroZ_bias;
    float roll, pitch, yaw;
    float temp;

    void calibrateGyro(void);
};

extern MPU mpu;

#endif