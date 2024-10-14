/*
*
* Barkout - Copyright 2024 Stephen Branley.
*
* This file is part of Barkout.
*
* Barkout is free software: you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation, either version 3 of the License, 
* or (at your option) any later version.
*
* Barkout is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*
*/


#ifndef _MENU_H_
#define _MENU_H_

#include "app.h"
#include "text.h"
#include <stdbool.h>

typedef struct
{
  const char* text;
  int value;
  const char* description;
} MenuItemOption;

typedef struct
{
  const char* text;
  int selectedvalue;
  int selectedindex;
  int optioncount;
  int* controlvalue;
  void (*onexecute)(void*);
  void (*ontoggle)(void*, void*);
  MenuItemOption** options;
} MenuItem;


typedef struct
{
  MenuItem** items;
  int selectedindex;
  int itemcount;
  int optionx;
  int x;
  int y;
  App* app;
} Menu;

MenuItem* menu_addItem(Menu* menu, const char* text, int* controlvalue, void (*onexecute)(void*), void (*ontoggle)(void*, void*));
MenuItemOption* menu_addItemOption(MenuItem* menuitem, const char* optiontext, const char* optiondescription, int optionvalue);
void menu_drawMenu(Menu* menu, App* app);
void menu_next(Menu* menu);
void menu_previous(Menu* menu);
void menu_nextOption(Menu* menu);
void menu_previousOption(Menu* menu);
void menu_free(Menu* menu);
void menu_execute(Menu* menu);

#endif // _MENU_H_
