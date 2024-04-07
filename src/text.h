#ifndef _TEXT_H_
#define _TEXT_H_

#include "app.h"
#include "config.h"
#include <assert.h>
#include <stdbool.h>

typedef enum { TEXT_CENTRED = 1, TEXT_ARENA_CENTRED = 2 } TextFlags;
typedef enum { fnTitle = 0, fnBody = 1, fnStory = 2 } FontType;

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

void text_drawtext(App* app, const char* text, int x, int y, SDL_Color color, int flags, FontType fonttype);

void text_drawwrappedtext(App* app, const char* text, int x, int y, SDL_Color color, int flags, int len, FontType fonttype);

bool text_drawflashtext(App* app, FlashText* text, int x, int y, int speed);

bool text_drawflashstory(App* app, FlashStory* story, FlashText* text, int y);

void text_loadfonts(App* app);

void text_freefonts(App* app);

#endif
