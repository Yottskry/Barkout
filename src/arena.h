#ifndef _ARENA_H_
#define _ARENA_H_

#include "resourcefactory.h"
#include "bounds.h"
#include "bonus.h"
#include "bat.h"
#include "ball.h"
#include "app.h"
#include "brick.h"
#include "cat.h"
#include "levels.h"
#include "vector.h"
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <math.h>

#define MAXBULLETS 10
#define MAXSPEED 12
#define NUMEXPLOSIONS 40

typedef unsigned char byte;

typedef struct
{
  int x;
  int y;
  int speed;
} Bullet;

typedef struct
{
  Bounds bounds;
  int width;
  int brickcount;
  long bonuscounter;
  int bonuscount;
  int remaining;
  int bulletcount;
  int level;
  Animation* bg;
  Animation* mg;
  Animation* fg;
  Uint32 counter;
  ResourceFactory* factory;
  Brick** bricks;
  Bonus** bonuses;
  Bullet** bullets;
  int numlevels;
  Level* levels;
  Explosion explosions[NUMEXPLOSIONS];
  Uint8 alpha;
  int multiplier;
} Arena;

// Level related functions
int arena_loadBinary(ResourceFactory* factory, Arena* arena, char* fname);
int arena_loadLevels(Arena* arena, ResourceFactory* factory);

void arena_loadBricks(Arena* arena, int level);
void arena_drawBricks(Arena* arena, SDL_Renderer* renderer);
void arena_moveBricks(Arena* arena, Ball* ball);
void arena_resetBricks(Arena* arena);
void arena_freeLevels(Arena* arena);

// Bonus related functions - mostly moved to bonus.h
Bonus* arena_addBonus(Arena* arena, int x, int y, Bonustype type);
Bonus* arena_batCollidesBonus(Arena* arena, Bat* player, Ball* ball);

// Events
void bat_afterShrink(void* sender, void* data);
void bat_afterGrow(void* sender, void* data);
void bat_afterLaser(void* sender, void* data);

// Ball related functions
int ball_moveBall(Ball* ball, Arena* arena, Bat* player);
int ball_collidesBat(Ball* ball, Bat* player, Edge* e);

// Bullet related functions
void arena_addBullet(Arena* arena, Bat* player);
void arena_moveBullets(Arena* arena);
void arena_drawBullets(Arena* arena, SDL_Renderer* renderer);
void arena_freeBullet(Arena* arena, int index);
void arena_freeBullets(Arena* arena);
void arena_checkBulletCollisions(Arena* arena, Bat* player);

void arena_finalLevelEnd(void* sender);
void arena_resetExplosions(Arena* arena);
bool arena_drawExplosions(Arena* arena, SDL_Renderer* renderer);

#endif
