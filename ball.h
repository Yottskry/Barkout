#ifndef _BALL_H_
#define _BALL_H_

#include "animation.h"
#include "arena.h"
#include "bat.h"

#define PI 3.14159

// Sticky for start of play, stuck for special power, loose for released from paddle in special power.
typedef enum { bsNormal, bsSticky, bsStuck, bsLoose } Ballstate;

typedef struct
{
  int x;
  int y;
  int cx;
  int cy;
  int radius;
  int speed;
  // Direction of travel, degrees
  double bearing;
  //int score;
  //int level;
  Animation* anim;
  Ballstate state;
} Ball;

int ball_moveball(Ball* ball, Arena* arena, Bat* player);

Brick* ball_collidesbricks(Arena* arena, Ball* ball, Edge* e);

int ball_collidesbat(Ball* ball, Bat* player, Edge* e);

#endif
