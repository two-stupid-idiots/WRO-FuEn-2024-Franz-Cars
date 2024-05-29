#include "ColorSensor.h"

ColorSensor color;

void ColorSensor::init(void) {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);

  logger.info("[Color]  Initialized");
}

int ColorSensor::getRedFrequency(void) {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  int redFrequency = pulseIn(OUT, LOW);
  logger.trace(("[Color]  Red: " + String(redFrequency)).c_str());
  return redFrequency;
}

int ColorSensor::getGreenFrequency(void) {
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  int greenFrequency = pulseIn(OUT, LOW);
  logger.trace(("[Color]  Green: " + String(greenFrequency)).c_str());
  return greenFrequency;
}

int ColorSensor::getBlueFrequency(void) {
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  int blueFrequency = pulseIn(OUT, LOW);
  logger.trace(("[Color]  Blue: " + String(blueFrequency)).c_str());
  return blueFrequency;
}

int ColorSensor::getNoFilter(void) {
  digitalWrite(S2, HIGH);
  digitalWrite(S3, LOW);
  int frequency = pulseIn(OUT, LOW);
  logger.trace(("[Color]  No Filter: " + String(frequency)).c_str());
  return frequency;
}

void ColorSensor::test(void) {
  LogLevel logLevel = logger.getLevel();
  logger.setLevel(LogLevel::TRACE);
  getRedFrequency();
  delay(500);
  getGreenFrequency();
  delay(500);
  getBlueFrequency();
  delay(500);
  getNoFilter();
  logger.trace("--------");
  logger.setLevel(logLevel);
  delay(1000);
}
