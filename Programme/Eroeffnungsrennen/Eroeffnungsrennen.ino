#include "ColorSensor.h"
#include "Logger.h"
#include "MotorController.h"
#include "MPU.h"
#include "ServoController.h"
#include "UltraschallSensor.h"

#define BUTTON_PIN 1

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

void run() {
  motor.run(100);

  if (ir.getDistance(side::left) < ir.getDistance(side::right)) {
    servo.set(-42);
  }

  if (ir.getDistance(side::left) > ir.getDistance(side::right)) {
    servo.set(42);
  }

  if (ir.getDistance(side::front) < ir.getDistance(side::right)) {
    servo.set(-84);
  }
  

  if (ir.getDistance(side::front) < ir.getDistance(side::left)) {
    servo.set(84);
  }
}

void setup() {
  initSerial();
  logger.init(LogLevel::INFO);
  color.init();
  motor.init();
  mpu.init();
  servo.init();
  ir.init();
  servo.set(0);
}

void loop() {
  color.test();
}
