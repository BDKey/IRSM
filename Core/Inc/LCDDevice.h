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

#define ROWS_AMOUNT 20

#endif /* INC_LCDDRIVER_H_ */

#include "i2c.h"
#include <string>
#include <inttypes.h>

class LCDDevice;
