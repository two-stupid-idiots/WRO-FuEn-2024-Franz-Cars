#define TRIG_PIN 13
#define ECHO_PIN 12

void initGIPO() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void get() {
  unsigned int i = 0;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  i = ((unsigned int)pulseIn(ECHO_PIN, HIGH) / 58);
  Serial.print(i);
  Serial.println("cm");
  delay(2000);
}

void setup() {
  Serial.begin(9600);
  initGIPO();
  Serial.println("----Ultraschallsenor----");
}

void loop() {
  get();
}
