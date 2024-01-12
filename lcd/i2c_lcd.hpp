/*
 * i2c_lcd.hpp
 *
 *  Created on: Dec 20, 2023
 *      Author: jajtn
 */

#ifndef I2C_LCD_HPP_
#define I2C_LCD_HPP_

#include "stm32l4xx_hal.h"

#define LCD_EXP_ENABLE			 (0x0C)
#define LCD_EXP_DISABLE		     (0x08)

#define LCD_NO_ROWS				 4
#define LCD_MAX_ROW				 (LCD_NO_ROWS-1)
#define LCD_NO_COLS				 20
#define LCD_MAX_COLS			 (LCD_NO_COLS-1)
#define LCD_MAX_CHAR			 (LCD_NO_ROWS*LCD_NO_COLS)
#define LCD_BUFF_LEN 			 (4*LCD_MAX_CHAR)
#define LCD_I2C_DEFAULT_ADDRESS  0x4E

#define USE_ASSERT_HANDLER

#ifdef USE_ASSERT_HANDLER
	#define ASSERT_HANDLER(h) if(h == NULL) return
#else
	#define ASSERT_HANDLER(h)
#endif

typedef union{

	char d[LCD_NO_ROWS][LCD_NO_COLS];
	char s[LCD_MAX_CHAR];

}inputBuff_t;

typedef enum
{
	TX_INIT,
	TX_REQUEST,
	TX_1ST_ROW,
	TX_2ND_ROW,
	TX_3RD_ROW,
	TX_4TH_ROW,
}txState_t;

typedef enum
{
	BUFFER_UNCHANGED,
	BUFFER_UPDATED,
	BUFFER_ENCODED

}bufferState_t;

class LCD{

public:
	LCD();
	LCD(I2C_HandleTypeDef *hi2c, uint8_t addr);

private:

	inputBuff_t inputBuff;
	char buff[LCD_BUFF_LEN];
	I2C_HandleTypeDef *hi2c;
	bufferState_t bufferState;
	uint8_t _addr;

	void updateBuffer(char *str);
	void sendCommand(char cmd);

public:

	void update(void);
	void write(char *str);
	void writePosition(uint8_t row, uint8_t col, char *str);
	void clear(void);

};




#endif /* I2C_LCD_HPP_ */
