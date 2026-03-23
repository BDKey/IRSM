/*
 * KeypadDriver.cpp
 *
 *  Created on: 22 мар. 2026 г.
 *      Author: Ideapad-43
 */
#include "KeypadDevice.h"


//Columns are for writing, Rows are for reading
class KeypadDevice{
public:
	std::list<std::list<char>> Symbols;
	uint16_t LongHoldDelay;
	KeypadDevice(GPIO_TypeDef& GPIOx, std::list<uint16_t> Columns_GPIO_Pins, std::list<uint16_t> Rows_GPIO_Pins, std::list<std::list<char>> Symbols, uint16_t LongHoldDelay){
		this->GPIOx = &GPIOx;
		this->Columns_GPIO_Pins = Columns_GPIO_Pins;
		this->Rows_GPIO_Pins = Rows_GPIO_Pins;
		this->Symbols = Symbols;
		this->LongHoldDelay = LongHoldDelay;
	}
	void UpdateKeymap(){
		uint8_t row{0};
		uint8_t column{0};
		auto rowiter = Rows_GPIO_Pins.begin();
		auto columniter = Columns_GPIO_Pins.begin();
		for (std::list<char> Row : Symbols){
			for (char Symbol : Row){
				HAL_GPIO_WritePin(GPIOx, *rowiter, GPIO_PIN_SET);
				if (KeyMap[row][column]==0){
					//if (HAL_GPIO_ReadPin(GPIOx, ))
				} else {

				}
				HAL_GPIO_WritePin(GPIOx, *rowiter, GPIO_PIN_SET);
				columniter++;
				column++;
			}
			columniter = Columns_GPIO_Pins.begin();
			column=0;
			rowiter++;
			row++;
		}
	}
	char GetChar(){
		UpdateKeymap();
		if (buffer.empty()) return 0;
		char Key = buffer.front();
		buffer.pop_front();
		return Key;
	}
	std::list<char> GetChars(){
		UpdateKeymap();
		if (buffer.empty()) return 0;
		std::list<char> Keys = buffer;
		buffer.clear();
		return Keys;
	}
protected:
	std::list<std::list<uint32_t>> KeyMap{};
	std::list<char> buffer{};
	std::list<uint16_t> Columns_GPIO_Pins;
	std::list<uint16_t> Rows_GPIO_Pins;
	GPIO_TypeDef* GPIOx;
};
