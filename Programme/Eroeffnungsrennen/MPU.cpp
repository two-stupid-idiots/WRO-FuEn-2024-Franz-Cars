#include "MPU.h"

MPU mpu;

void MPU::init(void) {
  Wire.begin();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(GYRO_CONFIG);
  Wire.write(0x00);
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(ACCEL_CONFIG);
  Wire.write(0x00);
  Wire.endTransmission(true);

  calibrateGyro();
  lastTime = millis();

  logger.info("[MPU]    Initialized");
}

void MPU::calibrateGyro(void) {
  const int numReadings = 5000;
  float gyroX_sum = 0, gyroY_sum = 0, gyroZ_sum = 0;

  for (int i = 0; i < numReadings; i++) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(GYRO_XOUT_H);
    Wire.endTransmission(false);

    Wire.requestFrom(MPU_ADDR, 6, true);

    gyroX_sum += (Wire.read() << 8 | Wire.read()) / 131.0;
    gyroY_sum += (Wire.read() << 8 | Wire.read()) / 131.0;
    gyroZ_sum += (Wire.read() << 8 | Wire.read()) / 131.0;

    logger.trace(("[MPU]    Calibration: " + String(i) + "/" + String(numReadings) + " >> x: " + String(gyroX_sum) + " | y: " + String(gyroY_sum) + " | z: " + String(gyroZ_sum)).c_str());
  }

  gyroX_bias = gyroX_sum / (float) numReadings ;
  gyroY_bias = gyroY_sum / (float) numReadings ;
  gyroZ_bias = gyroZ_sum / (float) numReadings ;

  logger.debug(("[MPU]    Gyro calibration value: x: " + String(gyroX_bias) + " y: " + String(gyroY_bias) + " z: " + String(gyroZ_bias)).c_str());
}

void MPU::scan(void) {
  byte error, address;
  int nDevices = 0;

  logger.info("[MPU]    Scanning...");

  for (address = 0; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    Wire.beginTransmission(address+1);

    if(error == 0 && Wire.endTransmission() != 0) {
      if(address < 16) {
        logger.info(("[MPU]    I2C Device found device at address 0x0" + String(address, HEX)).c_str());
      }
      logger.info(("[MPU]    I2C Device found device at address 0x" + String(address, HEX)).c_str());
      nDevices++;
    } else if(error == 4) {
      if(address < 16) {
        logger.info(("[MPU]    Unkown error at address 0x00" + String(address, HEX)).c_str());
      }
      logger.debug(("[MPU]    Unkown error at address 0x0" + String(address, HEX)).c_str());
    }
  }
  if(nDevices == 0) {
    logger.error("[MPU]    No I2C devices found");
  } else {
    logger.info("[MPU]    Scan completed");
  }
  delay(5000);
}

void MPU::readRawData(void) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_ADDR, 14, true);

  accX = (Wire.read() << 8 | Wire.read()) / 16384.0;
  accY = (Wire.read() << 8 | Wire.read()) / 16384.0;
  accZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

  temp = (Wire.read() << 8 | Wire.read()) / 340.0 + 36.53;

  gyroX = (Wire.read() << 8 | Wire.read()) / 65.50 - gyroX_bias;
  gyroY = (Wire.read() << 8 | Wire.read()) / 65.50 - gyroY_bias;
  gyroZ = (Wire.read() << 8 | Wire.read()) / 65.50 - gyroZ_bias;

  logger.trace(("[MPU]    AcX: " + String(accX) + " | AcY: " + String(accY) + " | AcZ: " + String(accZ) + " | Tmp: " + String(temp) + " | GyX: " + String(gyroX) + " | GyY: " + String(gyroY) + " | GyZ: " + String(gyroZ)).c_str());
}

void MPU::calculateAngle(void) {
  unsigned long currentTime = millis();
  deltaTime = (float)(currentTime - lastTime) / 1000;
  lastTime = currentTime;

  //        !!!--------!!!
  //muss weg
  logger.info(("[MPU]    Current: " + String(currentTime) + " | Delta: " + String(deltaTime)  + " | gyroZ: " + String(gyroZ)).c_str());

  roll = atan2(accY, accZ) * 180 / PI;
  pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * 180 / PI;
  //yaw = (0.1 * (gyroZ * deltaTime) + (1 - 0.1) * yaw) * 1.1;

  if (gyroZ > 1 || gyroZ < -1) {
    yaw = (0.1 * (gyroZ * deltaTime) + (1 - 0.1) * yaw);
  }
  

  float alpha = 0.98;
  roll = alpha * (roll + gyroX * deltaTime) + (1 - alpha) * roll;
  pitch = alpha * (pitch + gyroY * deltaTime) + (1 - alpha) * pitch;

  //        !!!--------!!!
  //muss maximal auf debug
  logger.info(("[MPU]    Roll: " + String(roll) + " | Pitch: " + String(pitch) + " | Yaw: " + String(yaw)).c_str());
}

void MPU::test(void) {
  LogLevel logLevel = logger.getLevel();
  logger.setLevel(LogLevel::TRACE);
  readRawData();
  delay(1000);
  logger.setLevel(logLevel);
}
