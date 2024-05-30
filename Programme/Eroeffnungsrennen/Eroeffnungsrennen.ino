#include "ColorSensor.h"
#include "Logger.h"
#include "MotorController.h"
#include "MPU.h"
#include "ServoController.h"
#include "UltraschallSensor.h"

#define BUTTON_PIN 1
dir direction;

void initSerial() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println("----Start----");
}

void wait() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  while (digitalRead(BUTTON_PIN) == HIGH) {
    delay(100);
  }
}

void start() {
  direction = color.isLine();
  while (direction == dir::null) {
    motor.run(80);
    direction = color.isLine();
  }
  motor.run(0);
  if (direction == dir::right) {
    logger.debug("Direction: right");
  }
  if (direction == dir::left) {
    logger.debug("Direction: left");
  }
  delay(1000);
}

void run() {
  motor.run(80);

  if (ir.getDistance(side::left) < ir.getDistance(side::right)) {
    servo.set(-42);
  }

  if (ir.getDistance(side::left) > ir.getDistance(side::right)) {
    servo.set(42);
  }

  if (direction == dir::right && ir.getDistance(side::front) < ir.getDistance(side::right)) {
    servo.set(-84);
  }

  if (direction == dir::left && ir.getDistance(side::front) < ir.getDistance(side::left)) {
    servo.set(84);
  }
}

void setup() {
  initSerial();
  logger.init(LogLevel::DEBUG);
  color.init();
  motor.init();
  //mpu.init();
  servo.init();
  ir.init();
  servo.set(0);
  //start();
  color.calibrate();
}

void loop() {
  // run();
  //color.test();
}
