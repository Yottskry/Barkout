#ifndef _BRICK_H_
#define _BRICK_H_

#define MAXBRICKPARTICLES 255
#define BRICKDECAY 25

#include "resourcefactory.h"

typedef enum { bsStatic, bsHit } Brickstate;
typedef enum { btNormal, btHard, btIndestructible, btWormhole } Bricktype;
typedef enum { eLeft, eRight, eTop, eBottom, eTopLeft, eTopRight, eBottomLeft, eBottomRight, eNone } Edge;

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
  int frame;
  Bricktype type;
  Uint32 lastticks;
  Sparkle particles[MAXBRICKPARTICLES];
} Brick;

#endif
