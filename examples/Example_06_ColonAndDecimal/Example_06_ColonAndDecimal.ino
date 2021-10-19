/*****************************************************************************************
 * This example tests the library's response to printing colons or decimal points.
 * 
 * Priyanka Makin @ SparkFun Electronics
 * Original Creation Date: February 3, 2020
 * 
 * SparkFun labored with love to create this code. Feel like supporting open source hardware?
 * Buy a board from SparkFun!https://www.sparkfun.com/products/16391
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

void setup(){
  Serial.begin(115200);
  Serial.println("SparkFun Qwiic Alphanumeric - Example 6: Colon And Decimal");

  Wire.begin();

  if (display.begin() == false)
  {
    Serial.println("Device did not acknowledge! Freezing.");
    while(1);
  }
  Serial.println("Display acknowledged.");

  //You can print colons and decimals
  //NOTE: they can only go in the character position determined by the layout of the display
  display.print("12:3.4");
  
//  display.decimalOn();  //Turn decimals on
//  display.decimalOff();   //Turn decimals off
//  display.decimalOnSingle(0); //Turn decimal on for one display
//  display.colonOn();      //Turn colons on
//  display.colonOff();     //Turn colons off
//  display.colonOnSingle(1); //Turn colon on for one display
}

void loop()
{
}
