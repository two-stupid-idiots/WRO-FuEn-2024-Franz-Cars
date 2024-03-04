#define ENL1 9
#define IN11 10
#define IN12 11

#define ENL2 6
#define IN21 7
#define IN22 8

void initSerial() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println();
  Serial.println("----Motor----");
}

void initGPIO() {
  pinMode(ENL1, OUTPUT);
  pinMode(IN11, OUTPUT);
  pinMode(IN12, OUTPUT);

  pinMode(ENL2, OUTPUT);
  pinMode(IN21, OUTPUT);
  pinMode(IN22, OUTPUT);

  digitalWrite(ENL2, LOW);
}

//speed must be between -100 and 100
void runMotor(int speed) {
  Serial.println("test");
  analogWrite(ENL2, int(abs(speed)*2.55));

  if (speed >= 0) {
    digitalWrite(IN21, HIGH);
    digitalWrite(IN22, LOW);
  }

  if (speed < 0) {
    digitalWrite(IN21, LOW);
    digitalWrite(IN22, HIGH);
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
