#ifndef _TEXT_H_
#define _TEXT_H_

#include "app.h"

typedef struct
{
  char* text;
  int duration;
  Uint8 alpha;
  Uint8 targetalpha;
} FlashText;

void text_drawtext(App* app, const char* text, int x, int y, SDL_Color color);

void text_drawflashtext(App* app, FlashText* text, int x, int y);

#endif
