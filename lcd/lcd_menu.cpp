/*
 * lcd_menu.cpp
 *
 *  Created on: Jan 3, 2024
 *      Author: jajtn
 */

#include "lcd_menu.hpp"
#include "i2c_lcd.hpp"
#include "../utilities/utilities.h"

void MenuItem::init_m()
{
	config_m.scroll = 0;
	config_m.showParam = 0;
	config_m.overlapParam = 0;
	startChar_m = 0;
}

MenuItem::MenuItem()
{
	init_m();
}

MenuItem::MenuItem(const char *name)
{
	init_m();
}


MenuItem::MenuItem(const char *name, void (*ptr)())
{
	init_m();
	onClick_ptr = ptr;
}

void MenuItem::onFocus()
{

}


Menu::Menu()
{
	currentIndex_m = 0;
	itemNo_m = 0;
}

void Menu::addItem(MenuItem *item_)
{
	if(item_==nullptr)
		return;

	if(itemNo_m < MAX_ITEMS)
	{
		items_m[itemNo_m] = item_;
		itemNo_m++;
	}
}

void Menu::deleteItem(MenuItem *item_)
{
	if(item_ == nullptr)
		return;

	if(itemNo_m == 0)
		return;

	for(uint8_t i=0; i<itemNo_m; i++)	// find item
	{
		if(items_m[i] == item_)
			break;
	}

	for(uint8_t k=i; k<itemNo_m-1;k++)	// reorder items
		items_m[k] = itmes_m[k+1];

}

void Menu::up()
{
	currentIndex_m++;
	if(currentIndex_m > MAX_ITEMS)
		currentIndex_m = 0;
}

void Menu::down()
{
	currentIndex_m--;
	if(currentIndex_m < 0)
		currentIndex_m = MAX_ITEMS;
}

void Menu::select()
{
	items_m[currentIndex_m]->onClick();
}

void Menu::update()
{

}





