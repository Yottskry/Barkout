#ifndef _BALL_H_
#define _BALL_H_

#include "brick.h"
#include "bounds.h"
#include <stdbool.h>

#define PI 3.14159

typedef enum
{
  bsNormal, // Normal play
  bsSticky, // Start of game sticky
  bsStuck,  // Special power sticky
  bsLoose,  // Loose when special power
  bsDeadly  // Destroys bricks and does not bounce off them
} Ballstate;

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
  Sprite sprite;
  Ballstate state;
} Ball;

bool ball_collidesbounds(Ball* ball, Bounds* bounds, Edge* e);
Brick* ball_collidesbricks(Ball* ball, Brick** bricks, int brickcount, Edge* e);

#endif
