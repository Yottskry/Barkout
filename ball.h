#ifndef _BALL_H_
#define _BALL_H_

#include "animation.h"
#include "arena.h"
#include "bat.h"

#define PI 3.14159

typedef enum {eLeft, eRight, eTop, eBottom, eNone} Edge;

typedef struct
{
  int x;
  int y;
  int cx;
  int cy;
  int radius;
  int speed;
  double bearing;
  int score;
  int level;
  Animation* anim;
} Ball;


int ball_moveball(Ball* ball, Arena* arena, Bat* player);
Brick* ball_collidesbricks(Arena* arena, Ball* ball, Edge* e);

#endif
