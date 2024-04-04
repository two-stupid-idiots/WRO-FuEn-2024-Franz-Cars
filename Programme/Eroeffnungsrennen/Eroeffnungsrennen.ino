#include "Logger.h"
#include "MotorController.h"
#include "ServoController.h"
#include "UltraschallSensor.h"

#define BUTTON_PIN 1

void initSerial() {
  Serial.begin(9600);
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
  while (ir.getDistance(side::front) > ir.getDistance(side::left) && ir.getDistance(side::front) > ir.getDistance(side::right)) {
    motor.run(75);
    if (ir.getDistance(side::left) < ir.getDistance(side::right)) {
      //steer right
      servo.set(-42);
    }
    if (ir.getDistance(side::right) < ir.getDistance(side::left)) {
      //steer left
      servo.set(42);
    }
  }
  
  if (ir.getDistance(side::front) < ir.getDistance(side::left)) {
    //turn left
  }
  if (ir.getDistance(side::front) < ir.getDistance(side::right)) {
    //turn right
  }
}

void setup() {
  initSerial();
  logger.setLevel(LogLevel::DEBUG);
  motor.init();
  servo.init();
  ir.init();
  servo.set(0);
}

void loop() {
  
}