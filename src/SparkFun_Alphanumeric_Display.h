/******************************************************************************
SparkFun_Alphanumeric_Display.h
SparkFun Alphanumeric Display Library Header File
Priyanka Makin @ SparkFun Electronics
Original Creation Date: July 25, 2019
https://github.com/sparkfun/SparkFun_Alphanumeric_Display_Arduino_Library

Updated April 30, 2020 by Gaston Williams to add defineChar function

Pickup a board here: https://sparkle.sparkfun.com/sparkle/storefront_products/16391

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

#include <Arduino.h>
#include <Wire.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_ADDRESS 0x70 // Default I2C address when A0, A1 are floating
#define DEFAULT_NOTHING_ATTACHED 0xFF

// Define constants for segment bits
#define SEG_A 0x0001
#define SEG_B 0x0002
#define SEG_C 0x0004
#define SEG_D 0x0008
#define SEG_E 0x0010
#define SEG_F 0x0020
#define SEG_G 0x0040
#define SEG_H 0x0080
#define SEG_I 0x0100
#define SEG_J 0x0200
#define SEG_K 0x0400
#define SEG_L 0x0800
#define SEG_M 0x1000
#define SEG_N 0x2000


typedef enum
{
    ALPHA_BLINK_RATE_NOBLINK = 0b00,
    ALPHA_BLINK_RATE_2HZ = 0b01,
    ALPHA_BLINK_RATE_1HZ = 0b10,
    ALPHA_BLINK_RATE_0_5HZ = 0b11,
} alpha_blink_rate_t;

typedef enum
{
    ALPHA_DISPLAY_ON = 0b1,
    ALPHA_DISPLAY_OFF = 0b0,
} alpha_display_t;

typedef enum
{
    ALPHA_DECIMAL_ON = 0b1,
    ALPHA_DECIMAL_OFF = 0b0,
} alpha_decimal_t;

typedef enum
{
    ALPHA_COLON_ON = 0b1,
    ALPHA_COLON_OFF = 0b0,
} alpha_colon_t;

typedef enum
{
    ALPHA_CMD_SYSTEM_SETUP = 0b00100000,
    ALPHA_CMD_DISPLAY_SETUP = 0b10000000,
    ALPHA_CMD_DIMMING_SETUP = 0b11100000,
} alpha_command_t;

// Structure for defining new character displays
struct CharDef {
  uint8_t  position;
  int16_t segments;
  struct CharDef * next;
};

// class HT16K33
class HT16K33 : public Print
{
private:
    TwoWire *_i2cPort;          // The generic connection to user's chosen I2C hardware
    uint8_t _deviceAddressDisplayOne; // Address of primary alphanumeric display
    uint8_t _deviceAddressDisplayTwo;
    uint8_t _deviceAddressDisplayThree;
    uint8_t _deviceAddressDisplayFour;
    uint8_t digitPosition = 0;
    uint8_t numberOfDisplays = 1;
    bool displayOnOff = 0; // Tracks display on/off bit of display setup register
    bool decimalOnOff = 0;
    bool colonOnOff = 0;
    uint8_t blinkRate = ALPHA_BLINK_RATE_NOBLINK; // Tracks blink bits in display setup register

    // Enough RAM for up to 4 displays on same I2C bus
    uint8_t displayRAM[16 * 4];
    char displayContent[4 * 4 + 1] = "";

    // Linked List of character definitions
    struct CharDef * pCharDefList = NULL;

public:
    // Device status
    bool begin(uint8_t addressDisplayOne = DEFAULT_ADDRESS,
               uint8_t addressDisplayTwo = DEFAULT_NOTHING_ATTACHED,
               uint8_t addressDisplayThree = DEFAULT_NOTHING_ATTACHED,
               uint8_t addressDisplayFour = DEFAULT_NOTHING_ATTACHED,
               TwoWire &wirePort = Wire); // Sets the address of the device and opens the Wire port for communication
    bool isConnected(uint8_t displayNumber);
    bool initialize();
    uint8_t lookUpDisplayAddress(uint8_t displayNumber);

    //Display configuration functions
    bool clear();
    bool setBrightness(uint8_t duty);
    bool setBrightnessSingle(uint8_t displayNumber, uint8_t duty);
    bool setBlinkRate(float rate);
    bool setBlinkRateSingle(uint8_t displayNumber, float rate);
    bool displayOn();
    bool displayOff();
    bool displayOnSingle(uint8_t displayNumber);
    bool displayOffSingle(uint8_t displayNumber);
    bool setDisplayOnOff(uint8_t displayNumber, bool turnOnDisplay);

    bool enableSystemClock();
    bool disableSystemClock();
    bool enableSystemClockSingle(uint8_t displayNumber);
    bool disableSystemClockSingle(uint8_t displayNumber);

    // Light up functions
    void illuminateSegment(uint8_t segment, uint8_t digit);
    void illuminateChar(uint16_t disp, uint8_t digit);
    void printChar(uint8_t displayChar, uint8_t digit);
    bool updateDisplay();

    // Define Character Segment Map
    bool defineChar(uint8_t displayChar, uint16_t segmentsToTurnOn);
    uint16_t getSegmentsToTurnOn (uint8_t charPos);

    // Decimal functions
    bool decimalOn();
    bool decimalOff();
    bool decimalOnSingle(uint8_t displayNumber, bool updateNow = true);
    bool decimalOffSingle(uint8_t displayNumber, bool updateNow = true);
    bool setDecimalOnOff(uint8_t displayNumber, bool turnOnDecimal, bool updateNow = true);
    
    // Colon functions
    bool colonOn();
    bool colonOff();
    bool colonOnSingle(uint8_t displayNumber, bool updateNow = true);
    bool colonOffSingle(uint8_t displayNumber, bool updateNow = true);
    bool setColonOnOff(uint8_t displayNumber, bool turnOnColon, bool updateNow = true);

    // Shifting
    bool shiftRight(uint8_t shiftAmt = 1);
    bool shiftLeft(uint8_t shiftAmt = 1);

    // For overloading the print function
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buffer, size_t size);
    virtual size_t write(const char *str);

    // I2C abstraction
    bool readRAM(uint8_t address, uint8_t reg, uint8_t *buff, uint8_t buffSize);
    bool writeRAM(uint8_t address, uint8_t reg, uint8_t *buff, uint8_t buffSize);
    bool writeRAM(uint8_t reg, uint8_t data);
};

#endif