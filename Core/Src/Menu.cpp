/*
 * Menu.cpp
 *
 *  Created on: 11 апр. 2026 г.
 *      Author: Ideapad-43
 */


#include "Menu.h"

class Menu {
public:
	std::list<std::string> MenuItems;
	void Menu(uint8_t screenLength, std::list<std::string> MenuItems = {}){
		cursor={};
		this->screenLength = screenLength;
		this->MenuItems = MenuItems;
	}
	void MoveCursor(uint8_t distance = 1){
		// Handling moving cursor up (down on the screen)
		if (distance > 0 && ((cursor + distance) >= (MenuItems.size() - 1))) {
			cursor = MenuItems.size() - 1;
		}
		// Handling moving cursor down (up on the screen)
		else if (distance < 0 && ((cursor + distance) <= (1 - screenLength))) {
			cursor = screenLength - 1;
		}
		// We can add zero check here but this probably will only slow down the process
		else cursor += distance;
	}
	void AddItem(std::string Item){
		MenuItems.push_back(Item);
	}
	void AddItems(std::list<std::string> Items){
		MenuItems.splice(MenbuItems.end(), Items);
	}
	// Return Items to display on screen
	void GetItems(){
		for (int i=0;i<screenLength;i++){

		}
	}
	// Used to get Items from the "screen" (in commas because it's not a real screen)
	// If offset > screenLength then returns nothing (STM hates error handling)
	std::string GetItem(uint8_t offset = 0){

	}
private:
	int8_t cursor;
	uint8_t screenLength;
};


// OUTDATED/UNUSED
/*
class Menu{
public:
	std::list<std::string> MenuItems;
	uint8_t Cursor;
	uint8_t Offset;
	Menu(std::list<std::string> MenuItems, uint8_t Offset = 0){
		this->MenuItems=MenuItems;
		Cursor=0;
		this->Offset=Offset;
	}
	std::list<std::string> GetItemsToDisplay(uint8_t amount){
		std::list<std::string> ItemsToDisplay {};
		if (amount < 1 || Cursor + Offset + amount >= MenuItems.size()) return ItemsToDisplay;
		auto iter = MenuItems.begin();
		for (int i=0;i<Cursor+Offset;i++) iter++;
		for (int i=0; i<amount;i++)
		{
			ItemsToDisplay.push_back(*iter);
			iter++;
		}
		ItemsToDisplay.resize(amount, ""); // For the compatibility with the LCD display (menu may be LESS than the amount)
		return ItemsToDisplay;
	}
	void MoveUp() {
		if (Cursor+Offset < (MenuItems.size() - 1)) {
			Cursor++;
		}
	}
	void MoveDown(){
		if (Cursor+Offset > 0) Cursor--;
	}
	std::string GetCurrentItem() {
		return ItemsToDisplay[Cursor+Offset];
	}
};
*/
