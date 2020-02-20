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
#include <SparkFun_Alphanumeric_Display.h>

/*--------------------------- Device Status----------------------------------*/
/* BEGIN
    This function checks if the TMP will ACK over I2C, and
	if the TMP will correctly self-identify with the proper
	device ID. This will set the address of the device along 
	with setting the wire for the I2C Communication. 
	This will return true if both checks pass.
*/
bool HT16K33::begin(uint8_t addressLeft, uint8_t addressLeftCenter, uint8_t addressRightCenter, uint8_t addressRight, TwoWire &wirePort)
{

	_deviceAddressLeft = addressLeft;				//grab the address of the alphanumeric
	_deviceAddressLeftCenter = addressLeftCenter;   //grab the address of the alphanumeric
	_deviceAddressRightCenter = addressRightCenter; //grab the address of the alphanumeric
	_deviceAddressRight = addressRight;				//grab the address of the alphanumeric

	if (_deviceAddressRight != DEFAULT_NOTHING_ATTACHED)
	{
		sizeOfDisplay = 16;
	}
	else if (_deviceAddressRightCenter != DEFAULT_NOTHING_ATTACHED)
	{
		sizeOfDisplay = 12;
	}
	else if (_deviceAddressLeftCenter != DEFAULT_NOTHING_ATTACHED)
	{
		sizeOfDisplay = 8;
	}

	//TODO: try to figure this out
	//malloc more displayRAM

	_i2cPort = &wirePort; //assign the port

	for (uint8_t i = 0; i < sizeOfDisplay / 4; i++)
	{
		if (isConnected(lookUpDisplayAddress(i)) == false)
		{
			return false;
		}
		if (initialize(lookUpDisplayAddress(i)) == false)
		{
			return false;
		}
		if (checkDeviceID(lookUpDisplayAddress(i) == false))
		{
			return false;
		}
	}

	clearDisplay();
	return true;
}

//DEBUG: DO WE HAVE TO DO ANY OF THIS CHECK_ID STUFF??!
bool HT16K33::isConnected(uint8_t address)
{
	_i2cPort->beginTransmission(address);
	if (_i2cPort->endTransmission() == 0)
	{
		//Check that we are talking to the right thing. Is the device ID what we expect?
		// if (checkDeviceID() == true)
		return true;
	}
	return false;
}

bool HT16K33::initialize(uint8_t address)
{
	//We need this temp zero value when we only write one byte to HT16K33 RAM
	int temp = 0;

	//internal system clock enable
	if (writeRAM(address, 0x21, (uint8_t *)&temp, 0) == false)
		return false;
	//ROW/INT output pin set, INT pin output level set
	if (writeRAM(address, 0xA0, (uint8_t *)&temp, 0) == false)
		return false;
	//Set brightness of display by duty cycle
	if (setBrightness(16) == false)
	{
		return false;
	}
	//Blinking set - blinking off, display on
	if (setBlinkRate(0) == false)
	{
		blinkRate = 0b000;
		onState = 1;
		return false;
	}
	return true;
}

//Verify that all objects on I2C bus are alphanumeric displays
bool HT16K33::checkDeviceID(uint8_t address)
{
	uint8_t temp;
	//Turn off display - DEBUG: should it be ALL or ONE?
	allDisplaysOff();
	//Write 0xAA to register 0
	writeRAM(address, 0, 0xAA, 1);
	//Read it back, it should be 0xAA
	readRAM(address, 0, (uint8_t *)&temp, sizeof((uint8_t)temp));
	if (temp != 0xAA)
		return false;
	//Clear the write we just did
	clearDisplay();
	//Turn display back on
	allDisplaysOn();
	return true;
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
}

/*-------------------------- Display configuration functions ---------------------------*/

bool HT16K33::clearDisplay()
{
	for (uint8_t i = 0; i < 16 * sizeOfDisplay / 4; i++)
	{
		//Clear the displayRAM array
		displayRAM[i] = 0;
		// write(i, (uint8_t *)&temp, (uint8_t)sizeof(temp));
	}
	updateDisplay();
	digitPosition = 0;
}

//Duty valid between 1 and 16
bool HT16K33::setBrightness(uint8_t duty)
{
	bool status = true;
	for (uint8_t i = 0; i < sizeOfDisplay / 4; i++)
	{
		if (setBrightnessDisplay(i, duty) == false)
			status = false;
	}
	return status;
}

bool HT16K33::setBrightnessDisplay(uint8_t displayNumber, uint8_t duty)
{
	int temp = 0;
	//Duty value is acceptable
	if (1 <= duty <= 16)
	{
		//Based on datasheet
		duty = duty + 223;
	}
	//else if duty value doesn't make sense, default to full brightness
	else
	{
		duty = 239;
	}

	return (writeRAM(lookUpDisplayAddress(displayNumber), duty, (uint8_t *)&temp, 0));
}

//Parameter "rate" in Hz
//Valid options for "rate" are defined by datasheet: 2, 1, or 0.5 Hz
//Any other input to this function will result in steady alphanumeric display
bool HT16K33::setBlinkRate(float rate)
{
	bool status = true;
	for (uint8_t i = 0; i < sizeOfDisplay / 4; i++)
	{
		if (setBlinkRateDisplay(i, rate) == false)
			status = false;
	}
	return status;
}

bool HT16K33::setBlinkRateDisplay(uint8_t displayNumber, float rate)
{
	int temp = 0;

	if (rate == 2)
	{
		blinkRate = 0b010;
	}
	else if (rate == 1)
	{
		blinkRate = 0b100;
	}
	else if (rate == 0.5)
	{
		blinkRate = 0b110;
	}
	//default to no blink
	else
	{
		blinkRate = 0b000;
	}

	//If the display is blinking, then it must also be on
	onState = 1;

	uint8_t writeBlinkRate = 0b10000001 ^ blinkRate;
	return (writeRAM(lookUpDisplayAddress(displayNumber), writeBlinkRate, (uint8_t *)&temp, 0));
}

bool HT16K33::allDisplaysOn()
{
	bool status = true;
	for (uint8_t i = 0; i < sizeOfDisplay / 4; i++)
	{
		if (singleDisplayOn(i) == false)
			status = false;
	}
	onState = 1;
	return status;
}

bool HT16K33::singleDisplayOn(uint8_t displayNumber)
{
	int temp = 0;
	uint8_t state = 0b10000001 ^ blinkRate;
	return (writeRAM(lookUpDisplayAddress(displayNumber), state, (uint8_t *)&temp, 0));
}

bool HT16K33::allDisplaysOff()
{
	bool status = true;
	for (uint8_t i = 0; i < sizeOfDisplay / 4; i++)
	{
		if (singleDisplayOff(i) == false)
			status = false;
	}
	onState = 0;
	return status;
}

bool HT16K33::singleDisplayOff(uint8_t displayNumber)
{
	int temp = 0;
	uint8_t state = 0b10000000 ^ blinkRate;
	return (writeRAM(lookUpDisplayAddress(displayNumber), state, (uint8_t *)&temp, 0));
}

/*---------------------------- Light up functions ---------------------------------*/

void HT16K33::illuminateSegment(uint8_t segment, uint8_t digit)
{
	//DEBUGGING
	Serial.write(segment);
	Serial.println();

	//TODO: reassign pins on hardware to make this happen!
	//segment = segment - 'A';

	uint8_t com;
	uint8_t row;

	if (segment == 'A' || segment == 'I')
	{
		com = 6;
	}
	else if (segment == 'B' || segment == 'H')
	{
		com = 5;
	}
	else if (segment == 'C' || segment == 'J')
	{
		com = 3;
	}
	else if (segment == 'D' || segment == 'K')
	{
		com = 7;
	}
	else if (segment == 'E' || segment == 'L')
	{
		com = 1;
	}
	else if (segment == 'F' || segment == 'M')
	{
		com = 4;
	}
	else if (segment == 'G' || segment == 'N')
	{
		com = 2;
	}

	if (digit == 0)
	{
		if (segment >= 'A' && segment <= 'G')
			row = 8;
		else
			row = 9;
	}
	else if (digit == 1)
	{
		if (segment >= 'A' && segment <= 'G')
			row = 7;
		else
			row = 10;
	}
	else if (digit == 2)
	{
		if (segment >= 'A' && segment <= 'G')
			row = 2;
		else
			row = 3;
	}
	else if (digit == 3)
	{
		if (segment >= 'A' && segment <= 'G')
			row = 0;
		else
			row = 4;
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

	//Update displayRAM array
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

#define SFE_ALPHANUM_UNKNOWN_CHAR 87

void HT16K33::printChar(uint8_t displayChar, uint8_t digit)
{
	static uint16_t alphanumeric_segs[88]{
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
		0b1001000000000,  //':'
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

	uint16_t characterPosition = 0;

	//Symbols
	if (displayChar >= '#' && displayChar <= '-')
	{
		characterPosition = displayChar - '#';
	}
	//Symbols + digits
	else if (displayChar >= '/' && displayChar <= '>')
	{
		characterPosition = displayChar - '/' + 11;
	}
	//Upper case letters + symbols
	else if (displayChar >= 'A' && displayChar <= ']')
	{
		characterPosition = displayChar - 'A' + 11 + 16;
	}
	//Symbols + lower case letters
	else
	{
		characterPosition = displayChar - '_' + 11 + 16 + 29;
	}

	//DEBUG: this doesn't quite work the way I think it should
	//User wants to display unknown character
	if (characterPosition > sizeof(alphanumeric_segs))
		characterPosition = SFE_ALPHANUM_UNKNOWN_CHAR;

	illuminateChar(alphanumeric_segs[characterPosition], digit);
}

/*
 * Write a byte to the display.
 * Required for Print.
 */
size_t HT16K33::write(uint8_t b)
{
	printChar(b, digitPosition++);
	digitPosition %= sizeOfDisplay;

	updateDisplay(); //Send RAM buffer over I2C bus
} // write

/*
 * Write a character buffer to the display.
 * Required for Print.
 */
size_t HT16K33::write(const uint8_t *buffer, size_t size)
{
	size_t n = size;

	while (size--)
	{
		printChar(*buffer++, digitPosition++);
		digitPosition %= sizeOfDisplay;
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
}

bool HT16K33::updateDisplay()
{
	bool status = true;
	for (uint8_t i = 0; i < sizeOfDisplay / 4; i++)
	{
		if (writeRAM(lookUpDisplayAddress(i), 0, (uint8_t *)displayRAM + (i * 16), 16) == false)
			status = false;
	}

	return status;
}

/*----------------------- Internal I2C Abstraction -----------------------------*/

bool HT16K33::readRAM(uint8_t address, uint8_t reg, uint8_t *buff, uint8_t buffSize)
{
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

// //Overloaded function declaration
// //Use when writing just one byte of data
// bool HT16K33::writeRAM(uint8_t reg, uint8_t data)
// {
// 	return (writeRAM(reg, (uint8_t *)&data, (uint8_t)sizeof(data)));
// }