#ifndef _ARENA_H_
#define _ARENA_H_

#include "resourcefactory.h"
#include "bounds.h"
#include "bonus.h"
#include "bat.h"
#include "ball.h"

#define BRICKW 40
#define BRICKH 25

typedef enum { bsStatic, bsHit } Brickstate;
typedef enum { btNormal, btHard, btIndestructible } Bricktype;
typedef enum { eLeft, eRight, eTop, eBottom, eTopLeft, eTopRight, eBottomLeft, eBottomRight, eNone } Edge;
typedef enum { gsNewLevel, gsGetReady, gsRunning, gsDying, gsPaused } Gamestate;

typedef struct
{
  Sprite* sprite;
  int left;
  int top;
  int right;
  int bottom;
  int hitcount;
  int frame;
  Bricktype type;
  Uint32 lastticks;
} Brick;

typedef struct
{
  Bounds bounds;
  int width;
  int brickcount;
  long bonuscounter;
  int bonuscount;
  ResourceFactory* factory;
  Brick** bricks;
  Bonus** bonuses;
} Arena;

// Level related functions
int arena_loadbricks(Arena* arena, ResourceFactory* factory, const char* fname);
int arena_drawbricks(Arena* arena, SDL_Renderer* renderer);
int arena_freebricks(Arena* arena);

// Bonus related functions
Bonus* arena_addbonus(Arena* arena, int x, int y, Bonustype type);
int arena_movebonuses(Arena* arena, Bat* player);
int arena_freebonus(Arena* arena, Bonus* bonus);
int arena_freebonuses(Arena* arena);
int arena_drawbonuses(Arena* arena, SDL_Renderer* renderer);
Bonus* arena_batcollidesbonus(Arena* arena, Bat* player, Ball* ball);

// Events
void bat_aftershrink(void* sender, void* data);

// Ball related functions
int ball_moveball(Ball* ball, Arena* arena, Bat* player);
Brick* ball_collidesbricks(Arena* arena, Ball* ball, Edge* e);
int ball_collidesbat(Ball* ball, Bat* player, Edge* e);

#endif
