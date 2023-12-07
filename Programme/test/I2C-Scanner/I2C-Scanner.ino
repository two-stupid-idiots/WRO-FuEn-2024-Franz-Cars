#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while(!Serial);
  Serial.println("----I2C Scanner----");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning...");

  for(address = 0; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    Wire.beginTransmission(address+1);
       
    if(error == 0 && Wire.endTransmission() != 0) {
      Serial.print("I2C Device found at address 0x");
      if(address < 16)
        Serial.print("0");
      Serial.println(address,HEX);
      nDevices++;
    }
      else if(error == 4) {
        Serial.print("Unkown error at address 0x");
        if(address < 16)
          Serial.print("0");
        Serial.println(address,HEX);
    }
  }
  if(nDevices == 0)
    Serial.println("No I2C device found");
  else
    Serial.println("done");
        
  delay(5000);
}