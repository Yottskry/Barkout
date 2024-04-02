#ifndef _TEXT_H_
#define _TEXT_H_

#include "app.h"
#include <assert.h>
#include <stdbool.h>

typedef enum { TEXT_CENTRED = 1, TEXT_ARENA_CENTRED = 2 } TextFlags;

typedef struct
{
  char* text;
  int duration;
  Uint8 alpha;
  Uint8 targetalpha;
} FlashText;

typedef struct
{
  char* texts[4];
  int count;
  int current;
} FlashStory;

void text_drawtext(App* app, const char* text, int x, int y, SDL_Color color, int flags);

void text_drawwrappedtext(App* app, const char* text, int x, int y, SDL_Color color, int flags, int len);

void text_drawbgtext(App* app, const char* text, int x, int y, SDL_Color color, SDL_Color bgcolor, int flags);

bool text_drawflashtext(App* app, FlashText* text, int x, int y, int speed);

bool text_drawflashstory(App* app, FlashStory* story, FlashText* text, int y);

#endif
