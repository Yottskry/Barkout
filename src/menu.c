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


#include "menu.h"
#include "config.h"

MenuItem* menu_addItem(Menu* menu, const char* text, int* controlvalue, void (*onexecute)(void*), void (*ontoggle)(void*, void*))
{
  MenuItem* item = malloc(sizeof(MenuItem));
  TEST_ALLOC(item)
  item->options = NULL;
  item->text = text;
  if(controlvalue != NULL)
    item->selectedvalue = *controlvalue;
  else
    item->selectedvalue = 0;
  item->selectedindex = 0;
  item->optioncount = 0;
  item->controlvalue = controlvalue;
  item->onexecute = onexecute;
  item->ontoggle = ontoggle;
  menu->itemcount++;
  menu->items = realloc(menu->items, sizeof(MenuItem*)*menu->itemcount);
  TEST_ALLOC(menu->items)
  menu->items[menu->itemcount-1] = item;
  return item;
}

MenuItemOption* menu_addItemOption(MenuItem* menuitem, const char* optiontext, const char* optiondescription, int optionvalue)
{
  MenuItemOption* option = malloc(sizeof(*option));
  TEST_ALLOC(option)
  option->text = optiontext;
  option->description = optiondescription;
  option->value = optionvalue;
  menuitem->optioncount++;
  menuitem->options = realloc(menuitem->options, sizeof(MenuItemOption*)*menuitem->optioncount);
  TEST_ALLOC(menuitem->options)
  menuitem->options[menuitem->optioncount-1] = option;
  if(option->value == menuitem->selectedvalue)
    menuitem->selectedindex = menuitem->optioncount-1;
  return option;
}

void menu_drawMenu(Menu* menu, App* app)
{
  for(int i = 0; i < menu->itemcount; i++)
  {
    MenuItem* item = menu->items[i];
    if(i==menu->selectedindex)
      text_drawText(app, item->text, menu->x, 35*i + menu->y, (SDL_Color){200, 200, 255, 255}, 0, fnMenu);
    else
      text_drawText(app, item->text, menu->x, 35*i + menu->y, (SDL_Color){255, 255, 255, 100}, 0, fnMenu);

    if(item->optioncount > 0)
    {
      text_drawText(app, item->options[item->selectedindex]->text, menu->optionx, 35*i + menu->y, (SDL_Color){255, 255, 255, 255}, 0, fnMenu);
      if(i==menu->selectedindex)
        text_drawWrappedText(app, item->options[item->selectedindex]->description, menu->x, 500, (SDL_Color){255, 255, 255, 255}, 0, 600, fnBody);
    }
  }
}

void menu_next(Menu* menu)
{
  menu->selectedindex++;
  if(menu->selectedindex == menu->itemcount)
    menu->selectedindex = 0;
}

void menu_previous(Menu* menu)
{
  menu->selectedindex--;
  if(menu->selectedindex < 0)
    menu->selectedindex = menu->itemcount-1;
}

void menu_nextOption(Menu* menu)
{
  MenuItem* item = menu->items[menu->selectedindex];

  if(item->optioncount == 0)
    return;

  item->selectedindex++;
  if(item->selectedindex == item->optioncount)
    item->selectedindex = 0;
  item->selectedvalue = item->options[item->selectedindex]->value;
  *(item->controlvalue) = item->selectedvalue;

  if(item->ontoggle != NULL)
    item->ontoggle((void*)menu->app, (void*)item);
}

void menu_previousOption(Menu* menu)
{
  MenuItem* item = menu->items[menu->selectedindex];

  if(item->optioncount == 0)
    return;

  item->selectedindex--;
  if(item->selectedindex < 0)
    item->selectedindex = item->optioncount - 1;
  item->selectedvalue = item->options[item->selectedindex]->value;
  *(item->controlvalue) = item->selectedvalue;

  if(item->ontoggle != NULL)
    item->ontoggle((void*)menu->app, (void*)item);
}

void menu_free(Menu* menu)
{
  for(int i = 0; i < menu->itemcount; i++)
  {
    for(int j = 0; j < menu->items[i]->optioncount; j++)
    {
      free(menu->items[i]->options[j]);
    }
    free(menu->items[i]->options);
    free(menu->items[i]);
  }
  free(menu->items);
}

void menu_execute(Menu* menu)
{
  MenuItem* item = menu->items[menu->selectedindex];
  if(item->onexecute != NULL)
  {
    item->onexecute((void*)menu->app);
  }
}
