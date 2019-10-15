/******************************************************************************
SparkFunTMP117.cpp
SparkFunTMP117 Library Source File
Madison Chodikov @ SparkFun Electronics
Original Creation Date: October 13, 2019
***Include GitHub Repo link here***

This file implements all functions of the HT16K33 class. Functions here range
from writing to the Alphanumeric Display, changing the brightness, setting the
oscillation on, and writing to the various settings with the HT16K33

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno
	TMP117 Breakout Version: 1.0.0

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_Alphanmeric_Display.h"


/* CONSTRUCTOR
    This function will use the main I2C port on the Arduino 
	by default.
	This needs to be called when running the example sketches to
	initialize the sensor and be able to call to the library. 
*/
HT16K33::HT16K33()
{
}


/* BEGIN
    This function checks if the TMP will ACK over I2C, and
	if the TMP will correctly self-identify with the proper
	device ID. This will set the address of the device along 
	with setting the wire for the I2C Communication. 
	This will return true if both checks pass.
*/
bool HT16K33::begin(uint8_t sensorAddress, TwoWire &wirePort)
{
	_i2cPort = &wirePort;			// Chooses the wire port of the device
	_deviceAddress = sensorAddress; // Sets the address of the device

	//make sure the TMP will acknowledge over I2C
	_i2cPort->beginTransmission(sensorAddress);
	if (_i2cPort->endTransmission() != 0)
	{
		return false;
	}

	uint16_t deviceID = readRegister(HT16K33_I2C_ADDRESS); // reads registers into rawData

	//make sure the device ID reported by the HT16K33 is correct
	//should always be 0x____
	if (deviceID != DEVICE_ID_VALUE)
	{
		return false;
	}

	/* Start the Oscillation of the device */
	Wire.beginTransmission(0x2120); // 0x2120 = The oscillation of system setup and the definition

	return true; //returns true if all the checks pass
}

/* SET BRIGHTNESS
	This function will take in a number in the range 0-15 and send it to the 
	dimming set on the HT16K33 Command Summary. 
*/
bool setBrightness(uint8_t brightLevel)
{
	uint16_t fullLevel = 0b1110000011101111
	Wire.beginTransmission(HT16K33_I2C_ADDRESS);
	if(brightLevel == 0)		// 0
		Wire.write(0xE0EF);
	else if(brightLevel == 1)	// 1
		Wire.write(0xE1EF);
	else if(brightLevel == 2)	// 2
		Wire.write(0xE2EF);
	else if(brightLevel == 3)	// 3
		Wire.write(0xE3EF);
	else if(brightLevel == 4)	// 4
		Wire.write(0xE4EF);
	else if(brightLevel == 5)	// 5
		Wire.write(0xE5EF);
	else if(brightLevel == 6)	// 6
		Wire.write(0xE6EF);
	else if(brightLevel == 7)	// 7
		Wire.write(0xE7EF);
	else if(brightLevel == 8)	// 8
		Wire.write(0xE8EF);
	else if(brightLevel == 9)   // 9
		Wire.write(0xE9EF);
	else if(brightLevel == 10)  // 10
		Wire.write(0xEAEF);
	else if(brightLevel == 11)  // 11
		Wire.write(0xEBEF);
	else if(brightLevel == 12)  // 12
		Wire.write(0xECEF);
	else if(brightLevel == 13)  // 13
		Wire.write(0xEDEF);
	else if(brightLevel == 14)  // 14
		Wire.write(0xEEEF);
	else if(brightLevel == 15)  // 15
		Wire.write(0xEFEF);
	else
		return false;

	return true;
}