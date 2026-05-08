/*
 * KeypadDriver.cpp
 *
 *  Created on: 22 мар. 2026 г.
 *      Author: Арсений
 */
#include "KeypadDevice.h"

// Columns are for writing, Rows are for reading
KeypadDevice::KeypadDevice(std::list<GPIO_TypeDef*>& columns_GPIOx,
	    std::list<uint16_t>& columns_GPIO_pins,
	    std::list<GPIO_TypeDef*>& rows_GPIOx,
	    std::list<uint16_t>& rows_GPIO_pins,
	    std::list<std::list<char>>& symbols,
	    uint16_t hold_delay){
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
//SOMETHING ISN'T WORKING (it can't register any key presses(maybe it's all about me being dumb asf with pointers and iterators(f*ck my stupid life :3c)))
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

	auto keymaprowiter = KeyMap.begin();
	auto keymapcolumniter = (*keymaprowiter).begin();

	auto symbolsrowiter = symbols.begin();
	auto symbolscolumniter = (*symbolsrowiter).begin();

	auto rowgpio = *rowgpioiter;
	auto columngpio = *columngpioiter;
	for (auto& Row : symbols){
		rowgpio = *rowgpioiter;
		HAL_GPIO_WritePin(rowgpio, *rowiter, GPIO_PIN_SET);
		HAL_Delay(5);
		for (auto& Symbol : Row){
			columngpio = *columngpioiter;
			if ((*keymapcolumniter)==0){
				if (HAL_GPIO_ReadPin(columngpio, *columniter) == GPIO_PIN_SET){
					(*keymapcolumniter)=HAL_GetTick();
				}
			} else {
				if (HAL_GPIO_ReadPin(columngpio, *columniter) == GPIO_PIN_RESET){
					buffer.push_back(std::tuple<bool, char> {((HAL_GetTick() - (*symbolscolumniter)) >= hold_delay), Symbol});
					(*keymapcolumniter)=0;
				}
			}
			columniter++;
			columngpioiter++;
			keymapcolumniter++;
			symbolscolumniter++;
		}
		HAL_GPIO_WritePin(rowgpio, *rowiter, GPIO_PIN_RESET);
		columniter = columns_GPIO_pins.begin();
		columngpioiter = columns_GPIOx.begin();
		rowiter++;
		rowgpioiter++;
		keymaprowiter++;
		keymapcolumniter = (*keymaprowiter).begin();
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
