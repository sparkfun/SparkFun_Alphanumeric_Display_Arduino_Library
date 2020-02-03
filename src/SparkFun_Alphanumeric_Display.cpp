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
		// else if (segment >= 'H' && segment <= 'N' || segment == 'g')
		else
			row = 9;
	}
	else if (digit == 2)
	{
		if (segment >= 'A' && segment <= 'G')
			row = 7;
		// else if (segment >= 'H' && segment <= 'N' || segment == 'g')
		else
			row = 10;
	}
	else if (digit == 3)
	{
		if (segment >= 'A' && segment <= 'G')
			row = 2;
		// else if (segment >= 'H' && segment <= 'N' || segment == 'g')
		else
			row = 3;
	}
	else if (digit == 4)
	{
		if (segment >= 'A' && segment <= 'G')
			row = 0;
		// else if (segment >= 'H' && segment <= 'N' || segment == 'g')
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

void HT16K33::printChar(uint8_t dispChar, uint8_t digit)
{
	uint16_t temp;
	switch (dispChar)
	{
	case '0':
		temp = SEG_0;
		break;
	case '1':
		temp = SEG_1;
		break;
	case '2':
		temp = SEG_2;
		break;
	case '3':
		temp = SEG_3;
		break;
	case '4':
		temp = SEG_4;
		break;
	case '5':
		temp = SEG_5;
		break;
	case '6':
		temp = SEG_6;
		break;
	case '7':
		temp = SEG_7;
		break;
	case '8':
		temp = SEG_8;
		break;
	case '9':
		temp = SEG_9;
		break;
	case 'A':
		temp = SEG_A;
		break;
	case 'B':
		temp = SEG_B;
		break;
	case 'C':
		temp = SEG_C;
		break;
	case 'D':
		temp = SEG_D;
		break;
	case 'E':
		temp = SEG_E;
		break;
	case 'F':
		temp = SEG_F;
		break;
	case 'G':
		temp = SEG_G;
		break;
	case 'H':
		temp = SEG_H;
		break;
	case 'I':
		temp = SEG_I;
		break;
	case 'J':
		temp = SEG_J;
		break;
	case 'K':
		temp = SEG_K;
		break;
	case 'L':
		temp = SEG_L;
		break;
	case 'M':
		temp = SEG_M;
		break;
	case 'N':
		temp = SEG_N;
		break;
	case 'O':
		temp = SEG_O;
		break;
	case 'P':
		temp = SEG_P;
		break;
	case 'Q':
		temp = SEG_Q;
		break;
	case 'R':
		temp = SEG_R;
		break;
	case 'S':
		temp = SEG_S;
		break;
	case 'T':
		temp = SEG_T;
		break;
	case 'U':
		temp = SEG_U;
		break;
	case 'V':
		temp = SEG_V;
		break;
	case 'W':
		temp = SEG_W;
		break;
	case 'X':
		temp = SEG_X;
		break;
	case 'Y':
		temp = SEG_Y;
		break;
	case 'Z':
		temp = SEG_Z;
		break;
	case 'a':
		temp = SEG_a;
		break;
	case 'b':
		temp = SEG_b;
		break;
	case 'c':
		temp = SEG_c;
		break;
	case 'd':
		temp = SEG_d;
		break;
	case 'e':
		temp = SEG_e;
		break;
	case 'f':
		temp = SEG_f;
		break;
	case 'g':
		temp = SEG_g;
		break;
	case 'h':
		temp = SEG_h;
		break;
	case 'i':
		temp = SEG_i;
		break;
	case 'j':
		temp = SEG_j;
		break;
	case 'k':
		temp = SEG_k;
		break;
	case 'l':
		temp = SEG_l;
		break;
	case 'm':
		temp = SEG_m;
		break;
	case 'n':
		temp = SEG_n;
		break;
	case 'o':
		temp = SEG_o;
		break;
	case 'p':
		temp = SEG_p;
		break;
	case 'q':
		temp = SEG_q;
		break;
	case 'r':
		temp = SEG_r;
		break;
	case 's':
		temp = SEG_s;
		break;
	case 't':
		temp = SEG_t;
		break;
	case 'u':
		temp = SEG_u;
		break;
	case 'v':
		temp = SEG_v;
		break;
	case 'w':
		temp = SEG_w;
		break;
	case 'x':
		temp = SEG_x;
		break;
	case 'y':
		temp = SEG_y;
		break;
	case 'z':
		temp = SEG_z;
		break;
	default:
		temp = SEG_CLEAR;
	}

	illuminateChar(temp, digit);
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

// /* SET BRIGHTNESS
// 	This function will take in a number in the range 0-15 and send it to the
// 	dimming set on the HT16K33 Command Summary.
// */
// bool setBrightness(uint8_t brightLevel)
// {
// 	uint16_t fullLevel = 0b1110000011101111

// 						 brightLevel
// 						 << 8;
// 	fullLevel += brightLevel;
// 	Wire.beginTransmission(HT16K33_I2C_ADDRESS);
// 	Wire.write(fullLevel); // Send the full level with the correct level shifted in
// 	Wire.endTransmission();

// 	// Implementation in case above doesn't work
// 	// if(brightLevel == 0)		// 0
// 	// 	Wire.write(0xE0EF);
// 	// else if(brightLevel == 1)	// 1
// 	// 	Wire.write(0xE1EF);
// 	// else if(brightLevel == 2)	// 2
// 	// 	Wire.write(0xE2EF);
// 	// else if(brightLevel == 3)	// 3
// 	// 	Wire.write(0xE3EF);
// 	// else if(brightLevel == 4)	// 4
// 	// 	Wire.write(0xE4EF);
// 	// else if(brightLevel == 5)	// 5
// 	// 	Wire.write(0xE5EF);
// 	// else if(brightLevel == 6)	// 6
// 	// 	Wire.write(0xE6EF);
// 	// else if(brightLevel == 7)	// 7
// 	// 	Wire.write(0xE7EF);
// 	// else if(brightLevel == 8)	// 8
// 	// 	Wire.write(0xE8EF);
// 	// else if(brightLevel == 9)   // 9
// 	// 	Wire.write(0xE9EF);
// 	// else if(brightLevel == 10)  // 10
// 	// 	Wire.write(0xEAEF);
// 	// else if(brightLevel == 11)  // 11
// 	// 	Wire.write(0xEBEF);
// 	// else if(brightLevel == 12)  // 12
// 	// 	Wire.write(0xECEF);
// 	// else if(brightLevel == 13)  // 13
// 	// 	Wire.write(0xEDEF);
// 	// else if(brightLevel == 14)  // 14
// 	// 	Wire.write(0xEEEF);
// 	// else if(brightLevel == 15)  // 15
// 	// 	Wire.write(0xEFEF);
// 	// else
// 	// 	return false;

// 	return true;
// }

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