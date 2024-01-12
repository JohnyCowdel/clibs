/*
 * i2c_lcd.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: jajtn
 */

#include "cmsis_os.h"
#include "i2c_lcd.hpp"
#include "stm32l4xx_hal_i2c.h"
#include "stm32l4xx_hal_dma.h"
#include "i2c.h"

char defaultChars[] = "--------Initialization--------";
const uint8_t rowTable[] = {0,2,1,3};

LCD::

LCD::LCD(I2C_HandleTypeDef *_hi2c)
{
	ASSERT_HANDLER(_hi2c);

	this->hi2c = _hi2c;
	//this->hi2c->MasterTxCpltCallback = &this->txCallback;

	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	sendCommand(0x30);
	HAL_Delay(5);  // wait for >4.1ms
	sendCommand(0x30);
	HAL_Delay(1);  // wait for >100us
	sendCommand(0x30);
	HAL_Delay(10);
	sendCommand(0x20);  // 4bit mode
	HAL_Delay(10);

  // display initialisation
	sendCommand(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	sendCommand(0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	sendCommand(0x01);  // clear display
	HAL_Delay(2);
	sendCommand(0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	sendCommand(0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)

	clear();
	write(defaultChars);
}

void LCD::write(char *str)
{
	writePosition(0, 0, str);
}

void LCD::writePosition(uint8_t row, uint8_t col, char *str)
{

	if(row>LCD_MAX_ROW)
		row = LCD_MAX_ROW;
	if(col>LCD_MAX_COLS)
		col = LCD_MAX_COLS;

	while(true)
	{
		inputBuff.d[rowTable[row]][col++] = *str;
		if(col>LCD_MAX_COLS)
		{
			col = 0;
			row++;
			if(row>LCD_MAX_ROW)
				break;
		}
		str++;
		if(*str == '\0')
			break;
	}
	this->bufferState = BUFFER_UPDATED;
}

void LCD::clear()
{

	for(uint8_t i = 0; i<LCD_MAX_CHAR; i++)
	{
		inputBuff.s[i] = ' ';
	}
	this->bufferState = BUFFER_UPDATED;
}

void LCD::updateBuffer(char *str)
{
	char data_u, data_l;

	for(uint32_t i=0; i< LCD_BUFF_LEN; i+=4)
	{
		data_u = ((*str)&0xf0);
		data_l = (((*str)<<4)&0xf0);

		this->buff[i]   = data_u|0x0D;
		this->buff[i+1] = data_u|0x09;
		this->buff[i+2] = data_l|0x0D;
		this->buff[i+3] = data_l|0x09;

		str++;
	}

	this->bufferState = BUFFER_ENCODED;
}

void LCD::sendCommand(char cmd)
{
	ASSERT_HANDLER(this->hi2c);

	char data_u, data_l;
	uint8_t data_t[4];

	data_u = (cmd & 0xf0);
	data_l = ((cmd<<4) & 0xf0);

	data_t[0] = data_u|LCD_EXP_ENABLE;  	//en=1, rs=0
	data_t[1] = data_u|LCD_EXP_DISABLE;  	//en=0, rs=0
	data_t[2] = data_l|LCD_EXP_ENABLE;  	//en=1, rs=0
	data_t[3] = data_l|LCD_EXP_DISABLE;  	//en=0, rs=0

	HAL_I2C_Master_Transmit(hi2c, LCD_I2C_EXPANDER_ADDRESS,(uint8_t *) data_t, 4, 100);
}


void LCD::update()
{
	HAL_StatusTypeDef retVal;

	if(bufferState == BUFFER_UPDATED)
		updateBuffer(inputBuff.s);

	if(bufferState == BUFFER_ENCODED)
	{
		retVal = HAL_I2C_Master_Transmit_DMA(hi2c, LCD_I2C_EXPANDER_ADDRESS , (uint8_t*) buff, sizeof(buff));
		if(retVal == HAL_OK)
			bufferState = BUFFER_UNCHANGED;
	}

}







