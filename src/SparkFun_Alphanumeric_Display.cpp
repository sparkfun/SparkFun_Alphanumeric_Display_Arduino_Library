/******************************************************************************
SparkFun_Alphanumeric_Display.cpp
SparkFun Alphanumeric Display Library Source File
Priyanka Makin @ SparkFun Electronics
Original Creation Date: February 25, 2020
***Include GitHub Repo link here***

This file implements all functions of the HT16K33 class. Functions here range
from printing to one or more Alphanumeric Displays, changing the display settings, and writing/
reading the RAM of the HT16K33.

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno
	TMP117 Breakout Version: 1.0.0

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include <SparkFun_Alphanumeric_Display.h>

/*--------------------------- Device Status----------------------------------*/

bool HT16K33::begin(uint8_t addressLeft, uint8_t addressLeftCenter, uint8_t addressRightCenter, uint8_t addressRight, TwoWire &wirePort)
{

	_deviceAddressLeft = addressLeft;				//grab the address of the alphanumeric
	_deviceAddressLeftCenter = addressLeftCenter;   //grab the address of the alphanumeric
	_deviceAddressRightCenter = addressRightCenter; //grab the address of the alphanumeric
	_deviceAddressRight = addressRight;				//grab the address of the alphanumeric

	if (_deviceAddressRight != DEFAULT_NOTHING_ATTACHED)
		numberOfDisplays = 4;
	else if (_deviceAddressRightCenter != DEFAULT_NOTHING_ATTACHED)
		numberOfDisplays = 3;
	else if (_deviceAddressLeftCenter != DEFAULT_NOTHING_ATTACHED)
		numberOfDisplays = 2;
	else
		numberOfDisplays = 1;

	//TODO: malloc more displayRAM

	Serial.print("numberOfDisplays: ");
	Serial.println(numberOfDisplays);

	_i2cPort = &wirePort; //Remember the user's setting

	for (uint8_t i = 0; i < numberOfDisplays; i++)
	{
		if (isConnected(i) == false)
		{
			Serial.print("Failed isConnected() on displayNumber: ");
			Serial.println(i);
			return false;
		}
		// if (checkDeviceID(i) == false)
		// {
		// 	Serial.println(i);
		// 	Serial.println("Hello, I've failed checkDeviceID()");
		// 	return false;
		// }
	}

	if (initialize() == false)
	{
		Serial.println("Failed initialize()");
		return false;
	}

	// clear();
	return true;
}

//The repeated start condition seems to help the display at address 0x71.
bool HT16K33::isConnected(uint8_t displayNumber)
{
	_i2cPort->beginTransmission(lookUpDisplayAddress(displayNumber));
	_i2cPort->endTransmission();

	_i2cPort->beginTransmission(lookUpDisplayAddress(displayNumber));
	if (_i2cPort->endTransmission() == 0)
		return true;
	return false;
}

bool HT16K33::initialize()
{
	//Turn on system clock of all displays
	if (enableSystemClock() == false)
	{
		Serial.println("I've failed enableSystemClock()");
		return false;
	}

	//Set brightness of all displays to full brightness
	if (setBrightness(16) == false)
	{
		Serial.println("I've failed setBrightness()");
		return false;
	}

	//Blinking set - blinking off
	if (setBlinkRate(ALPHA_BLINK_RATE_NOBLINK) == false)
	{
		Serial.println("I've failed setBlinkRate()");
		return false;
	}

	//Turn on all displays
	if (displayOn() == false)
	{
		Serial.println("Failed display on");
		return false;
	}

	return true;
}

// //Verify that all objects on I2C bus are alphanumeric displays
// bool HT16K33::checkDeviceID(uint8_t displayNumber)
// {
// 	uint8_t address = lookUpDisplayAddress(displayNumber);
// 	uint8_t test = 0xAA;
// 	uint8_t temp;
// 	//Turn off display - DEBUG: should it be ALL or ONE?
// 	singleDisplayOff(displayNumber);
// 	//Write 0xAA to register 0
// 	writeRAM(address, 0, (uint8_t *)&test, 1);
// 	//Read it back, it should be 0xAA
// 	readRAM(address, 0, (uint8_t *)&temp, sizeof((uint8_t)temp));
// 	if (temp != test)
// 		return false;
// 	//DEBUGGING: taking this out for now... can't call this write to ALL displays when we haven't initalized all displays yet!
// 	//Clear the write we just did
// 	// clear();
// 	//Turn display back on
// 	singleDisplayOn(displayNumber);
// 	return true;
// }

bool HT16K33::enableSystemClock()
{
	bool status = true;
	for (uint8_t i = 0; i < numberOfDisplays; i++)
	{
		if (enableSystemClockSingle(i) == false)
			status = false;
	}
	return status;
}

bool HT16K33::disableSystemClock()
{
	bool status = true;
	for (uint8_t i = 0; i < numberOfDisplays; i++)
	{
		if (enableSystemClockSingle(i) == false)
			status = false;
	}
	return status;
}

bool HT16K33::enableSystemClockSingle(uint8_t displayNumber)
{
	uint8_t dataToWrite = ALPHA_CMD_SYSTEM_SETUP | 1; //Enable system clock

	uint8_t temp = 0;
	return (writeRAM(lookUpDisplayAddress(displayNumber), dataToWrite, (uint8_t *)&temp, 0));
}

bool HT16K33::disableSystemClockSingle(uint8_t displayNumber)
{
	uint8_t dataToWrite = ALPHA_CMD_SYSTEM_SETUP | 0; //Standby mode

	uint8_t temp = 0;
	return (writeRAM(lookUpDisplayAddress(displayNumber), dataToWrite, (uint8_t *)&temp, 0));
}

uint8_t HT16K33::lookUpDisplayAddress(uint8_t displayNumber)
{
	switch (displayNumber)
	{
	case 0:
		return _deviceAddressLeft;
		break;
	case 1:
		return _deviceAddressLeftCenter;
		break;
	case 2:
		return _deviceAddressRightCenter;
		break;
	case 3:
		return _deviceAddressRight;
		break;
	}

	return 0; //We shouldn't get here
}

/*-------------------------- Display configuration functions ---------------------------*/

bool HT16K33::clear()
{
	//Clear the displayRAM array
	for (uint8_t i = 0; i < 16 * (numberOfDisplays); i++)
		displayRAM[i] = 0;

	digitPosition = 0;

	return (updateDisplay());
}

//Duty valid between 1 and 16
bool HT16K33::setBrightness(uint8_t duty)
{
	bool status = true;
	for (uint8_t i = 0; i < numberOfDisplays; i++)
	{
		if (setBrightnessSingle(i, duty) == false)
			status = false;
	}
	return status;
}

bool HT16K33::setBrightnessSingle(uint8_t displayNumber, uint8_t duty)
{
	if (duty > 15)
		duty = 15; //Error check

	uint8_t dataToWrite = ALPHA_CMD_DIMMING_SETUP | duty;

	uint8_t temp = 0;
	return (writeRAM(lookUpDisplayAddress(displayNumber), dataToWrite, (uint8_t *)&temp, 0));
}

//Parameter "rate" in Hz
//Valid options for "rate" are defined by datasheet: 2, 1, or 0.5 Hz
//Any other input to this function will result in steady alphanumeric display
bool HT16K33::setBlinkRate(float rate)
{
	bool status = true;
	for (uint8_t i = 0; i < numberOfDisplays; i++)
	{
		if (setBlinkRateSingle(i, rate) == false)
			status = false;
	}
	return status;
}

bool HT16K33::setBlinkRateSingle(uint8_t displayNumber, float rate)
{
	int temp = 0;

	if (rate == 2)
	{
		blinkRate = ALPHA_BLINK_RATE_2HZ;
	}
	else if (rate == 1)
	{
		blinkRate = ALPHA_BLINK_RATE_1HZ;
	}
	else if (rate == 0.5)
	{
		blinkRate = ALPHA_BLINK_RATE_0_5HZ;
	}
	//default to no blink
	else
	{
		blinkRate = ALPHA_BLINK_RATE_NOBLINK;
	}

	uint8_t dataToWrite = ALPHA_CMD_DISPLAY_SETUP | (blinkRate << 1) | displayOnOff;

	return (writeRAM(lookUpDisplayAddress(displayNumber), dataToWrite, (uint8_t *)&temp, 0));
}

bool HT16K33::displayOnSingle(uint8_t displayNumber)
{
	return setDisplayOnOff(displayNumber, true);
}
bool HT16K33::displayOffSingle(uint8_t displayNumber)
{
	return setDisplayOnOff(displayNumber, false);
}

//Set or clear the display on/off bit of a given display number
bool HT16K33::setDisplayOnOff(uint8_t displayNumber, bool turnOnDisplay)
{
	if (turnOnDisplay == true)
		displayOnOff = ALPHA_DISPLAY_ON;
	else
		displayOnOff = ALPHA_DISPLAY_OFF;

	int temp = 0;
	uint8_t dataToWrite = ALPHA_CMD_DISPLAY_SETUP | (blinkRate << 1) | displayOnOff;

	return (writeRAM(lookUpDisplayAddress(displayNumber), dataToWrite, (uint8_t *)&temp, 0));
}

//Turn on/off the entire display
bool HT16K33::displayOn()
{
	bool status = true;

	displayOnOff = ALPHA_DISPLAY_ON;

	for (uint8_t i = 0; i < numberOfDisplays; i++)
	{
		if (displayOnSingle(i) == false)
			status = false;
	}

	return status;
}

bool HT16K33::displayOff()
{
	bool status = true;

	displayOnOff = ALPHA_DISPLAY_OFF;

	for (uint8_t i = 0; i < numberOfDisplays; i++)
	{
		if (displayOffSingle(i) == false)
			status = false;
	}

	return status;
}

/*---------------------------- Light up functions ---------------------------------*/

void HT16K33::illuminateSegment(uint8_t segment, uint8_t digit)
{
	uint8_t com;
	uint8_t row;

	com = segment - 'A';
	if (com > 6)
	{
		com = com - 7;
	}
	if (segment == 'I')
		com = 0;
	if (segment == 'H')
		com = 1;

	row = digit;
	if (segment > 'G')
		row += 4;

	//Decimal point/colon is pseudo-"fourth digit"
	if (digit == 4)
	{
		row = 8;
	}

	uint8_t dat;
	uint8_t adr = com * 2;

	//Determine the address bit
	if (row > 7)
	{
		adr++;
	}

	//Determine the data bit
	if (row > 7)
	{
		row -= 8;
	}
	dat = 1 << row;

	uint8_t displayNumber;

	// //what display are we trying to light up??
	// if (digitPosition > 0 && digitPosition < 5)
	// 	displayNumber = 0;
	// //on second display
	// else if (digitPosition > 4)
	// {
	// 	displayNumber = 1;
	// }

	// //Update displayRAM array
	// displayRAM[16 * displayNumber + adr] = displayRAM[16 * displayNumber + adr] ^ (dat >> 16 * displayNumber);

	displayRAM[adr] = displayRAM[adr] ^ dat;
}

void HT16K33::illuminateChar(uint16_t disp, uint8_t digit)
{
	for (int i = 0; i < 14; i++)
	{
		if ((disp >> i) & 0b1)
		{
			illuminateSegment('A' + i, digit);
		}
	}
}

#define SFE_ALPHANUM_UNKNOWN_CHAR 89

void HT16K33::printChar(uint8_t displayChar, uint8_t digit)
{
	static uint16_t alphanumeric_segs[90]{
		0b00000000000000, //' ' (space)

		0b1001101001110,  //'#'
		0b1001101101101,  //'$'
		0b10010000100100, //'%'
		0b110011011001,   //'&'
		0b1000000000,	 //'''
		0b111001,		  //'('
		0b1111,			  //')'
		0b11111010000000, //'*'
		0b1001101000000,  //'+'
		0b10000000000000, //','
		0b101000000,	  //'-'
		0b10,			  //'.' - DEBUG: need to test
		0b10010000000000, //'/'
		0b111111,		  //'0'
		0b10000000110,	//'1'
		0b101011011,	  //'2'
		0b101001111,	  //'3'
		0b101100110,	  //'4'
		0b101101101,	  //'5'
		0b101111101,	  //'6'
		0b1010000000001,  //'7'
		0b101111111,	  //'8'
		0b101100111,	  //'9'
		0b1,			  //':' - DEBUG: need to test
		0b10001000000000, //';'
		0b110000000000,   //'<'
		0b101001000,	  //'='
		0b10000010000000, //'>'

		0b101110111,	  //'A'
		0b1001100001111,  //'B'
		0b111001,		  //'C'
		0b1001000001111,  //'D'
		0b101111001,	  //'E'
		0b101110001,	  //'F'
		0b100111101,	  //'G'
		0b101110110,	  //'H'
		0b1001000001001,  //'I'
		0b11110,		  //'J'
		0b110001110000,   //'K'
		0b111000,		  //'L'
		0b10010110110,	//'M'
		0b100010110110,   //'N'
		0b111111,		  //'O'
		0b101110011,	  //'P'
		0b100000111111,   //'Q'
		0b100101110011,   //'R'
		0b110001101,	  //'S'
		0b1001000000001,  //'T'
		0b111110,		  //'U'
		0b10010000110000, //'V'
		0b10100000110110, //'W'
		0b10110010000000, //'X'
		0b1010010000000,  //'Y'
		0b10010000001001, //'Z'
		0b111001,		  //'['
		0b100010000000,   //'\'
		0b1111,			  //']'

		0b1000,			  //'_'
		0b10000000,		  //'`'
		0b101011111,	  //'a'
		0b100001111000,   //'b'
		0b101011000,	  //'c'
		0b10000100001110, //'d'
		0b1111001,		  //'e'
		0b1110001,		  //'f'
		0b110001111,	  //'g'
		0b101110100,	  //'h'
		0b1000000000000,  //'i'
		0b1110,			  //'j'
		0b1111000000000,  //'k'
		0b1001000000000,  //'l'
		0b1000101010100,  //'m'
		0b100001010000,   //'n'
		0b101011100,	  //'o'
		0b10001110001,	//'p'
		0b100101100011,   //'q'
		0b1010000,		  //'r'
		0b110001101,	  //'s'
		0b1111000,		  //'t'
		0b11100,		  //'u'
		0b10000000010000, //'v'
		0b10100000010100, //'w'
		0b10110010000000, //'x'
		0b1100001110,	 //'y'
		0b10010000001001, //'z'
		0b10000011001001, //'{'
		0b1001000000000,  //'|'
		0b110100001001,   //'}'

		0b11111111111111, //Unknown character
	};

	uint16_t characterPosition = 65532;

	//space
	if (displayChar == ' ')
		characterPosition = 0;
	//Symbols
	else if (displayChar >= '#' && displayChar <= '>')
	{
		characterPosition = displayChar - '#' + 1;
	}
	//Upper case letters + symbols
	else if (displayChar >= 'A' && displayChar <= ']')
	{
		characterPosition = displayChar - 'A' + 1 + 28;
	}
	//Symbols + lower case letters
	else
	{
		characterPosition = displayChar - '_' + 1 + 28 + 29;
	}

	// //For DEBUGGING
	// Serial.println(characterPosition);

	// //User wants to display unknown character
	// //If the current character was not defined in alphanumeric_segs
	// if (characterPosition == 65532)
	// 	characterPosition = SFE_ALPHANUM_UNKNOWN_CHAR;

	// //Digit keeps track of the digit we are illuminating on each individual display
	// //Can only be either 0, 1, 2, or 3
	// digit %= 4;

	// //Take care of pecial characters/segments '.' and ':'
	// if (characterPosition == 12 | characterPosition == 24)
	// {
	// 	digit = 4;
	// }

	illuminateChar(alphanumeric_segs[characterPosition], digit);
}

/*
 * Write a byte to the display.
 * Required for Print.
 */
size_t HT16K33::write(uint8_t b)
{
	// if (b == '.' | b == ':')
	// 	printChar(b, 1);
	// else
	// {
	printChar(b, digitPosition++);
	digitPosition %= (numberOfDisplays * 4); //Convert displays to number of digits
	// }

	return (updateDisplay()); //Send RAM buffer over I2C bus
} // write

/*
 * Write a character buffer to the display.
 * Required for Print.
 */
size_t HT16K33::write(const uint8_t *buffer, size_t size)
{
	size_t n = size;
	uint8_t buff;

	while (size--)
	{
		buff = *buffer++;
		// if (buff == '.')
		// 	printChar('.', 1);
		// else if (buff == ':')
		// 	printChar(':', 1);
		// else
		// {
		printChar(buff, digitPosition++);
		digitPosition %= (numberOfDisplays * 4);
		// }
	}
	updateDisplay(); //Send RAM buffer over I2C bus
	return n;
} //write

//Write a string to the display
size_t HT16K33::write(const char *str)
{
	if (str == NULL)
		return 0;
	return write((const uint8_t *)str, strlen(str));
} //write

bool HT16K33::updateDisplay()
{
	bool status = true;
	for (uint8_t i = 0; i < numberOfDisplays; i++)
	{
		if (writeRAM(lookUpDisplayAddress(i), 0, (uint8_t *)displayRAM + (i * 16), 16) == false)
		{
			Serial.print("updateDisplay fail at display 0x");
			Serial.println(lookUpDisplayAddress(i), HEX);
			status = false;
		}
	}

	return status;
}

/*----------------------- Internal I2C Abstraction -----------------------------*/

bool HT16K33::readRAM(uint8_t address, uint8_t reg, uint8_t *buff, uint8_t buffSize)
{
	_i2cPort->beginTransmission(address);
	_i2cPort->endTransmission();

	_i2cPort->beginTransmission(address);
	_i2cPort->write(reg);
	_i2cPort->endTransmission();

	if (_i2cPort->requestFrom(address, buffSize) > 0)
	{
		for (uint8_t i = 0; i < buffSize; i++)
		{
			buff[i] = _i2cPort->read();
		}
		return true;
	}

	return false;
}

// //Overloaded function declaration
// //Use when reading just one byte of data
// bool HT16K33::read(uint8_t reg, uint8_t data)
// {
// 	return (read(reg, (uint8_t *)&data, (uint8_t)sizeof(data)));
// }

bool HT16K33::writeRAM(uint8_t address, uint8_t reg, uint8_t *buff, uint8_t buffSize)
{
	_i2cPort->beginTransmission(address);
	_i2cPort->endTransmission(false);

	_i2cPort->beginTransmission(address);
	_i2cPort->write(reg);

	for (uint8_t i = 0; i < buffSize; i++)
	{
		_i2cPort->write(buff[i]);
	}

	if (_i2cPort->endTransmission() == 0)
	{
		return true;
	}

	return false;
}

bool HT16K33::writeOne(uint8_t address, uint8_t reg)
{
	_i2cPort->beginTransmission(address);
	_i2cPort->endTransmission(false);

	_i2cPort->beginTransmission(address);
	_i2cPort->write(reg);

	if (_i2cPort->endTransmission() == 0)
	{
		return true;
	}

	return false;
}

// //Overloaded function declaration
// //Use when writing just one byte of data
// bool HT16K33::writeRAM(uint8_t reg, uint8_t data)
// {
// 	return (writeRAM(reg, (uint8_t *)&data, (uint8_t)sizeof(data)));
// }