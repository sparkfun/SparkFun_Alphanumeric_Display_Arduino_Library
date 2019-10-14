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
	_i2cPort->beginTransmission(_deviceAddress);
	if (_i2cPort->endTransmission() != 0)
	{
		return false;
	}

	uint16_t deviceID = readRegister(TMP117_DEVICE_ID); // reads registers into rawData

	//make sure the device ID reported by the TMP is correct
	//should always be 0x0117
	if (deviceID != DEVICE_ID_VALUE)
	{
		return false;
	}

	return true; //returns true if all the checks pass
}
