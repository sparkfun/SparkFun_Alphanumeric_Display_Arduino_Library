/****************************************************************
 * This sketch will run through the poossible values that the
 * Spark-X Alphanumeric Displays (SparkFun SPX-16391, SPX-16425,
 * SPX-16426, and SPX-16427) can display, so that the user can
 * easily see an example of each letter.
 * 
 * The first two digits will be the hexadecimal ASCII value, and
 * the fourth digit will actually display the character.
 * 
 * Furthermore, a message is sent to the serial monitor stating
 * what character is being displayed.
 * 
 * I wrote this for my own use, but have decided to release
 * it as "user contributed" to the general community.
 * 
 * Written by Clark Jones, and released as "beerware", as-is,
 * with abolutely no guarantees.
 */

#include <Wire.h>
#include <SparkFun_Alphanumeric_Display.h>

HT16K33 display;

unsigned long nextDisplayTime;

uint8_t showingChar;

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

  nextDisplayTime = 0;
  showingChar = '!';
}

void loop() {
  unsigned long nowTime;
  nowTime = millis();
  if ( nowTime >= nextDisplayTime ) {
    nextDisplayTime = nowTime + 2000;
    display.clear();
    char hexUpper, hexLower;
    byte temp;
    temp = showingChar >> 4; // faster than dividing by 16
    if ( temp <= 9 ) {
      hexUpper = '0' + temp;
    } else {
      hexUpper = 'A' + temp - 10;
    };
    temp = showingChar & 0x0F;
    if ( temp <= 9 ) {
      hexLower = '0' + temp;
    } else {
      hexLower = 'A' + temp - 10;
    };
    display.printChar( hexUpper, 0 );
    display.printChar( hexLower, 1 );
    display.printChar( (char)showingChar, 3 );
    display.updateDisplay();
    Serial.print( "Showing character '" );
    Serial.print( (char)showingChar );
    Serial.print( "' (0x" );
    Serial.print( hexUpper );
    Serial.print( hexLower );
    Serial.println( ")" );
    showingChar++;
    if ( showingChar > '~' ) {
      showingChar = '!';
    }

  }; // nowTime >= nextDisplayTime

}
