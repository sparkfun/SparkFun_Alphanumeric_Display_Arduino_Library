/*****************************************************************************************
 * This example tests display options (brightness, blinking) on the 14-segment display.
 * 
 * Priyanka Makin @ SparkFun Electronics
 * Original Creation Date: February 18, 2020
 * 
 * SparkFun labored with love to create this code. Feel like supporting open source hardware?
 * Buy a board from SparkFun! LINK GOES HERE
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
#include <SparkFun_Alphanumeric_Display.h>
HT16K33 display;

void setup() {
  Serial.begin(115200);
  Serial.println("Qwiic Alphanumeric examples");
  Wire.begin(); //Join I2C bus

  //check if display will acknowledge
  if (display.begin() == false)
  {
    Serial.println("Device did not acknowledge! Freezing.");
    while(1);
  }
  Serial.println("Display acknowledged.");
  
  //Input to setBrightness is duty cycle over 16
  display.setBrightness(10);
  //Blink rate in Hz
  //Acceptable options are defined by HT16K33 datasheet and are 0.5, 1, or 2 Hz
  display.setBlinkRate(2);
  
  display.print("Milk");
}

void loop() {
  
}
