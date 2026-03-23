/*
 * KeypadDriver.h
 *
 *  Created on: 22 мар. 2026 г.
 *      Author: Арсений
 */

#ifndef INC_KEYPADDEVICE_H_
#define INC_KEYPADDEVICE_H_





#endif /* INC_KEYPADDEVICE_H_ */

#define TimeBetweenUpdates 10; // Для того, чтобы не так часто опрашивать клавиатуру

#include "gpio.h"
#include <list>
#include <tuple>

typedef __uint8_t  uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;

class KeypadDevice;
