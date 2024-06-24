/**************************************************************************************
 * This example tests scrolling functionality of alphanumeric displays.
 * 
 * Priyanka Makin @ SparkFun Electronics
 * Original Creation Date: February 26, 2020
 * 
 * SparkFun labored with love to create this code. Feel like supporting open source hardware?
 * Buy a board from SparkFun! https://www.sparkfun.com/products/16391
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2024 SparkFun Electronics
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions: The
 * above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
 * "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Hardware Connections:
 * Attach Red Board to computer using micro-B USB cable.
 * Attach Qwiic Alphanumeric board to Red Board using Qwiic cable. 
 *  Don't close any of the address jumpers so that it defaults to address 0x70.
 * Attach a second Alphanumeric display using Qwiic cable.
 *  Close address jumper A0 so that this display's address become 0x71.
 * 
 * Distributed as-is; no warranty is given.
 *****************************************************************************************/
#include <Wire.h>

#include <SparkFun_Alphanumeric_Display.h>  //Click here to get the library: http://librarymanager/All#SparkFun_Qwiic_Alphanumeric_Display by SparkFun
HT16K33 display;

void setup() {
  Serial.begin(115200);
  Serial.println("SparkFun Qwiic Alphanumeric - Example 9: Scrolling String");
  Wire.begin(); //Join I2C bus

  //check if displays will acknowledge
  if (display.begin(0x70) == false)
  {
    Serial.println("Device did not acknowledge! Freezing.");
    while(1);
  }
  Serial.println("Displays acknowledged.");

  display.print("MILK");
}

void loop() 
{  
  delay(300);
  display.shiftLeft();
  //Alternatively - you could also shift the string to the right
  //display.shiftRight();
}
