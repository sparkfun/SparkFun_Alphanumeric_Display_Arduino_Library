/*****************************************************************************************
 * This example tests the library's response to printing an unknown character.
 * 
 * Priyanka Makin @ SparkFun Electronics
 * Original Creation Date: March 13, 2020
 * Updated April 30, 2020 by Gaston Williams - changed exclamation to tab character
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
 ****************************************************************************************/
#include <Wire.h>

#include <SparkFun_Alphanumeric_Display.h>  //Click here to get the library: http://librarymanager/All#SparkFun_Qwiic_Alphanumeric_Display by SparkFun
HT16K33 display;

void setup() {
  Serial.begin(115200);
  Serial.println("SparkFun Qwiic Alphanumeric - Example 7: Unknown Char");

  Wire.begin();

  if (display.begin() == false)
  {
    Serial.println("Device did not acknowledge! Freezing.");
    while(1);
  }
  Serial.println("Display acknowledged.");

  //Because this is a character unknown to the library, expect the display
  //To turn on all segments for that unknown digit
  display.print("\t\t\t\t");  //tabs are not printable characters
}

void loop() 
{
}
