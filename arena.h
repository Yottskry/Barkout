#ifndef _ARENA_H_
#define _ARENA_H_

#include "brick.h"
#include "animationfactory.h"
#include <SDL.h>

#define MAXCOLS 13

typedef enum {eLeft, eRight, eTop, eBottom, eTopLeft, eTopRight, eBottomLeft, eBottomRight, eNone} Edge;

typedef enum {gsNewLevel, gsGetReady, gsRunning, gsDying} Gamestate;

typedef struct
{
  int top;
  int bottom;
  int left;
  int right;
  int width;
  int rowcount;
  int colcount;
  int brickcount;
  Brick** bricks;
} Arena;

int arena_loadbricks(Arena* arena, AnimationFactory* factory, const char* fname);
int arena_drawbricks(Arena* arena, SDL_Renderer* renderer);
int arena_freebricks(Arena* arena);

#endif
