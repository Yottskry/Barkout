#ifndef _BRICK_H_
#define _BRICK_H_

#define MAXBRICKPARTICLES 255
#define BRICKDECAY 25
#define RESURRECTTIMER 600
#define EXPLOSIONPARTICLES 200
#define BRICKSPEED 3

#include "resourcefactory.h"
#include <stdbool.h>

typedef enum { bsStatic, bsHit } Brickstate;
// Despite these being mostly mutually exclusive, it's possible for any type of brick to be moving
typedef enum { btNormal = 1, btHard = 2, btIndestructible = 4, btWormhole = 8, btResurrecting = 16, btMoving = 32 } Bricktype;
typedef enum { eNone = 0, eLeft = 1, eRight = 2, eTop = 4, eBottom = 8, eTopLeft = 16, eTopRight = 32, eBottomLeft = 64, eBottomRight = 128 } Edge;

// Used by Bricks and the Ball.
typedef struct
{
  int x;
  int y;
  Uint16 alpha;
  Uint32 gdiff;
  int xv;
  int yv;
} Sparkle;

typedef struct
{
  Uint32 startdelay;
  Uint32 starttime;
  bool isdead;
  Sparkle particles[EXPLOSIONPARTICLES];
  int x;
  int y;
  SDL_Color color;
} Explosion;

typedef struct
{
  Sprite* sprite;
  int left;
  int top;
  int right;
  int bottom;
  int hitcount;
  int starthitcount;
  int frame;
  bool isdead;
  Bricktype type;
  Uint32 lastticks;
  Edge solidedges;
  int counter;
  int speed;
  Sparkle particles[MAXBRICKPARTICLES];
} Brick;

#endif
