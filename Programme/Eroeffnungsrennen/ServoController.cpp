#include "ServoController.h"

ServoController servo;

void ServoController::init(void) {
  pinMode(SERVO_ENL_PIN, OUTPUT);
  pinMode(SERVO_IN1_PIN, OUTPUT);
  pinMode(SERVO_IN2_PIN, OUTPUT);

  logger.info("[Servo]  Initialized");
}

void ServoController::set(int value) {
  if (value >= 0) {
    digitalWrite(SERVO_IN1_PIN, HIGH);
    digitalWrite(SERVO_IN2_PIN, LOW);
  }

  if (value < 0) {
    digitalWrite(SERVO_IN1_PIN, LOW);
    digitalWrite(SERVO_IN2_PIN, HIGH);
  }

  if (value >= -100 && value <= 100) {
    analogWrite(SERVO_ENL_PIN, int(abs(value)*2.55));
    logger.trace(("[Servo]  Value: " + String(value) + "%").c_str());
  } else {
    logger.error("[Servo]  Value must be between -100 and 100");
  }
}

void ServoController::test(void) {
  LogLevel level = logger.getLevel();
  logger.setLevel(LogLevel::TRACE);
  for (int i = 0; i <= 98; i += 14) {
    set(i);
    delay(1000);
  }
  for (int i = 98; i >= 0; i += -14) {
    set(i);
    delay(1000);
  }
  for (int i = -98; i <= 0; i += 14) {
    set(i);
    delay(1000);
  }
  logger.debug("--------");
  logger.setLevel(level);
}
