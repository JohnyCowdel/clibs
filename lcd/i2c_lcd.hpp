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
#define LCD_MAX_CHAR			 (LCD_NO_ROWS*LCD_NO_COLS+1)
#define LCD_BUFF_LEN 			 (4*LCD_MAX_CHAR)
#define LCD_I2C_DEFAULT_ADDRESS  0x4E

#define USE_ASSERT_HANDLER

#ifdef USE_ASSERT_HANDLER
	#define ASSERT_PTR(h) if(h == NULL) return
#else
	#define ASSERT_PTR(h)
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
	LCD(I2C_HandleTypeDef *hi2c);
	LCD(I2C_HandleTypeDef *hi2c, uint8_t addr);
private:

	inputBuff_t m_inputBuff;
	char m_buff[LCD_BUFF_LEN];
	I2C_HandleTypeDef *m_hi2c;
	TIM_HandleTypeDef *m_htim;
	uint32_t m_channel;
	bufferState_t m_bufferState;
	uint8_t m_addr;
	uint8_t m_brightness;

	void init(void);
	void updateBuffer(char *str);
	void sendCommand(char cmd);

public:

	void setPWMSource(TIM_HandleTypeDef *htim, uint32_t channel);
	void setBrightness(uint8_t b);
	void update(void);
	void write(const char *str);
	void writePosition(uint8_t row, uint8_t col, const char *str);
	void clear(void);

};




#endif /* I2C_LCD_HPP_ */
