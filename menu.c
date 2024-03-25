#include "menu.h"
#include "config.h"

MenuItem* menu_additem(Menu* menu, const char* text, int* controlvalue, void (*onexecute)(void*), void (*ontoggle)(void*, void*))
{
  MenuItem* item = malloc(sizeof(MenuItem));
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
  if(option->value == menuitem->selectedvalue)
    menuitem->selectedindex = menuitem->optioncount-1;
  return option;
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

    if(item->optioncount > 0)
    {
      text_drawtext(app, item->options[item->selectedindex]->text, menu->optionx, 40*i + menu->y, (SDL_Color){255, 255, 255, 255}, 0);
      if(i==menu->selectedindex)
        text_drawwrappedtext(app, item->options[item->selectedindex]->description, menu->x, 500, (SDL_Color){255, 255, 255, 255}, 0, 600);
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

void menu_nextoption(Menu* menu)
{
  MenuItem* item = menu->items[menu->selectedindex];
  item->selectedindex++;
  if(item->selectedindex == item->optioncount)
    item->selectedindex = 0;
  item->selectedvalue = item->options[item->selectedindex]->value;
  *(item->controlvalue) = item->selectedvalue;

  if(item->ontoggle != NULL)
    item->ontoggle((void*)menu->app, (void*)item);
}

void menu_previousoption(Menu* menu)
{
  MenuItem* item = menu->items[menu->selectedindex];
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
