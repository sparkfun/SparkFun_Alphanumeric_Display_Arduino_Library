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

#include <Wire.h>
#include <Arduino.h>

#ifndef __SparkFun_Alphanumeric_Display_H__
#define __SparkFun_Alphanumeric_Display_H__

#define HT16K33_I2C_ADDRESS 

class HT16K33
{
public:
    HT16K33(); // Constructor

    bool begin(uint8_t driverAddress, TwoWire &wirePort); // Sets the address of the device and opens the Wire port for communication
    void setBrightness(uint8_t brightLevel);              // Sets the brightness of the LED Display, can take a number 1-16
    uint8_t getBrightness();                              // Returns the level of brightness, 1-16
    void setBlinkRate(uint8_t rate);                      // Sets the blinking rate of the LED Display, can take a number 0-3
    uint8_t getBlinkRate();
    void clearDisplay();
    void clearSegment();
    void setSegment(uint8_t segment, uint8_t digit);
    void displayNumer();

private:
    TwoWire *_i2cPort = NULL; //The generic connection to user's chosen I2C hardware
    uint8_t _deviceAddress;   // Address of

    uint16_t readRegister(uint8_t reg);             // Reads 2 register bytes from sensor
    void writeRegister(uint8_t reg, uint16_t data); // Wires single byte of data to the sensor
};

#endif