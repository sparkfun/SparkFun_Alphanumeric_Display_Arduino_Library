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

uint8_t displayRAM[16];

/*--------------------------- Device Status----------------------------------*/
/* BEGIN
    This function checks if the TMP will ACK over I2C, and
	if the TMP will correctly self-identify with the proper
	device ID. This will set the address of the device along 
	with setting the wire for the I2C Communication. 
	This will return true if both checks pass.
*/
bool HT16K33::begin(uint8_t address, TwoWire &wirePort)
{
	_deviceAddress = address; //grab the address of the alphanumeric
	_i2cPort = &wirePort;	 //assign the port

	//return true if the device is connected
	return (isConnected());
}

//DEBUG: DO WE HAVE TO DO ANY OF THIS CHECK_ID STUFF??!
bool HT16K33::isConnected()
{
	_i2cPort->beginTransmission(_deviceAddress);
	if (_i2cPort->endTransmission() == 0)
	{
		//Check that we are talking to the right thing. Is the device ID what we expect?
		// if (checkDeviceID() == true)
		return true;
	}
	return false;
}

uint8_t HT16K33::deviceID()
{
	uint8_t id;
	// read(ID, (uint8_t *)&id, (uint8_t)sizeof(id)); //read and return the ID register
	return id;
}

bool HT16K33::checkDeviceID()
{
	return (deviceID() == DEV_ID); //Return true if the device ID matches what we expect
}

/*---------------------------- Light Up Functions ------------------------------*/
bool HT16K33::initialize()
{
	//internal system clock enable
	int temp = 0;
	write(0x21, (uint8_t *)&temp, 0);
	//ROW/INT output pin set, INT pin output level set
	write(0xA0, (uint8_t *)&temp, 0);
	//Dimming set
	write(0xEF, (uint8_t *)&temp, 0);
	//Blinking set - blinking off, display on
	write(0x81, (uint8_t *)&temp, 0);
}

bool HT16K33::clearDisplay()
{
	int temp = 0;
	for (int i = 0; i < 16; i++)
	{
		//Clear the displayRAM array
		displayRAM[i] = 0;
		write(i, (uint8_t *)&temp, (uint8_t)sizeof(temp));
	}
}

bool HT16K33::setBrightness(uint8_t duty)
{
	int temp = 0;
	duty = duty + 223;
	write(duty, (uint8_t *)&temp, 0);
}

//DEBUGGING: is this possible?
uint8_t HT16K33::getBrightness()
{
	uint8_t duty;
	read()
}

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

	if (digit == 1)
	{
		if (segment >= 'A' && segment <= 'G')
			row = 8;
		else
			row = 9;
	}
	else if (digit == 2)
	{
		if (segment >= 'A' && segment <= 'G')
			row = 7;
		else
			row = 10;
	}
	else if (digit == 3)
	{
		if (segment >= 'A' && segment <= 'G')
			row = 2;
		else
			row = 3;
	}
	else if (digit == 4)
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
	updateDisplay();
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

void HT16K33::printString(char *s, uint8_t n)
{
	for (int i = 0; i < n; i++)
	{
		printChar(s[i], i + 1);
	}
}

bool HT16K33::updateDisplay()
{
	return (write(0, (uint8_t *)displayRAM, (uint8_t)sizeof(displayRAM)));
}

// /* SET BLINK RATE
// 	This function will take in a number, 0-3, to set the rate of blinking
// 	for the display.
// 	0 = Off
// 	1 = 2 Hz
// 	2 = 1 Hz
// 	3 = 0.5 Hz
// */
// uint8_t HT16K33::setBlinkRate(uint8_t rate)
// {
// 	if ((rate > 3) || (rate < 0)) //

// 		uint16_t fullBlinkRate = 0b1000000110000000 // The address starts with blinking rate off

// 								 fullBlinkRate
// 								 << 8;
// 	fullBlinkRate += rate;
// 	Wire.beginTransmission(HT16K33_I2C_ADDRESS);
// 	Wire.write(fullBlinkRate); // Send the full level with the correct level shifted in
// 	Wire.endTransmission();

// 	// Implementation in case above doesn't work
// 	// if(rate == 0)		// 0
// 	//    Wire.write(0x8280);
// 	// else if(rate == 1)	// 1
// 	// 	  Wire.write(0x8380);
// 	// else if(rate == 2)	// 2
// 	// 	  Wire.write(0x8480);
// 	// else if(rate == 3)	// 3
// 	// 	  Wire.write(0x8580);
// 	// else
// 	// 	  return 0;
// }

/*----------------------- Internal I2C Abstraction -----------------------------*/

bool HT16K33::read(uint8_t reg, uint8_t *buff, uint8_t buffSize)
{
	_i2cPort->beginTransmission(_deviceAddress);
	_i2cPort->write(reg);
	_i2cPort->endTransmission();

	if (_i2cPort->requestFrom(_deviceAddress, buffSize) > 0)
	{
		for (uint8_t i = 0; i < buffSize; i++)
		{
			buff[i] = _i2cPort->read();
		}
		return true;
	}

	return false;
}

//Overloaded function declaration
//Use when reading just one byte of data
bool HT16K33::read(uint8_t reg, uint8_t data)
{
	return (read(reg, (uint8_t *)&data, (uint8_t)sizeof(data)));
}

bool HT16K33::write(uint8_t reg, uint8_t *buff, uint8_t buffSize)
{
	_i2cPort->beginTransmission(_deviceAddress);
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

//Overloaded function declaration
//Use when writing just one byte of data
bool HT16K33::write(uint8_t reg, uint8_t data)
{
	return (write(reg, (uint8_t *)&data, (uint8_t)sizeof(data)));
}