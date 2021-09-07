#include <Wire.h>

#include <SparkFun_Alphanumeric_Display.h> //Click here to get the library: http://librarymanager/All#Alphanumeric_Display by SparkFun
HT16K33 display;

void setup()
{
  Serial.begin(115200);
  Serial.println("Qwiic Alphanumeric examples");

  Wire.begin(); //Join I2C bus

  if (display.begin(0x71) == false)
  {
    Serial.println("Device did not acknowledge! Freezing.");
    while (1);
  }
  Serial.println("Display acknowledged.");

  display.setBrightness(15);
  display.print("\t\t\t\t");  //Unknown char turns on all segements!
  display.decimalOn();
  display.colonOn();
}

void loop()
{
//  for (uint8_t i = 0; i < 17; i++){
//    display.setBrightness(i);
//    display.print("\t\t\t\t");  //Unknown char turns on all segements!
//    display.decimalOn();
//    display.colonOn();
//    delay(1000);
//  }
}
