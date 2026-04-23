/*
 * Menu.cpp
 *
 *  Created on: 11 апр. 2026 г.
 *      Author: Ideapad-43
 */


#include "Menu.h"

class Menu {
public:
	Menu(uint8_t screenLength, uint8_t offset = 0, std::list<std::string> menuItems = {}){
		this->screenLength = screenLength;
		this->offset = offset >= screenLength ? screenLength - 1 : offset;
		this->cursor = -this->offset;
		this->menuItems = menuItems;
	}
	void moveCursor(uint8_t distance = 1){
		// Handling moving cursor up (down on the screen)
		if (distance > 0 && ((cursor + offset + distance) >= (menuItems.size() - 1))) {
			cursor = menuItems.size() - 1 - offset;
		}
		// Handling moving cursor down (up on the screen)
		else if (distance < 0 && ((cursor + offset + distance) <= 0)) {
			cursor = -offset;
		}
		// We can add zero check here but this will only slow down the process
		else cursor += distance;
	}
	void addItem(std::string Item){
		menuItems.push_back(Item);
	}
	void addItems(std::list<std::string> Items){
		menuItems.splice(menuItems.end(), Items);
	}
	// Return all items that are currently displayed
	std::list<std::string> getItems(bool createEmptyItems = true){
		uint8_t itemsLeft = screenLength;
		std::list<std::string> items {};
		auto iter = menuItems.begin();
		int listPointer {cursor};

		// Skipping all negative items
		while (listPointer < 0 && itemsLeft){
			if (createEmptyItems) items.push_back("");
			listPointer++;
			itemsLeft--;
		}

		// Moving iter to the first displayed item
		for (int i = 0; i < cursor; i++) iter++;

		// Getting items
		while (itemsLeft) {
			if (listPointer >= menuItems.size()){
				if (createEmptyItems) items.push_back("");
				itemsLeft--;
				//listPointer++; // We can ignore it, it will not affect the workflow
			}
			else{
				items.push_back(*iter);
				itemsLeft--;
				listPointer++;
				iter++;
			}
		}

		return items;
	}

	//Return selected item
	std::string getItem(){
		auto iter = menuItems.begin();
		int listPointer = cursor + offset;

		for (int i = 0; i < listPointer; i++) iter++;

		return *iter;
	}
private:
	std::list<std::string> menuItems;
	int8_t cursor;
	uint8_t offset; // Used to offset selection from the top of the screen
	uint8_t screenLength;
};
