/*
 * i2c_lcd.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: jajtn
 */

//#include "cmsis_os.h"
#include "defines.h"
#include "i2c_lcd.hpp"

#ifndef SIMULATOR
#include "stm32l4xx_hal_i2c.h"
#include "stm32l4xx_hal_tim.h"
#else
#include ""
#endif

const uint8_t rowTable[] = {0,2,1,3};

LCD::LCD(I2C_HandleTypeDef *hi2c)
{
	ASSERT_PTR(hi2c);
	m_addr = LCD_I2C_DEFAULT_ADDRESS;
	m_hi2c = hi2c;

	m_inputBuff.s[LCD_MAX_CHAR-1] = '\0';

	clear();
	write("--------Initialization--------");

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

}

LCD::LCD(I2C_HandleTypeDef *hi2c, uint8_t addr)
{
	ASSERT_PTR(hi2c);
	m_addr = addr;
	m_hi2c = hi2c;
	init();
}



void LCD::init(void)
{

}

void LCD::write(const char *str)
{
	writePosition(0, 0, str);
}

void LCD::writePosition(uint8_t row, uint8_t col, const char *str)
{

	if(row>LCD_MAX_ROW)
		row = LCD_MAX_ROW;
	if(col>LCD_MAX_COLS)
		col = LCD_MAX_COLS;

	while(*str != '\0')
	{
		m_inputBuff.d[rowTable[row]][col++] = *str;
		if(col>LCD_MAX_COLS)
		{
			col = 0;
			row++;
			if(row>LCD_MAX_ROW)
				break;
		}
		str++;
	}
	m_bufferState = BUFFER_UPDATED;
}

void LCD::clear()
{

	for(uint8_t i = 0; i<LCD_MAX_CHAR; i++)
	{
		m_inputBuff.s[i] = ' ';
	}
	m_bufferState = BUFFER_UPDATED;
}

void LCD::updateBuffer(char *str)
{
	char data_u, data_l;

	for(uint32_t i=0; i< LCD_BUFF_LEN; i+=4)
	{
		data_u = ((*str)&0xf0);
		data_l = (((*str)<<4)&0xf0);

		m_buff[i]   = data_u|0x0D;
		m_buff[i+1] = data_u|0x09;
		m_buff[i+2] = data_l|0x0D;
		m_buff[i+3] = data_l|0x09;

		str++;
	}

	m_bufferState = BUFFER_ENCODED;
}

void LCD::sendCommand(char cmd)
{
	ASSERT_PTR(m_hi2c);

	char data_u, data_l;
	uint8_t data_t[4];

	data_u = (cmd & 0xf0);
	data_l = ((cmd<<4) & 0xf0);

	data_t[0] = data_u|0x08;  	//en=1, rs=0
	data_t[1] = data_u|0x0C;  	//en=0, rs=0
	data_t[2] = data_l|0x08;  	//en=1, rs=0
	data_t[3] = data_l|0x0C;  	//en=0, rs=0

	HAL_I2C_Master_Transmit(m_hi2c, m_addr,(uint8_t *) data_t, 4, 200);
}


void LCD::update()
{
	ASSERT_PTR(m_hi2c);

	HAL_StatusTypeDef retVal;

	if(m_bufferState == BUFFER_UPDATED)
		updateBuffer(m_inputBuff.s);

	if(m_bufferState == BUFFER_ENCODED)
	{
		retVal = HAL_I2C_Master_Transmit_DMA(m_hi2c, m_addr , (uint8_t*) m_buff, LCD_BUFF_LEN);
		if(retVal == HAL_OK)
			m_bufferState = BUFFER_UNCHANGED;
	}

}

void LCD::setBrightness(uint8_t b)
{
	ASSERT_PTR(m_hi2c);
	__HAL_TIM_SET_COMPARE(m_htim, m_channel, b);
}

void LCD::setPWMSource(TIM_HandleTypeDef *htim, uint32_t channel)
{
	ASSERT_PTR(htim);
	m_htim = htim;
	m_channel = channel;

	__HAL_TIM_SET_AUTORELOAD(m_htim, 256);
	HAL_TIM_PWM_Start(m_htim, m_channel);
}







