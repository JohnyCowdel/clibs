/*
 * lcd_menu.hpp
 *
 *  Created on: Jan 3, 2024
 *      Author: jajtn
 */

#include "stdint.h"

#ifndef LCD_MENU_HPP_
#define LCD_MENU_HPP_

#define ITEMS_IN_MENU	20
#define MAX_ITEMS		(ITEMS_IN_MENU - 1)
#define MIN_ITEMS		0


typedef struct
{
	uint8_t scroll:1;
	uint8_t showParam;
	uint8_t overlapParam;

}itemConfig_t;


class MenuItem
{
	MenuItem();
	MenuItem(const char *name);
	MenuItem(const char *name, void (*ptr)());
public:
	void onClick();
	void onFocus();
private:

	void init_m();
	void (*onClick_ptr)(void);
	char itemName_m[40];
	int8_t overChar_m;
	int8_t startChar_m;
	char itemParam[5];
	itemConfig_t config_m;

};


class Menu
{
	Menu();
public:
	void addItem(MenuItem *item_);
	void select();
	void update();
	void up();
	void down();
private:
	int8_t currentIndex_m;
	MenuItem *items_m[ITEMS_IN_MENU];
	uint8_t itemNo_m;
};



#endif /* LCD_MENU_HPP_ */
