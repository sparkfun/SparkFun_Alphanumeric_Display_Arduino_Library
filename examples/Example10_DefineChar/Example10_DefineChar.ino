/**************************************************************************************
 * This example redefines some characters of an alphanumeric display.
 * 
 * Written by Gaston Williams
 * April 30, 2020
 * 
 * Based on code written by
 * Priyanka Makin @ SparkFun Electronics
 * Original Creation Date: February 26, 2020
 * 
 * SparkFun labored with love to create this code. Feel like supporting open source hardware?
 * Buy a board from SparkFun! https://www.sparkfun.com/products/16426
 * 
 * This code is beerware; if you see me (or any other SparkFun employee) at the
 * local, and you've found our code helpful, please buy us a round!
 * 
 * Hardware Connections:
 * Attach Red Board to computer using micro-B USB cable.
 * Attach Qwiic Alphanumeric board to Red Board using Qwiic cable. 
 *  Don't close any of the address jumpers so that it defaults to address 0x70.
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
  
  //Just for demo purposes, show original characters before change
  display.print("cafe");
  delay(500);
  display.print("size");
  

  //Update a, e, f, s and z to new characters
  //This change is not permanent, and lasts only for this program.
  
  //Define 14 segment bits: nmlkjihgfedcba
  display.defineChar('a', 0b01000001011000);                              
  display.defineChar('e', 0b10000001011000);
  display.defineChar('f', 0b01010101000000);
  //Also can use constants SEG_A - SEG_N to define characters
  display.defineChar('s', SEG_L | SEG_I | SEG_D); // 0b00100100001000  
  display.defineChar('z', SEG_N | SEG_G | SEG_D); // 0b10000001001000
}

void loop() 
{ 
  //Show the new characters
  delay(500);
  display.print("cafe");
  delay(500);
  display.print("size");
}
