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
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <math.h>

#define BRICKW 40
#define BRICKH 25
#define MAXBULLETS 10
#define MAXSPEED 12
#define BRICKSCORE 10
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
  int level;
  int brickcount;
  int catterycount;
  int spawnx;
  int spawny;
  int maxbonuslevel;
  Animation* bg;
  Animation* mg;
  Animation* fg;
  Brick** bricks;
  Cattery** catteries;
  void (*onlevelend)(void*);
} Level;

typedef struct
{
  Bounds bounds;
  int width;
  int brickcount;
  long bonuscounter;
  int bonuscount;
  int score;
  int remaining;
  int bulletcount;
  int lives; // Signed because we will reduce to -1 before saying Game Over
  int level;
  int spawnx;
  int spawny;
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
int arena_loadlevels(Arena* arena, ResourceFactory* factory);

void arena_loadbricks(Arena* arena, int level);
void arena_drawbricks(Arena* arena, SDL_Renderer* renderer);
void arena_resetbricks(Arena* arena);
void arena_freelevels(Arena* arena);

// Bonus related functions - mostly moved to bonus.h
Bonus* arena_addbonus(Arena* arena, int x, int y, Bonustype type);
Bonus* arena_batcollidesbonus(Arena* arena, Bat* player, Ball* ball);

// Events
void bat_aftershrink(void* sender, void* data);
void bat_aftergrow(void* sender, void* data);
void bat_afterlaser(void* sender, void* data);

// Ball related functions
int ball_moveball(Ball* ball, Arena* arena, Bat* player);
int ball_collidesbat(Ball* ball, Bat* player, Edge* e);

// Bullet related functions
void arena_addbullet(Arena* arena, Bat* player);
void arena_movebullets(Arena* arena);
void arena_drawbullets(Arena* arena, SDL_Renderer* renderer);
void arena_freebullet(Arena* arena, int index);
void arena_freebullets(Arena* arena);
void arena_checkbulletcollisions(Arena* arena);

void arena_drawlives(Arena* arena, App* app);
void arena_brickfinished(void* sender, void* data);
void arena_brickrepaired(void* sender, void* data);

void arena_finallevelend(void* sender);
void arena_resetexplosions(Arena* arena);
bool arena_drawexplosions(Arena* arena, SDL_Renderer* renderer);

#endif
