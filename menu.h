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
} Menu;

MenuItem* menu_additem(Menu* menu, const char* text);
MenuItemOption* menu_additemoption(MenuItem* menuitem, const char* optiontext, const char* optiondescription, int optionvalue);
void menu_drawmenu(Menu* menu, App* app);
void menu_next(Menu* menu);
void menu_previous(Menu* menu);
void menu_nextoption(Menu* menu);
void menu_previousoption(Menu* menu);
void menu_free(Menu* menu);

#endif // _MENU_H_
