#include <Servo.h>

#define SERVO_PIN 8
Servo servo;

#define LEFT_TRIG_PIN 3
#define LEFT_ECHO_PIN 2

#define FRONT_TRIG_PIN 5
#define FRONT_ECHO_PIN 4

#define RIGHT_TRIG_PIN 7
#define RIGHT_ECHO_PIN 6

enum side {
  left,
  front,
  right
};

void initSerial() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("----Eröffnungsrennen----");
}

void initServo() {
  servo.attach(SERVO_PIN);
  servo.write(90);
}

void initGPIO() {
  pinMode(LEFT_TRIG_PIN, OUTPUT);
  pinMode(LEFT_ECHO_PIN, INPUT);

  pinMode(FRONT_TRIG_PIN, OUTPUT);
  pinMode(FRONT_ECHO_PIN, INPUT);
  
  pinMode(RIGHT_TRIG_PIN, OUTPUT);
  pinMode(RIGHT_ECHO_PIN, INPUT);
}

void setServo(int angle) {
  servo.write(angle);
  Serial.println("Set servo to " + String(angle));
}

int getUltrasonicData(int TRIG_PIN, int ECHO_PIN) {
  unsigned int i = 0;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  i = ((unsigned int)pulseIn(ECHO_PIN, HIGH) / 58);
  return i;
}

void getDistance(side side) {
  int distance;
  switch (side) {
    case left:
      distance = getUltrasonicData(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
      Serial.println("Left distance: " + String(distance) + "cm");
      break;
    case front:
      distance = getUltrasonicData(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
      Serial.println("Front distance: " + String(distance) + "cm");
      break;
    case right:
      distance = getUltrasonicData(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);
      Serial.println("Right distance: " + String(distance) + "cm");
      break;
  }
}

void getLeftDistance() {
  getDistance(left);
}

void getFrontDistance() {
  getDistance(front);
}

void getRightDistance() {
  getDistance(right);
}

void testUltrasonicSensor() {
  getLeftDistance();
  getFrontDistance();
  getRightDistance();
  delay(1000);
  Serial.println("--------");
}

void testServo() {
  for (int i = 0; i <= 180; i += 10) {
    servo.write(i);
    Serial.println("Position: "+ String(i));
  }
  delay(1000);
  for (int i = 180; i >= 0; i -= 10) {
    servo.write(i);
    Serial.println("Position: "+ String(i));
  }
  delay(1000);
}

void setup() {
  initSerial();
  initGPIO();
  initServo();
}

void loop() {
  
}
