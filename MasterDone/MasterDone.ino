/*
 * Author: Phiet Do
 * Date: 12/4/2016
 * Purpose: This code is used for the Master Arduino device to obtain strain and acceleration values.
 * The code will obtain strain values by packets/buffers using I2C protocol from the slave Arduino.
 * The MPU6050 is another slave device that will transfer acceleration values using I2C protocol as well.
 * The HX711 is the ADC and amplifier used to measure the resistive strain gauge.
 * The MPU6050 is the other sensor needed to obtain the acceleration which is further used to calculate G-Force.
 * 
 * All HX711 library credit goes to bogde
 * Library can be found at https://github.com/bogde/HX711
 * 
 *  All MPU6050 library credit goes to Jeff Rowberg <jeff@rowberg.net>
 *  Library can be found at https://github.com/jrowberg/i2cdevlib
 *  
 *  Libraries needed in this design:
 *  Wire and I2Cdev: Needed to perfrom I2C communication from Slave Arduino and MPU6050 to Master Arduino
 *  MPU6050: Needed to interface and obtain acceleration and other values from the MPU6050.
*/
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro; // Create object for MPU6050
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az; // create variables that will hold the 3-axis acceleration values
float axg, ayg, azg; // variables that will hold the g-force of each acceleration value

#define OUTPUT_READABLE_ACCELGYRO // define MPU6050's acceleration functions used later

char datainfo[15]={};//empty array that will hold the values coming from the slave Arduino
String strain; // String that will hold the transferred strain from the slave
float strainval; // Variable needed to change the string to a quantifiable measurement, float

const int ledPinStrain = 13; // define pin to light up LED reacting based on strain.
const int ledPinXForce = 12; // define pin to light up LED reacting based on the G-Force in the x-axis
const int ledPinZForce = 11; // define pin to light up LED reacting based on the G-Force in the z-axis

// NOTE: y-axis is not needed for our design.

void setup() {
  //Initialize all the output pins that will be in use
  pinMode(ledPinStrain,OUTPUT);
  pinMode(ledPinXForce,OUTPUT);
  pinMode(ledPinZForce,OUTPUT);
  
  Wire.begin();        // initialize the i2c bus
  Serial.begin(38400);  // start serial for output

  // initialize the MPU6050 device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
  Wire.requestFrom(8, 7);    // request 7 bytes from slave arduino 8. The 7 bytes is the greatest size of the strain value.
  
  int i=0; //counter for each byte
  // While loop to go through each byte that has been sent through the I2C protocol and obtain the data to put in a char array.
  while (Wire.available()) {
    datainfo[i] = Wire.read(); // append every character or byte transferred into an empty char array
    i=i+1;
  }

  // Similar to the above while loop but the transferred information is now being sent to String instead of a char array
  int j = 0;
  while(datainfo[j] != '\0'){
    strain += datainfo[j];
   j=j+1;
  }

  //The if statement is needed because on occasion, no values are sent through I2C and we do not want to waste our time printing nothing
  if(strain.length()!= 0){
    Serial.println("------------------------------------------------------");
    
    //Serial.print("original value\t"); //only needed if you want to check if original info ha been sent.
    //Serial.println(datainfo);   //shows the data in the char array 
    //Serial.print("strain value\t"); //prints out the strain in string form; not really a difference from char array
    //Serial.println(strain);   
    strainval = strain.toFloat(); // Convert string to float; This is why we needed the strain variable to be a String.
    Serial.print("strain  value\t"); // prints out the float strain value
    Serial.println(strainval);   
    
    accelgyro.getAcceleration(&ax, &ay, &az); // prints out the raw acceleration values from the MPU6050
    //accelgyro.getRotation(&gx, &gy, &gz); // prints out the raw gyro values from the MPU6050; You must declare these variables above.

    // display tab-separated acceleration x/y/z values
    Serial.print("acceleration:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.println("\t");

    //16384 for 2gs, 8192 for 4gs, 4096 for 8gs, 2048 for 16gs
    //Typecast each integer to floats and calculate the G-Force of each axis
    axg = (float)ax/8192; 
    ayg = (float)ay/8192;
    azg = (float)az/8192;
    Serial.print("X,Y,Z G-Force: ");
    Serial.print(axg); Serial.print("\t");
    Serial.print(ayg); Serial.print("\t");
    Serial.println(azg);
    
  }

  //These are used to turn on the 3 LEDs based on certain situaton.

  //If the amount of grams greater than 300, then turn on the LED.
  if (strainval > 300.00){
    digitalWrite(ledPinStrain, HIGH);
  }
  else{
    digitalWrite(ledPinStrain,LOW);
  }
  //If the g-force in the x-axis is greater than 1, then turn on the LED.
  if (axg > 1.00){
    digitalWrite(ledPinXForce, HIGH);
    //delay(100);
  }
  else{
    digitalWrite(ledPinXForce,LOW);
  }
  //If the g-force in the z-axis is greater than 1, then turn on the LED.
  if (azg > 1.00){
    digitalWrite(ledPinZForce, HIGH);
    //delay(100);
  }
  else{
    digitalWrite(ledPinZForce,LOW);
  }

  //After the values have been displayed, processed, or saved, then clear the values to reset for the next set.
  strain = "";
  memset(datainfo, 0, sizeof datainfo);
  delay(1250); // wait for a little to process another set of values.
}
