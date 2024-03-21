#define ENL  6
#define IN1  7
#define IN2  8

void initSerial() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println();
  Serial.println("----Motor----");
}

void initGPIO() {
  pinMode(ENL, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(ENL, LOW);
}

//speed must be between -100 and 100
void runMotor(int speed) {
  analogWrite(ENL, int(abs(speed)*2.55));

  if (speed >= 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }

  if (speed < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
}

void test() {
  runMotor(100);
  delay(3000);
  runMotor(-100);
  delay(3000);
}

void setup() {
  initSerial();
  initGPIO();
  runMotor(0);
}

void loop() {
  //test();
}
