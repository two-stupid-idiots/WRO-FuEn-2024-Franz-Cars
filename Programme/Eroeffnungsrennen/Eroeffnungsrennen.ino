#include "ColorSensor.h"
#include "Logger.h"
#include "MotorController.h"
#include "MPU.h"
#include "ServoController.h"
#include "UltraschallSensor.h"

#define ORANGE_PIN 34
#define BLUE_PIN 36
#define WHITE_PIN 38
#define GREEN_PIN 40
#define BUTTON_PIN 32
boolean isWhiteLed;
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
  digitalWrite(GREEN_PIN, HIGH);
}

void start() {
  pinMode(ORANGE_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(WHITE_PIN, OUTPUT);

  direction = color.isLine();
  while (direction == dir::null) {
    motor.run(70);
    direction = color.isLine();
  }
  motor.run(0);
  if (direction == dir::right) {
    digitalWrite(ORANGE_PIN, HIGH);
    logger.debug("[START]  Direction: right");
    servo.set(-70);
    motor.run(50);
    delay(1000);
    servo.set(-84);
    motor.run(50);
    delay(1000);
  }
  if (direction == dir::left) {
    digitalWrite(BLUE_PIN, HIGH);
    logger.debug("[START]  Direction: left");
    servo.set(70);
    motor.run(50);
    delay(1000);
    servo.set(84);
    motor.run(50);
    delay(1000);
  }
  motor.run(0);
  servo.set(0);
}
 
void run() {
  motor.run(50);
  if (isWhiteLed) {
    digitalWrite(WHITE_PIN, LOW);
  } else {
    digitalWrite(WHITE_PIN, HIGH);
  }
 
  if (ir.getDistance(side::front) < 15) {
    motor.run(-50);
    if (direction == dir::right) {
      servo.set(38);
    } else {
      servo.set(-38);
    }
    delay(1000);
    motor.run(50);
  }
  

  if (ir.getDistance(side::left) < ir.getDistance(side::right)) {
    servo.set(-24);
  }

  if (ir.getDistance(side::left) > ir.getDistance(side::right)) {
    servo.set(38);
  }

  if (direction == dir::right && ir.getDistance(side::front) < ir.getDistance(side::right) && ir.getDistance(side::left) < ir.getDistance(side::right)) {
    servo.set(-84);
  }

  if (direction == dir::left && ir.getDistance(side::front) < ir.getDistance(side::left) && ir.getDistance(side::right) < ir.getDistance(side::left)) {
    servo.set(84);
  }
}

void setup() {
  initSerial();
  logger.init(LogLevel::INFO);
  color.init();
  motor.init();
  //mpu.init();
  servo.init();
  ir.init();
  servo.set(0);
  wait();
  start();
  //color.startCalibrate();
}

void loop() {
  run();
}
