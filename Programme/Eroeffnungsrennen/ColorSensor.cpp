#include "ColorSensor.h"

ColorSensor color;

void ColorSensor::init(void) {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  for (int i = 0; i < 4; i++) {
    orange_low[i] = orange_off[i] - orange_off[i] * 0.2;
    orange_up[i] = orange_off[i] + orange_off[i] * 0.2;
    blue_low[i] = blue_off[i] - blue_off[i] * 0.2;
    blue_up[i] = blue_off[i] + blue_off[i] * 0.2;
  }

  logger.debug(("[Color]  Orange Range: Red: " + String(orange_low[0]) + "-" + String(orange_up[0]) + " | Green: " + String(orange_low[1]) + "-" + String(orange_up[1]) + " | Blue: " + String(orange_low[2]) + "-" + String(orange_up[2]) + " | NoFilter: " + String(orange_low[3]) + "-" + String(orange_up[3])).c_str());
  logger.debug(("[Color]  Blue Range: Red: " + String(blue_low[0]) + "-" + String(blue_up[0]) + " | Green: " + String(blue_low[1]) + "-" + String(blue_up[1]) + " | Blue: " + String(blue_low[2]) + "-" + String(blue_up[2]) + " | NoFilter: " + String(blue_low[3]) + "-" + String(blue_up[3])).c_str());

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

void ColorSensor::calibrate(void) {
  int runtime = 200;
  for (int i = 0; i < runtime; i++) {
    calibrate_raw[0] += getRedFrequency();
    calibrate_raw[1] += getGreenFrequency();
    calibrate_raw[2] += getBlueFrequency();
    calibrate_raw[3] += getNoFilter();

    logger.info(("[Color]  " + String(i+1) + "/" + String(runtime) + " | Red: " + String(calibrate_raw[0]) + " | Green: " + String(calibrate_raw[1]) + " | Blue: " + String(calibrate_raw[2]) + " | NoFilter: " + String(calibrate_raw[3])).c_str());
    delay(500);
  }
  calibrate_off[0] = calibrate_raw[0] / runtime;
  calibrate_off[1] = calibrate_raw[1] / runtime;
  calibrate_off[2] = calibrate_raw[2] / runtime;
  calibrate_off[3] = calibrate_raw[3] / runtime;

  logger.info(("[Color]  Red: " + String(calibrate_off[0]) + " | Green: " + String(calibrate_off[1]) + " | Blue: " + String(calibrate_off[2]) + " | NoFilter: " + String(calibrate_off[3])).c_str());
}

dir ColorSensor::isLine(void) {
  now[0] = getRedFrequency();
  now[1] = getGreenFrequency();
  now[2] = getBlueFrequency();
  now[3] = getNoFilter();

  if ((now[0] > orange_low[0] && now[0] < orange_up[0]) && (now[1] > orange_low[1] && now[1] < orange_up[1]) && (now[2] > orange_low[2] && now[2] < orange_up[2]) && (now[3] > orange_low[3] && now[3] < orange_up[3])) {
    logger.debug("[Color]  Orange");
    logger.debug(("[Color]  Red: " + String(now[0]) + " | Green: " + String(now[1]) + " | Blue: " + String(now[2]) + " | NoFilter: " + String(now[3])).c_str());
    return dir::right;
  }
  if ((now[0] > blue_low[0] && now[0] < blue_up[0]) && (now[1] > blue_low[1] && now[1] < blue_up[1]) && (now[2] > blue_low[2] && now[2] < blue_up[2]) && (now[3] > blue_low[3] && now[3] < blue_up[3])) {
    logger.debug("[Color]  Blue");
    logger.debug(("[Color]  Red: " + String(now[0]) + " | Green: " + String(now[1]) + " | Blue: " + String(now[2]) + " | NoFilter: " + String(now[3])).c_str());
    return dir::left;
  }
  
  return dir::null;
}