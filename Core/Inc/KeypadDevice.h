/*
 * KeypadDriver.h
 *
 *  Created on: 22 мар. 2026 г.
 *      Author: Арсений
 */

#ifndef INC_KEYPADDEVICE_H_
#define INC_KEYPADDEVICE_H_





#endif /* INC_KEYPADDEVICE_H_ */

const int TimeBetweenUpdates {10}; // Для того, чтобы не так часто опрашивать клавиатуру

#include "gpio.h"
#include <list>
#include <tuple>
#include <inttypes.h>

class KeypadDevice;

class KeypadDevice{
public:
	uint16_t hold_delay;
	KeypadDevice(std::list<GPIO_TypeDef*> cols_GPIO, std::list<uint16_t> columns_GPIO_pins,
			std::list<GPIO_TypeDef*> rows_GPIO, std::list<uint16_t> rows_GPIO_pins,
			std::list<std::list<char>> symbols,
			uint16_t hold_delay);
	void UpdateKeymap();
	std::tuple<bool, char> GetChar();
	std::list<std::tuple<bool, char>> GetChars();
	void ClearBuffer();
protected:
	std::list<std::list<char>> symbols;
	std::list<std::list<uint32_t>> KeyMap{};
	std::list<std::tuple<bool, char>> buffer{};
	std::list<uint16_t> rows_GPIO_pins;
	std::list<GPIO_TypeDef*> rows_GPIOx;
	std::list<uint16_t> columns_GPIO_pins;
	std::list<GPIO_TypeDef*> columns_GPIOx;
private:
	uint32_t LastUpdate;
};
