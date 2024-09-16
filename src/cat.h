#ifndef _CAT_H_
#define _CAT_H_

#include "bounds.h"
#include "brick.h"
#include "ball.h"
#include "resourcefactory.h"

#define FIRSTBADDIE 25000
#define NEXTBADDIE 15000

// The bad guys!

typedef enum { dUp, dDown, dLeft, dRight } Direction;
typedef enum { csSpawning, csAlive, csDying, csDead } CatState;

typedef struct
{
  Sprite sprite;
  Bounds bounds; // Need to get away from separate x, y, w, h, etc
  int spawnx;
  int spawny;
  int speed;
  Direction nextdirection;
  CatState state;
} Cat;

void cat_init(Cat* cat, ResourceFactory* f);
void cat_move(Cat** cats, int count, Brick** bricks, int brickcount, Bounds* bounds);
void cat_draw(Cat** cats, int count, SDL_Renderer* renderer);
void cat_spawn(Cat** cats, int count, ResourceFactory* factory);
bool cat_collidesball(Cat** cats, int count, Ball* ball, ResourceFactory* factory);
bool cat_collidesbat(Cat** cats, int count, Bounds* bounds, ResourceFactory* factory);
void cat_afterdie(void* sender, void* data);
void cat_afterspawn(void* sender, void* data);

#endif // _CAT_H_
