/*
 * KeypadDriver.cpp
 *
 *  Created on: 22 мар. 2026 г.
 *      Author: Арсений
 */
#include "KeypadDevice.h"

// Columns are for writing, Rows are for reading
class KeypadDevice{
public:
	std::list<std::list<char>> Symbols;
	uint16_t HoldDelay;
	KeypadDevice(GPIO_TypeDef& GPIOx, std::list<uint16_t> Columns_GPIO_Pins, std::list<uint16_t> Rows_GPIO_Pins, std::list<std::list<char>> Symbols, uint16_t HoldDelay){
		this->GPIOx = &GPIOx;
		this->Columns_GPIO_Pins = Columns_GPIO_Pins;
		this->Rows_GPIO_Pins = Rows_GPIO_Pins;
		this->Symbols = Symbols;
		this->HoldDelay = HoldDelay;
		KeyMap = std::list<std::list<uint32_t>>(Symbols.size(), std::list<uint32_t>(Symbols.begin()->size(), 0)); // Initialyzes the KeyMap
		LastUpdate = HAL_GetTick();
	}
	void UpdateKeymap(){
		if ((HAL_GetTick() - LastUpdate) < TimeBetweenUpdates) {
			return;
		}
		LastUpdate = HAL_GetTick();
		// Precious pointers, point me to the bright future
		auto rowiter = Rows_GPIO_Pins.begin();
		auto columniter = Columns_GPIO_Pins.begin();

		auto symbolsrowiter = Symbols.begin();
		auto symbolscolumniter = (*symbolsrowiter).begin();
		for (std::list<char> Row : Symbols){
			for (char Symbol : Row){
				HAL_GPIO_WritePin(GPIOx, *rowiter, GPIO_PIN_SET);
				if ((*symbolscolumniter)==0){
					if (HAL_GPIO_ReadPin(GPIOx, *columniter) == GPIO_PIN_SET){
						(*symbolscolumniter)=HAL_GetTick();
					}
				} else {
					if (HAL_GPIO_ReadPin(GPIOx, *columniter) == GPIO_PIN_RESET){
						buffer.push_front(std::tuple<bool, char> {((HAL_GetTick() - (*symbolscolumniter)) >= HoldDelay), Symbol});
						(*symbolscolumniter)=0;
					}
				}
				HAL_GPIO_WritePin(GPIOx, *rowiter, GPIO_PIN_RESET);
				columniter++;
				symbolscolumniter++;
			}
			columniter = Columns_GPIO_Pins.begin();
			rowiter++;
			symbolsrowiter++;
			symbolscolumniter = (*symbolsrowiter).begin();
		}
	}
	std::tuple<bool, char> GetChar(){
		UpdateKeymap();
		if (buffer.empty()) return std::tuple<bool, char> {false, 0};
		std::tuple<bool, char> Key = buffer.front();
		buffer.pop_front();
		return Key;
	}
	std::list<std::tuple<bool, char>> GetChars(){
		UpdateKeymap();
		if (buffer.empty()) return std::list<std::tuple<bool, char>> {std::tuple<bool, char> {false, 0}};
		std::list<std::tuple<bool, char>> Keys = buffer;
		buffer.clear();
		return Keys;
	}
protected:
	std::list<std::list<uint32_t>> KeyMap{};
	std::list<std::tuple<bool, char>> buffer{};
	std::list<uint16_t> Columns_GPIO_Pins;
	std::list<uint16_t> Rows_GPIO_Pins;
	GPIO_TypeDef* GPIOx;
private:
	uint32_t LastUpdate;
};
