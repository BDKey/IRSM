/*
 * Menu.cpp
 *
 *  Created on: 11 апр. 2026 г.
 *      Author: Ideapad-43
 */


#include "Menu.h"

class Menu{
public:
	std::list<std::string> MenuItems;
	uint8_t Cursor;
	Menu(std::list<std::string> MenuItems){
		this->MenuItems=MenuItems;
		Cursor=0;
	}
	std::list<std::string> GetItemsToDisplay(uint8_t amount){
		std::list<std::string> ItemsToDisplay {};
		if (amount < 1 || Cursor + amount >= MenuItems.size()) return ItemsToDisplay;
		auto iter = MenuItems.begin();
		for (int i=0;i<Cursor;i++) iter++;
		for (int i=0; i<amount;i++)
		{
			ItemsToDisplay.push_back(*iter);
			iter++;
		}
		ItemsToDisplay.resize(amount, ""); // For the compatibility with the LCD display (menu may be LESS than amount)
		return ItemsToDisplay;
	}
	void MoveUp() {
		if (Cursor < (MenuItems.size() - 1)) {
			Cursor++;
		}
	}
	void MoveDown(){
		if (Cursor > 0) Cursor--;
	}
};
