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


#ifndef _TEXT_H_
#define _TEXT_H_

#include "app.h"
#include "config.h"
#include <assert.h>
#include <stdbool.h>

typedef enum { TEXT_CENTRED = 1, TEXT_ARENA_CENTRED = 2, TEXT_SHADOW = 4 } TextFlags;
typedef enum { fnTitle = 0, fnBody = 1, fnStory = 2, fnMenu = 3, fnSmallBody = 4} FontType;

typedef struct
{
  char* text;
  int duration;
  Uint8 alpha;
  Uint8 targetalpha;
  FontType font;
} FlashText;

typedef struct
{
  char* texts[4];
  int count;
  int current;
} FlashStory;

void text_drawText(App* app, const char* text, int x, int y, SDL_Color color, int flags, FontType fonttype);

void text_drawWrappedText(App* app, const char* text, int x, int y, SDL_Color color, int flags, int len, FontType fonttype);

bool text_drawFlashText(App* app, FlashText* text, int x, int y, int speed);

bool text_drawFlashStory(App* app, FlashStory* story, FlashText* text, int y);

void text_loadFonts(App* app);

void text_freeFonts(App* app);

#endif
