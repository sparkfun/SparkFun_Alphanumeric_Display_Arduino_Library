/*****************************************************************************************
 * This example tests the library's response to printing colons or decimal points.
 * 
 * Priyanka Makin @ SparkFun Electronics
 * Original Creation Date: February 3, 2020
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
 ****************************************************************************************/
#include <Wire.h>

#include <SparkFun_Alphanumeric_Display.h>  //Click here to get the library:
HT16K33 display;

void setup(){
  Serial.begin(115200);
  Serial.println("Qwiic Alphanumeric examples");

  Wire.begin();

  if (display.begin(0x70, 0x71) == false)
  {
    Serial.println("Device did not acknowledge! Freezing.");
    while(1);
  }
  Serial.println("Display acknowledged.");

  display.print("12345678");
  display.decimalOff();
}

void loop()
{
}
