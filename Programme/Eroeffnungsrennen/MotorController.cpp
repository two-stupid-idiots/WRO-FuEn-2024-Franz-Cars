#include "MotorController.h"

MotorController motor;

void MotorController::init(void) {
  pinMode(MOTOR_ENL_PIN, OUTPUT);
  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);
  
  logger.info("[Motor]  Initialized");
}

void MotorController::run(int speed) {
  if (speed >= 0) {
    digitalWrite(MOTOR_IN1_PIN, HIGH);
    digitalWrite(MOTOR_IN2_PIN, LOW);
  }

  if (speed < 0) {
    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, HIGH);
  }

  if (speed >= -100 && speed <= 100) {
    analogWrite(MOTOR_ENL_PIN, int(abs(speed)*2.55));
    logger.debug(("[Motor]  Speed: " + String(speed) + "%").c_str());
  } else {
    logger.error("[Motor]  Speed must be between -100 and 100");
  }
}

void MotorController::test() {
    LogLevel level = logger.getLevel();
    logger.setLevel(LogLevel::DEBUG);
  for (int i = 0; i <= 100; i += 25) {
    run(i);
    delay(1000);
  }
  for (int i = 75; i >= -100; i -= 25) {
    run(i);
    delay(1000);
  }
  for (int i = -75; i <= 0; i += 25) {
    run(i);
    delay(1000);
  }
  logger.debug("--------");
  logger.setLevel(level);
}
