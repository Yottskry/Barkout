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

MenuItem* menu_additem(Menu* menu, const char* text, int* controlvalue, void (*onexecute)(void*));
MenuItemOption* menu_additemoption(MenuItem* menuitem, const char* optiontext, const char* optiondescription, int optionvalue);
void menu_drawmenu(Menu* menu, App* app);
void menu_next(Menu* menu);
void menu_previous(Menu* menu);
void menu_nextoption(Menu* menu);
void menu_previousoption(Menu* menu);
void menu_free(Menu* menu);
void menu_execute(Menu* menu);

#endif // _MENU_H_
