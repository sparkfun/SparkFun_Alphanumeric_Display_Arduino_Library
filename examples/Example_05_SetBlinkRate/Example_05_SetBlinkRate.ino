/*****************************************************************************************
 * This example sets the blink rate of the Alphanumeric Display
 * 
 * Priyanka Makin @ SparkFun Electronics
 * Original Creation Date: February 18, 2020
 * 
 * SparkFun labored with love to create this code. Feel like supporting open source hardware?
 * Buy a board from SparkFun! https://www.sparkfun.com/products/16391
 * 
 * This code is Lemonadeware; if you see me (or any other SparkFun employee) at the 
 * local, and you've found our code helpful, please buy us a round!
 * 
 * Hardware Connections:
 * Attach Red Board to computer using micro-B USB cable.
 * Attach Qwiic Alphanumeric board to Red Board using Qwiic cable.
 * 
 * Distributed as-is; no warranty is given.
 *****************************************************************************************/
#include <Wire.h>

#include <SparkFun_Alphanumeric_Display.h>  //Click here to get the library: http://librarymanager/All#SparkFun_Qwiic_Alphanumeric_Display by SparkFun
HT16K33 display;

void setup() {
  Serial.begin(115200);
  Serial.println("SparkFun Qwiic Alphanumeric - Example 5: Set Blink Rate");
  Wire.begin(); //Join I2C bus

  //check if display will acknowledge
  if (display.begin() == false)
  {
    Serial.println("Device did not acknowledge! Freezing.");
    while(1);
  }
  Serial.println("Display acknowledged.");

  //Blink rate in Hz
  //Acceptable options are defined by HT16K33 datasheet and are 0.5, 1.0, or 2.0 Hz (float)
  display.setBlinkRate(2.0);
  
  display.print("Milk");
}

void loop() {
  
}
