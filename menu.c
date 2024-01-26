#include "menu.h"
#include "config.h"

MenuItem* menu_additem(Menu* menu, const char* text)
{
  MenuItem* item = malloc(sizeof(MenuItem));
  item->options = NULL;
  item->text = text;
  item->selectedvalue = 0;
  item->selectedindex = 0;
  item->optioncount = 0;
  menu->itemcount++;
  menu->items = realloc(menu->items, sizeof(MenuItem*)*menu->itemcount);
  menu->items[menu->itemcount-1] = item;
  return item;
}

MenuItemOption* menu_additemoption(MenuItem* menuitem, const char* optiontext, const char* optiondescription, int optionvalue)
{
  MenuItemOption* option = malloc(sizeof(*option));
  option->text = optiontext;
  option->description = optiondescription;
  option->value = optionvalue;
  menuitem->optioncount++;
  menuitem->options = realloc(menuitem->options, sizeof(MenuItemOption*)*menuitem->optioncount);
  menuitem->options[menuitem->optioncount-1] = option;
}

void menu_drawmenu(Menu* menu, App* app)
{
  for(int i = 0; i < menu->itemcount; i++)
  {
    MenuItem* item = menu->items[i];
    if(i==menu->selectedindex)
      text_drawtext(app, item->text, menu->x, 40*i + menu->y, (SDL_Color){200, 200, 255, 255}, 0);
    else
      text_drawtext(app, item->text, menu->x, 40*i + menu->y, (SDL_Color){255, 255, 255, 100}, 0);

    //for(int j = 0; j < menu->items[i]->optioncount; j++)
    //{
      //if(menu->items[i]->selectedvalue == menu->items[i]->options[j]->value)
      //{
      if(item->optioncount > 0)
      {
        text_drawtext(app, item->options[item->selectedindex]->text, menu->optionx, 40*i + menu->y, (SDL_Color){255, 255, 255, 100}, 0);
        if(i==menu->selectedindex)
          text_drawwrappedtext(app, item->options[item->selectedindex]->description, menu->x, 500, (SDL_Color){255, 255, 255, 100}, 0, 20);
      }
      //}
    //}
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

void menu_nextoption(Menu* menu)
{
  MenuItem* item = menu->items[menu->selectedindex];
  item->selectedindex++;
  if(item->selectedindex == item->optioncount)
    item->selectedindex = 0;
}

void menu_previousoption(Menu* menu)
{
}

void menu_free(Menu* menu)
{
}
