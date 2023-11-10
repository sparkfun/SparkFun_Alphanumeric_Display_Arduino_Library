/***************************************************************
 * This is intended as a "quick functionality test" of the
 * Spark-X Alphanumeric Displays (SparkFun SPX-16391, SPX-16425,
 * SPX-16426, and SPX-16427).
 * 
 * The sketch will illuminate one segment on each digit, with
 * each digit showing a different segment, cycling through all
 * 14 segments twice, then illuminating the colon and then the
 * decimal, before pausing briefly with a blank display then
 * repeating the process.
 * 
 * By the way, the reason for having the four digits "out of
 * step", so to speak, is that it proves that each is being
 * independantly addressed.
 * 
 * Note that as it stands, it is intended to test only one
 * display unit at a time, with the default address on the
 * unit.  However, it can easily be expanded to relax these
 * restrictions, and can also be modified for use as a
 * "power-on self test" at the beginning of a more complex
 * sketch.
 * 
 * I wrote this for my own use, but have decided to release
 * it as "user contributed" to the general community.
 * 
 * Written by Clark Jones, and released as "beerware", as-is,
 * with absolutely no guarantees.
 */

 #include <Wire.h>
 #include <SparkFun_Alphanumeric_Display.h>

 HT16K33 display;

 uint8_t segSequence[14]{
  // This is necessary because of the order that the segments
  // are assigned letters -- it's easier for a human to view
  // them and verify that all are working if they appear to
  // "rotate".
  'A', // top
  'B', // upper right
  'C', // lower right
  'D', // bottom
  'E', // lower left
  'F', // upper left
  'J', // upper middle vertical
  'K', // upper right diagonal
  'I', // middle right horizontal
  'L', // lower right diagonal
  'M', // lower middle vertical
  'N', // lower left diagonal
  'G', // middle left horizontal
  'H'  // upper left diagonal
 };

int currStep;
unsigned long nextDisplayTime;

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

  currStep = 0;
  nextDisplayTime = 0;
}; // setup()

void loop() {
  unsigned long nowTime;
  nowTime = millis();
  if ( nowTime >= nextDisplayTime ) {
    nextDisplayTime = nowTime + 250;
    display.clear();
    if ( currStep < 28 ) {
      // Display segments
      // Notice that these will "wrap around" on segSequence
      display.illuminateSegment( segSequence[ currStep % 14 ], 0);
      display.illuminateSegment( segSequence[ ( currStep + 1 ) % 14 ], 1 );
      display.illuminateSegment( segSequence[ ( currStep + 2 ) % 14 ], 2 );
      display.illuminateSegment( segSequence[ ( currStep + 3 ) % 14 ], 3 );
    } else if ( currStep == 28 ) {
      display.colonOn();
    } else if ( currStep == 29 ) {
      display.decimalOn();
    } else {
      // Two things to notice: this generates a brief blank time
      // and by setting currStep to -1 it will then get
      // incremented to 0
      currStep = -1;
    };
    display.updateDisplay();
    currStep++;
  }; // nowTime >= nextDisplayTime

}; // loop()
