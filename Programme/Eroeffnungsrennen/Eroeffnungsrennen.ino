#define SERVO_ENL_PIN   9
#define SERVO_IN1_PIN  10
#define SERVO_IN2_PIN  11

#define MOTOR_ENL_PIN   6
#define MOTOR_IN1_PIN   7
#define MOTOR_IN2_PIN   8

#define LEFT_TRIG_PIN  13
#define LEFT_ECHO_PIN  12

#define FRONT_TRIG_PIN  5
#define FRONT_ECHO_PIN  4

#define RIGHT_TRIG_PIN  3
#define RIGHT_ECHO_PIN  2

#define LEFT_I    -8
#define LEFT_II  -22
#define LEFT_III -36
#define LEFT_IV  -51
#define LEFT_V   -65
#define LEFT_VI  -79
#define LEFT_VII -93

#define RIGHT_I    8
#define RIGHT_II  22
#define RIGHT_III 36
#define RIGHT_IV  51
#define RIGHT_V   65
#define RIGHT_VI  79
#define RIGHT_VII 93

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

void initGPIO() {
  pinMode(SERVO_ENL_PIN, OUTPUT);
  pinMode(SERVO_IN1_PIN, OUTPUT);
  pinMode(SERVO_IN2_PIN, OUTPUT);

  pinMode(MOTOR_ENL_PIN, OUTPUT);
  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);

  pinMode(LEFT_TRIG_PIN, OUTPUT);
  pinMode(LEFT_ECHO_PIN, INPUT);

  pinMode(FRONT_TRIG_PIN, OUTPUT);
  pinMode(FRONT_ECHO_PIN, INPUT);
  
  pinMode(RIGHT_TRIG_PIN, OUTPUT);
  pinMode(RIGHT_ECHO_PIN, INPUT);
}

void setServo(int angle) {
  if (angle >= 0) {
    digitalWrite(SERVO_IN1_PIN, HIGH);
    digitalWrite(SERVO_IN2_PIN, LOW);
  }

  if (angle < 0) {
    digitalWrite(SERVO_IN1_PIN, LOW);
    digitalWrite(SERVO_IN2_PIN, HIGH);
  }

  if (angle >= -100 && angle <= 100) {
    analogWrite(SERVO_ENL_PIN, int(abs(angle)*2.55));
    if (logLevel >= info) {
      Serial.println("[INFO]  [SERVO] set angle to " + String(angle));
    }
  } else {
    if (logLevel >= error) {
      Serial.println("[ERROR] [SERVO] angle is outside the defined range and could not be set");
    }
  }
}

void runMotor(int speed) {
  if (speed >= 0) {
    digitalWrite(MOTOR_IN1_PIN, HIGH);
    digitalWrite(MOTOR_IN2_PIN, LOW);
  }

  if (speed < 0) {
    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, HIGH);
  }

  if (speed >= -100 && speed <= 100) {
    analogWrite(MOTOR_ENL_PIN, int(abs(speed)*2.55));
    if (logLevel >= info) {
      Serial.println("[INFO]  [MOTOR] set speed to " + String(speed));
    }
  } else {
    if (logLevel >= error) {
      Serial.println("[ERROR] [MOTOR] speed is outside the defined range and could not be set");
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
        Serial.println("[DEBUG] [ULTRA] left distance: " + String(distance) + "cm");
      }
      return distance;
    case front:
      distance = getUltrasonicData(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
      if (logLevel >= debug) {
        Serial.println("[DEBUG] [ULTRA] front distance: " + String(distance) + "cm");
      }
      return distance;
    case right:
      distance = getUltrasonicData(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);
      if (logLevel >= debug) {
        Serial.println("[DEBUG] [ULTRA] right distance: " + String(distance) + "cm");
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
  setServo(LEFT_VII);
  delay(1000);
  setServo(0);
  delay(1000);
  setServo(RIGHT_VII);
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

void test() {
  setServo(RIGHT_VI);
  runMotor(-79);
  delay(1000);
}

void(* resetFunc) (void) = 0;

void setup() {
  logLevel = info;
  initSerial();
  initGPIO();
  Serial.println("----Start----");
}

void loop() {
  test();
}