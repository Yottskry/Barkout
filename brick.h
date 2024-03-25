#ifndef _BRICK_H_
#define _BRICK_H_

#define MAXBRICKPARTICLES 255
#define BRICKDECAY 25
#define RESURRECTTIMER 600

#include "resourcefactory.h"

typedef enum { bsStatic, bsHit } Brickstate;
// Not sure if having these as flags is really beenficial as you'll never be in the position
// of something being both normal and indestructible, for example.
typedef enum { btNormal = 1, btHard = 2, btIndestructible = 4, btWormhole = 8, btResurrecting = 16 } Bricktype;
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
  Sprite* sprite;
  int left;
  int top;
  int right;
  int bottom;
  int hitcount;
  int starthitcount;
  int frame;
  Bricktype type;
  Uint32 lastticks;
  Edge solidedges;
  int counter;
  Sparkle particles[MAXBRICKPARTICLES];
} Brick;

#endif
