#include <Wire.h>
#include "SparkFun_Alphanumeric_Display.h"

HT16K33 display; // Initialize Sensor

void setup() 
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Test sketch");

  if(display.begin() == true)
  {
    Serial.println("Begin");
  }
  else
  {
    Serial.println("Device failed to setup - Freezing code.")l
    while(1); // Runs forever
  }

  

}

void loop() 
{
  // No need for a loop yet - still testing basic functionalities
}
