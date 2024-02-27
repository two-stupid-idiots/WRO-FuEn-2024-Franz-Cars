#include <Servo.h>

#define SERVO_PIN 8
Servo servo;

#define MOTOR_ENL_PIN 9
#define MOTOR_OUT1_PIN 10
#define MOTOR_OUT2_PIN 11

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

enum logLevel {
  fatal,
  error,
  warn,
  info,
  debug,
  trace
};

logLevel logLevel;

void initSerial() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println();
  Serial.println("----Eröffnungsrennen----");
}

void initServo() {
  servo.attach(SERVO_PIN);
  setServo(90);
}

void initGPIO() {
  pinMode(MOTOR_ENL_PIN, OUTPUT);
  pinMode(MOTOR_OUT1_PIN, OUTPUT);
  pinMode(MOTOR_OUT2_PIN, OUTPUT);

  pinMode(LEFT_TRIG_PIN, OUTPUT);
  pinMode(LEFT_ECHO_PIN, INPUT);

  pinMode(FRONT_TRIG_PIN, OUTPUT);
  pinMode(FRONT_ECHO_PIN, INPUT);
  
  pinMode(RIGHT_TRIG_PIN, OUTPUT);
  pinMode(RIGHT_ECHO_PIN, INPUT);
}

void setServo(int angle) {
  servo.write(angle);
  if (logLevel >= info) {
    Serial.println("[INFO]  set servo to " + String(angle) + "°");
  }
}

void runMotor(int speed) {
  if (speed >= 0) {
    digitalWrite(MOTOR_OUT1_PIN, HIGH);
    digitalWrite(MOTOR_OUT2_PIN, LOW);
  }

  if (speed < 0) {
    digitalWrite(MOTOR_OUT1_PIN, LOW);
    digitalWrite(MOTOR_OUT2_PIN, HIGH);
  }

  if (speed >= -100 && speed <= 100) {
    analogWrite(MOTOR_ENL_PIN, int(abs(speed)*2.55));
    if (logLevel >= info) {
      Serial.println("[INFO]  set motor speed to " + String(speed));
    }
  } else {
    if (logLevel >= error) {
      Serial.println("[ERROR] speed is outside the defined range and could not be set");
    }
  }
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

int getDistance(side side) {
  int distance;
  switch (side) {
    case left:
      distance = getUltrasonicData(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
      if (logLevel >= debug) {
        Serial.println("[DEBUG] left distance: " + String(distance) + "cm");
      }
      return distance;
    case front:
      distance = getUltrasonicData(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
      if (logLevel >= debug) {
        Serial.println("[DEBUG] front distance: " + String(distance) + "cm");
      }
      return distance;
    case right:
      distance = getUltrasonicData(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);
      if (logLevel >= debug) {
        Serial.println("[DEBUG] right distance: " + String(distance) + "cm");
      }
      return distance;
  }
}

void testUltrasonicSensor() {
  logLevel = debug;
  getDistance(left);
  getDistance(front);
  getDistance(right);
  delay(1000);
  if (logLevel >= debug) {
    Serial.println("--------");
  }
}

void testServo() {
  logLevel = debug;
  for (int i = 0; i <= 180; i += 10) {
    setServo(i);
    delay(500);
  }
  delay(1000);
  for (int i = 170; i >= 0; i -= 10) {
    setServo(i);
    delay(500);
  }
  delay(1000);
  if (logLevel >= debug) {
    Serial.println("--------");
  }
}

void testMotor() {
  logLevel = debug;
  for (int i = 0; i <= 100; i += 25) {
    runMotor(i);
    delay(1000);
  }
  for (int i = 75; i >= -100; i -= 25) {
    runMotor(i);
    delay(1000);
  }
  for (int i = -75; i <= 0; i += 25) {
    runMotor(i);
    delay(1000);
  }
  if (logLevel >= debug) {
    Serial.println("--------");
  }
}

void(* resetFunc) (void) = 0;

void setup() {
  logLevel = info;
  initSerial();
  initGPIO();
  initServo();
  Serial.println("----start loop----");
}

void loop() {
  
}