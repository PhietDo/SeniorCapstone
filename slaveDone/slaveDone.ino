/*
 * Author: Phiet Do
 * Date: 12/4/2016
 * Purpose: This code is used for the Slave Arduino device to calculate the strain using the HX711 module.
 * The code will obtain strain values and send packets/buffers using I2C protocol to the master Arduino device. 
 * (Not used in this code) The MPU6050 is another slave device that will transfer acceleration values using I2C protocol as well.
 * The HX711 is the ADC and amplifier used to measure the resistive strain gauge.
 * (Not used in this code) The MPU6050 is the other sensor needed to obtain the acceleration which is further used to calculate G-Force.
 * 
 * All HX711 library credit goes to bogde
 * Library can be found at https://github.com/bogde/HX711
 * 
 * All MPU6050 library credit goes to Jeff Rowberg <jeff@rowberg.net>
 * Library can be found at https://github.com/jrowberg/i2cdevlib
 *  
 * Libraries needed in this design:
 * Wire and I2Cdev: Needed to perfrom I2C communication from Slave Arduino and MPU6050 to Master Arduino
 * HX711: needed to interface with the HX711 values. Does the conversion from bits to grams but can easily obtain readings from 0 to 2^24 bits.
*/

#include <Wire.h>
#include "HX711.h"

char strain[7]; //empty array that will hold values to send to master Arduino

HX711 scale; // create an HX711 object
float fvalue; // used to obtain the one strain/gram reading
float value; // used to obtain the average strain/gram reading

void setup() {
  Wire.begin(8);                // Slave id #8
  Wire.onRequest(requestEvent); // Function to run when asking for data
  Serial.begin(38400);  // Initialize serial monitor

  Serial.println("Initializing HX711...");
  // parameter "gain" is ommited; the default value 128 is used by the library
  // HX711.DOUT  - pin #A1
  // HX711.PD_SCK - pin #A0
  scale.begin(A1, A0);
  scale.set_scale(2280.f);     // this value is obtained by calibrating the scale with known weights;
  scale.tare();                // reset the scale to 0
}

void loop() {
  fvalue = scale.get_units(),1; // obtain a strain/gram reading
  value = scale.get_units(10),1; // obtain an average strain/gram reading based on 10 readings
  Serial.print("one reading:\t");
  Serial.print(fvalue);
  Serial.print("\t| average:\t");
  Serial.println(value);
  dtostrf(value, 7, 2, strain); //converts the float or integer to a string. (floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, empty array);
  delay(500);
}

// function: to send data using I2C protocol
void requestEvent() {
  Wire.write(strain); 
  memset(strain, 0, sizeof strain);
}

