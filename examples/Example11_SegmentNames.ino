
/*******************************************************************************************
 * This is based on the SparkFun Example 2 for the Spark-X
 * alphanumeric display.
 * This version cycles through the names, displaying a name
 * and illuminating the corresponding segment on the next
 * digit.  Since the displays have 4 digits, we can show two
 * at a time.
 * Hardware Connections:
 * 
 * Attach Red Board to computer using micro-B USB cable.
 * Attach Qwiic Alphanumeric board to Red Board using Qwiic cable.
 * 
 * I wrote this for my own use, but have decided to release
 * it as "user contributed" to the general community.
 * 
 * Written by Clark Jones, and released as "beerware", as-is,
 * with abolutely no guarantees.
 *****************************************************************************************/
#include <Wire.h>

#include <SparkFun_Alphanumeric_Display.h>  //Click here to get the library: http://librarymanager/All#Alphanumeric_Display by SparkFun
HT16K33 display;

uint8_t currChar;
unsigned long nextDisplay;

void setup()
{
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

  currChar = 'A';
  nextDisplay = 0;
}

void loop()
{
  unsigned long nowTime;
  nowTime = millis();
  // Rather than using the millis() value, delay(2000) could have
  // been used, but if other things were going on, this allows
  // for them to happen, thus being better "style".
  if ( nowTime >= nextDisplay ) {
    nextDisplay = nowTime + 2000;
    display.clear();
    display.printChar(currChar, 0);
    display.illuminateSegment(currChar, 1);
    Serial.print("Displaying segment ");
    Serial.print(currChar);
    currChar++;
    display.printChar(currChar, 2);
    display.illuminateSegment(currChar,3);
    Serial.print(" and segment ");
    Serial.println(currChar);
    currChar++;
    display.updateDisplay();
    if ( currChar > 'N' )
      currChar = 'A'; // Cycle around
  }; // nowTime >= nextDisplay
}
