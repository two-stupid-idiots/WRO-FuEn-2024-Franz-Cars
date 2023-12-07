void initGIPO() {
  for (int i=0; i <= 13; i++) {
    pinMode(i, OUTPUT);
    Serial.println("Set PIN " + String(i) + " to OUTPUT.");
  }
}

void testPIN(int pin) {
  Serial.println("PIN: " + String(pin));
  Serial.println("- HIGH");
  digitalWrite(pin, HIGH);
  delay(3000);
  Serial.println("- LOW");
  digitalWrite(pin, LOW);
  delay(3000);
  Serial.println("Done");
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("----Start Check----");
  initGIPO();
  for (int i=0; i <= 13; i++) {
    testPIN(i);
    delay(5000);
  }
  Serial.println("----Fertig----");
}

void loop() {
  
}
