/******************************************************************************
SparkFun_Alphanumeric_Display.h
SparkFun Alphanumeric Display Library Header File
Madison Chodikov @ SparkFun Electronics
Original Creation Date: July 25, 2019
****ENTER PRODUCT PAGE URL HERE****

This file prototypes the HT16K33 class, implemented in SparkFun_Alphanumeric_Display.cpp.

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno
	Alphanumeric Display Breakout Version: 1.0.0

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/
#ifndef __SparkFun_Alphanumeric_Display_H__
#define __SparkFun_Alphanumeric_Display_H__

#include <Wire.h>
#include <Arduino.h>

#define DEFAULT_ADDRESS /*something*/
#define DEV_ID          /*something*/

class HT16K33
{
private:
    TwoWire *_i2cPort;      //The generic connection to user's chosen I2C hardware
    uint8_t _deviceAddress; // Address of alphanumeric display

public:
    //Device status
    bool begin(uint8_t address = DEFAULT_ADDRESS, TwoWire &wirePort); // Sets the address of the device and opens the Wire port for communication
    bool isConnected();
    uint8_t deviceID();
    bool checkDeviceID();
    bool setI2Caddress(uint8_t address);
    uint8_t getI2Caddress();

    bool setBrightness(uint8_t brightLevel); // Sets the brightness of the LED Display, can take a number 0-15
    // uint8_t getBrightness();                              // Returns the level of brightness, 1-16
    void startOscillation(); // Starts the oscillation of the device
    void setBlinkRate(uint8_t rate);
    // uint8_t getBlinkRate();
    void clearDisplay();                               // Clears the entire display
    void clearSegment(uint8_t segment, uint8_t digit); // Clears any desired segment input
    void setSegment(uint8_t segment, uint8_t digit);   // Sets any desired segment with an input of the segment and the digit
    void displayNumer();

    //I2C abstraction
    bool read(/*something*/ reg, uint8_t *buff, uint8_t buffSize);
    bool read(/*something*/ reg, uint8_t data);
    bool write(/*something*/ reg, uint8_t *buff, uint8_t buffSize);
    bool write(/*something*/ reg, uint8_t data);
};

#endif