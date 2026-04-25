/*
 * LCDDriver.h
 *
 *  Created on: 22 мар. 2026 г.
 *      Author: Ideapad-43
 */

#ifndef INC_LCDDRIVER_H_
#define INC_LCDDRIVER_H_

#define PIN_RS    (1 << 0)
#define PIN_EN    (1 << 2)
#define BACKLIGHT (1 << 3)
#define LCD_DELAY_MS 5

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

#define ROWS_AMOUNT 20

#endif /* INC_LCDDRIVER_H_ */

#include "i2c.h"
#include <string>
#include <inttypes.h>

#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_def.h>
#include <stm32f1xx_hal_i2c.h>
#include <sys/_stdint.h>

class LCDDevice {
public:
	LCDDevice(I2C_HandleTypeDef &hi2c, uint16_t I2CAddress);
	void send(uint8_t data, uint8_t flags);
	inline void command(uint8_t value);
	void write(uint8_t value);
	void write(std::string Text);
	void setCursor(uint8_t col, uint8_t row);
	void setLine(uint8_t line);
	void nextLine();
	void clear();
	void init();
private:
	uint8_t out_char;
	uint16_t I2CAddress;
	I2C_HandleTypeDef *hi2c;
	int8_t utf_hi_char;
	uint8_t currentRow;
	uint8_t currentCol;
	uint8_t _numlines,_currline;
};
