/*
 * KeypadDriver.cpp
 *
 *  Created on: 22 мар. 2026 г.
 *      Author: Арсений
 */
#include "KeypadDevice.h"

// Columns are for writing, Rows are for reading
KeypadDevice::KeypadDevice(const std::list<GPIO_TypeDef*>& columns_GPIOx,
	    const std::list<uint16_t>& columns_GPIO_pins,
	    const std::list<GPIO_TypeDef*>& rows_GPIOx,
	    const std::list<uint16_t>& rows_GPIO_pins,
	    const std::list<std::list<char>>& symbols,
	    uint16_t hold_delay = 500){
	this->rows_GPIO_pins = rows_GPIO_pins;
	this->rows_GPIOx = rows_GPIOx;
	this->columns_GPIO_pins = columns_GPIO_pins;
	this->columns_GPIOx = columns_GPIOx;
	this->symbols = symbols;
	this->hold_delay = hold_delay;
	KeyMap = std::list<std::list<uint32_t>>(symbols.size(), std::list<uint32_t>(symbols.begin()->size(), 0)); // Initialyzes the KeyMap
	LastUpdate = HAL_GetTick();
	KeyMap = {};
	buffer = {};
}
void KeypadDevice::UpdateKeymap(){
	if ((HAL_GetTick() - LastUpdate) < TimeBetweenUpdates) {
		return;
	}
	LastUpdate = HAL_GetTick();
	// Precious pointers, point me to the bright future
	auto rowiter = rows_GPIO_pins.begin();
	auto rowgpioiter = rows_GPIOx.begin();
	auto columniter = columns_GPIO_pins.begin();
	auto columngpioiter = columns_GPIOx.begin();

	auto symbolsrowiter = symbols.begin();
	auto symbolscolumniter = (*symbolsrowiter).begin();
	for (std::list<char> Row : symbols){
		for (char Symbol : Row){
			auto rowgpio = *rowgpioiter;
			auto columngpio = *columngpioiter;
			HAL_GPIO_WritePin(rowgpio, *rowiter, GPIO_PIN_SET);
			if ((*symbolscolumniter)==0){
				if (HAL_GPIO_ReadPin(columngpio, *columniter) == GPIO_PIN_SET){
					(*symbolscolumniter)=HAL_GetTick();
				}
			} else {
				if (HAL_GPIO_ReadPin(columngpio, *columniter) == GPIO_PIN_RESET){
					buffer.push_back(std::tuple<bool, char> {((HAL_GetTick() - (*symbolscolumniter)) >= hold_delay), Symbol});
					(*symbolscolumniter)=0;
				}
			}
			HAL_GPIO_WritePin(rowgpio, *rowiter, GPIO_PIN_RESET);
			columniter++;
			columngpioiter++;
			symbolscolumniter++;
		}
		columniter = columns_GPIO_pins.begin();
		columngpioiter = columns_GPIOx.begin();
		rowiter++;
		rowgpioiter++;
		symbolsrowiter++;
		symbolscolumniter = (*symbolsrowiter).begin();
	}
}
bool KeypadDevice::BufferIsNotEmpty(){
	return !buffer.empty();
}
std::tuple<bool, char> KeypadDevice::GetChar(){
	if (buffer.empty()) return std::tuple<bool, char> {false, 0};
	std::tuple<bool, char> Key = buffer.front();
	buffer.pop_front();
	return Key;
}
std::list<std::tuple<bool, char>> KeypadDevice::GetChars(){
	if (buffer.empty()) return std::list<std::tuple<bool, char>> {std::tuple<bool, char> {false, 0}};
	std::list<std::tuple<bool, char>> Keys = buffer;
	buffer.clear();
	return Keys;
}
void KeypadDevice::ClearBuffer(){
	buffer.clear();
}
