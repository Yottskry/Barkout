#ifndef _BRICK_H_
#define _BRICK_H_

#include "animation.h"

#define BRICKW 40
#define BRICKH 25

typedef enum { bsStatic, bsHit } Brickstate;

typedef struct
{
  int left;
  int top;
  int right;
  int bottom;
  int hitcount;
  int frame;
  Uint32 lastticks;
  Sprite* sprite;
} Brick;

#endif
