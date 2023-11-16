#ifndef _ARENA_H_
#define _ARENA_H_

#include "brick.h"
#include "animationfactory.h"
#include <SDL.h>

#define MAXCOLS 13

typedef struct
{
  int top;
  int bottom;
  int left;
  int right;
  int rowcount;
  int colcount;
  int brickcount;
  Brick** bricks;

} Arena;

int arena_loadbricks(Arena* arena, AnimationFactory* factory, const char* fname);
int arena_drawbricks(Arena* arena, SDL_Renderer* renderer);
int arena_freebricks(Arena* arena);

#endif
