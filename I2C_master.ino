#include <Wire.h>
int volts;
void setup() {
  Wire.begin();
  Serial.begin(115200);
   
}

void loop() {
  Wire.beginTransmission(9);
  int avail = Wire.requestFrom(9,2);

  if (avail == 2){
    volts = (Wire.read() << 8) | (Wire.read());
    Serial.println(volts);
  }
  Wire.endTransmission();
  delay(500);
  
}
