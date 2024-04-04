#include "UltraschallSensor.h"

void UltraschallSensor::init(void) {
  pinMode(LEFT_TRIG_PIN, OUTPUT);
  pinMode(LEFT_ECHO_PIN, INPUT);

  pinMode(FRONT_TRIG_PIN, OUTPUT);
  pinMode(FRONT_ECHO_PIN, INPUT);

  pinMode(RIGHT_TRIG_PIN, OUTPUT);
  pinMode(RIGHT_ECHO_PIN, INPUT);

  logger.debug("[ULTRA] Initialized");
}

int UltraschallSensor::getDistance(side side) {
  int distance;
  switch (side) {
    case side::left:
      distance = getUltrasonicData(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
      logger.debug(("[ULTRA] left distance: " + String(distance) + "cm").c_str());
      return distance;
    case side::front:
      distance = getUltrasonicData(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
      logger.debug(("[ULTRA] front distance: " + String(distance) + "cm").c_str());
      return distance;
    case side::right:
      distance = getUltrasonicData(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);
      logger.debug(("[ULTRA] right distance: " + String(distance) + "cm").c_str());
      return distance;
  }
}

void UltraschallSensor::test(void) {
  LogLevel logLevel = logger.getLevel();
  logger.setLevel(LogLevel::DEBUG);
  getDistance(side::left);
  getDistance(side::front);
  getDistance(side::right);
  delay(1000);
  logger.debug("--------");
  logger.setLevel(logLevel);
}

int UltraschallSensor::getUltrasonicData(int TRIG_PIN, int ECHO_PIN) {
  unsigned int i = 0;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  i = ((unsigned int)pulseIn(ECHO_PIN, HIGH) / 58);
  return i;
}

UltraschallSensor ir;