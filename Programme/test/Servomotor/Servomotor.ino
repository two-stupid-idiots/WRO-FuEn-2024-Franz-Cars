#define ENL  9
#define IN1 10
#define IN2 11

void initSerial() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println();
  Serial.println("----Servo----");
}

void initGPIO() {
  pinMode(ENL, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(ENL, LOW);
}

//angle must be between -100 and 100
void setServo(int angle) {
  analogWrite(ENL, int(abs(angle)*2.55));

  if (angle >= 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }

  if (angle < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
}

void test() {
  setServo(75);
  delay(3000);
  setServo(-75);
  delay(3000);
}

void setup() {
  initSerial();
  initGPIO();
  setServo(0);
}

void loop() {
  test();
}
