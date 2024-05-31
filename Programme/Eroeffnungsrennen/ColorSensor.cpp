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

void ColorSensor::startCalibrate(void) {
  logger.info("[Color]  Einzelne oder Mehrfache Kalibrierung? [e/m]");
  while (Serial.available() == 0) {}
  char mode = Serial.read();
  
  if (mode == 'e') {
      logger.info("[Color]  Einzelkalibrierung");

      logger.info("[Color]  Mit vielen Werten soll gemittelt werden?");
      while (Serial.available() == 0) {}
      int runtime = Serial.parseInt();
      logger.info(("[Color]  " + String(runtime) + " Werte werden genommen.").c_str());

      logger.info("[Color]  Drücke beliebige Taste zum Starten der Kalibrierung.");
      while (Serial.available() == 0) {}
      Serial.read();

      calibrate(mode, NULL, runtime);
  } else if (mode == 'm') {
      logger.info("[Color]  Mehrfachkalibrierung");

      logger.info("[Color]  Wie viele Ecken sollen kalibriert werden?");
      while (Serial.available() == 0) {}
      int corners = Serial.parseInt();
      if (corners == 1) {
        logger.info(("[Color]  " + String(corners) + " Ecke wird kalibriert.").c_str());
      } else {
        logger.info(("[Color]  " + String(corners) + " Ecken werden kalibriert.").c_str());
      }

      logger.info("[Color]  Mit vielen Werten soll gemittelt werden?");
      while (Serial.available() == 0) {}
      int runtime = Serial.parseInt();
      logger.info(("[Color]  " + String(runtime) + " Werte werden genommen.").c_str());

      logger.info("[Color]  Drücke beliebige Taste zum Starten der Kalibrierung.");
      while (Serial.available() == 0) {}
      Serial.read();

      calibrate(mode, corners, runtime);
  } else {
      logger.error("[Color]  Falsche Eingabe");
      return;
  }
}

void ColorSensor::calibrate(char mode, int corners, int runtime) {
  if (mode == 'e') {
    for (int i = 0; i < runtime; i++) {
      calibrate_raw[0] += getRedFrequency();
      calibrate_raw[1] += getGreenFrequency();
      calibrate_raw[2] += getBlueFrequency();
      calibrate_raw[3] += getNoFilter();

      logger.info(("[Color]  " + String(i+1) + "/" + String(runtime) + " | Red: " + String(calibrate_raw[0]) + " | Green: " + String(calibrate_raw[1]) + " | Blue: " + String(calibrate_raw[2]) + " | NoFilter: " + String(calibrate_raw[3])).c_str());
      delay(500);
    }
    for (int i = 0; i < 4; i++) {
      calibrate_off[i] = calibrate_raw[i] / runtime;
    }

    logger.info(("[Color]  Red: " + String(calibrate_off[0]) + " | Green: " + String(calibrate_off[1]) + " | Blue: " + String(calibrate_off[2]) + " | NoFilter: " + String(calibrate_off[3])).c_str());
  } else if (mode == 'm') {
    logger.info("[Color]  Es wird an jeder der Ecken erst Orange, dann Blau gemessen.");
    for (size_t i = 0; i < corners; i++) {
      logger.info(("[Color]  Drücke beliebige Taste zum Starten der Kalibrierung  an der Orangen Linie (" + String(i+1) + "/" + String(corners) + " Kurven).").c_str());
      while (Serial.available() == 0) {}
      Serial.read();

      for (int j = 0; j < runtime; j++) {
        calibrate_orange_raw[i][0] += getRedFrequency();
        calibrate_orange_raw[i][1] += getGreenFrequency();
        calibrate_orange_raw[i][2] += getBlueFrequency();
        calibrate_orange_raw[i][3] += getNoFilter();

        logger.info(("[Color]  " + String(j+1) + "/" + String(runtime) + " | Red: " + String(calibrate_orange_raw[i][0]) + " | Green: " + String(calibrate_orange_raw[i][1]) + " | Blue: " + String(calibrate_orange_raw[i][2]) + " | NoFilter: " + String(calibrate_orange_raw[i][3])).c_str());
        delay(500);
      }

      for (int k = 0; k < 4; k++) {
        calibrate_orange_off[i][k] = calibrate_orange_raw[i][k] / runtime;
      }
      logger.info(("[Color]  Red: " + String(calibrate_orange_off[i][0]) + " | Green: " + String(calibrate_orange_off[i][1]) + " | Blue: " + String(calibrate_orange_off[i][2]) + " | NoFilter: " + String(calibrate_orange_off[i][3])).c_str());
    
      logger.info("[Color]  Drücke beliebige Taste zum Fortführen der Kalibrierung an der Blauen Linie.");
      while (Serial.available() == 0) {}
      Serial.read();

      for (int j = 0; j < runtime; j++) {
        calibrate_blue_raw[i][0] += getRedFrequency();
        calibrate_blue_raw[i][1] += getGreenFrequency();
        calibrate_blue_raw[i][2] += getBlueFrequency();
        calibrate_blue_raw[i][3] += getNoFilter();

        logger.info(("[Color]  " + String(j+1) + "/" + String(runtime) + " | Red: " + String(calibrate_blue_raw[i][0]) + " | Green: " + String(calibrate_blue_raw[i][1]) + " | Blue: " + String(calibrate_blue_raw[i][2]) + " | NoFilter: " + String(calibrate_blue_raw[i][3])).c_str());
        delay(500);
      }

      for (int k = 0; k < 4; k++) {
        calibrate_blue_off[i][k] = calibrate_blue_raw[i][k] / runtime;
      }
      logger.info(("[Color]  Red: " + String(calibrate_orange_off[i][0]) + " | Green: " + String(calibrate_orange_off[i][1]) + " | Blue: " + String(calibrate_orange_off[i][2]) + " | NoFilter: " + String(calibrate_orange_off[i][3])).c_str());
    }
    logger.info("[Color]  Fertig. Ergebnisse:");

    for (size_t i = 0; i < corners; i++) {
      orange_raw[0] += calibrate_orange_off[i][0];
      orange_raw[1] += calibrate_orange_off[i][1];
      orange_raw[2] += calibrate_orange_off[i][2];
      orange_raw[3] += calibrate_orange_off[i][3];
      logger.info(("[Color]  Orange " + String(i+1) + "/" + String(corners) + " | Red: " + String(calibrate_orange_off[i][0]) + " | Green: " + String(calibrate_orange_off[i][1]) + " | Blue: " + String(calibrate_orange_off[i][2]) + " | NoFilter: " + String(calibrate_orange_off[i][3])).c_str());
    }
    for (int i = 0; i < 4; i++) {
      orange_off[i] = orange_raw[i] / corners;
    }
    logger.info(("[Color]  Orange Median | Red: " + String(orange_off[0]) + " | Green: " + String(orange_off[1]) + " | Blue: " + String(orange_off[2]) + " | NoFilter: " + String(orange_off[3])).c_str());

    for (size_t i = 0; i < corners; i++) {
      blue_raw[0] += calibrate_blue_off[i][0];
      blue_raw[1] += calibrate_blue_off[i][1];
      blue_raw[2] += calibrate_blue_off[i][2];
      blue_raw[3] += calibrate_blue_off[i][3];
      logger.info(("[Color]  Blue " + String(i+1) + "/" + String(corners) + " | Red: " + String(calibrate_blue_off[i][0]) + " | Green: " + String(calibrate_blue_off[i][1]) + " | Blue: " + String(calibrate_blue_off[i][2]) + " | NoFilter: " + String(calibrate_blue_off[i][3])).c_str());
    }
    for (int i = 0; i < 4; i++) {
      blue_off[i] = blue_raw[i] / corners;
    }
    logger.info(("[Color]  Blue Median | Red: " + String(blue_off[0]) + " | Green: " + String(blue_off[1]) + " | Blue: " + String(blue_off[2]) + " | NoFilter: " + String(blue_off[3])).c_str());
  }
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