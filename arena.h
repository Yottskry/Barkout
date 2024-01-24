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
#include <SDL_mixer.h>
#include <stdio.h>
#include <math.h>

#define BRICKW 40
#define BRICKH 25
#define NUMLEVELS 9
#define MAXBULLETS 10
#define MAXSPEED 12
#define BRICKSCORE 100

typedef unsigned char byte;

typedef enum { gsTitle, gsStory, gsNewLevel, gsGetReady, gsRunning, gsDying, gsPaused } Gamestate;

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
  int spawnx;
  int spawny;
  Animation* bg;
  Animation* mg;
  Animation* fg;
  Brick** bricks;
} Level;

typedef struct
{
  Bounds bounds;
  unsigned int width;
  unsigned int brickcount;
  long bonuscounter;
  unsigned int bonuscount;
  unsigned int score;
  unsigned int remaining;
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
  Level levels[NUMLEVELS];
} Arena;

// Level related functions
int arena_loadlevels(Arena* arena, ResourceFactory* factory);

void arena_loadbricks(Arena* arena, int level);
void arena_drawbricks(Arena* arena, SDL_Renderer* renderer);
void arena_resetbricks(Arena* arena);
void arena_freelevels(Arena* arena);

// Bonus related functions
Bonus* arena_addbonus(Arena* arena, int x, int y, Bonustype type);
int arena_movebonuses(Arena* arena);
int arena_freebonus(Arena* arena, Bonus* bonus);
int arena_freebonuses(Arena* arena);
int arena_drawbonuses(Arena* arena, SDL_Renderer* renderer);
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

#endif
