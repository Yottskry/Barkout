#ifndef _CAT_H_
#define _CAT_H_

#include "bounds.h"
#include "brick.h"
#include "resourcefactory.h"

// The bad guys!

typedef enum { dUp, dDown, dLeft, dRight } Direction;
typedef enum { csAlive, csDead } CatState;

typedef struct
{
  Sprite sprite;
  Bounds bounds; // Need to get away from separate x, y, w, h, etc
  int speed;
  Direction nextdirection;
  CatState state;
} Cat;


Cat* cat_create();
void cat_init(Cat* cat, ResourceFactory* f);
void cat_free(Cat* cat);
void cat_move(Cat* cat, Brick** bricks, int brickcount, Bounds* bounds);
void cat_draw(Cat* cat, SDL_Renderer* renderer);
#endif // _CAT_H_
