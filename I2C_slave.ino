#include <Wire.h>
int volts;
byte a[2];
void setup(){

  Wire.begin(9);
  Wire.onRequest(requestEvent);
}


void loop(){

}

void requestEvent(){
  volts = analogRead(0);
  a[0] = highByte(volts);
  a[1] = lowByte(volts);
  Wire.write(a,2);
}
