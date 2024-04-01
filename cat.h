#ifndef _CAT_H_
#define _CAT_H_

#include "bounds.h"
#include "brick.h"
#include "ball.h"
#include "resourcefactory.h"

#define FIRSTBADDIE 25000
#define NEXTBADDIE 15000
#define BADDIECOUNT 3
// The bad guys!

typedef enum { dUp, dDown, dLeft, dRight } Direction;
typedef enum { csSpawning, csAlive, csDying, csDead } CatState;

typedef struct
{
  Sprite sprite;
  Bounds bounds; // Need to get away from separate x, y, w, h, etc
  int speed;
  Direction nextdirection;
  CatState state;
} Cat;


typedef struct
{
  int count;
  Cat** cats;
} Cattery; // a cat factory :D

void cat_init(Cat* cat, ResourceFactory* f);
void cat_move(Cat* cat, Brick** bricks, int brickcount, Bounds* bounds);
void cat_draw(Cat* cat, SDL_Renderer* renderer);
void cat_spawn(Cat* cats, ResourceFactory* factory, int x, int y);
bool cat_collidesball(Cat* cats, Ball* ball, ResourceFactory* factory);
bool cat_collidesbat(Cat* cats, Bounds* bounds, ResourceFactory* factory);
void cat_afterdie(void* sender, void* data);
void cat_afterspawn(void* sender, void* data);

Cat* cat_addcat(Cattery* cattery, ResourceFactory* f);
void cat_removecat(Cattery* cattery);

#endif // _CAT_H_
