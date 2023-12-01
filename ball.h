#ifndef _BALL_H_
#define _BALL_H_

#include "arena.h"
#include "bat.h"

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

int ball_moveball(Ball* ball, Arena* arena, Bat* player);

Brick* ball_collidesbricks(Arena* arena, Ball* ball, Edge* e);

int ball_collidesbat(Ball* ball, Bat* player, Edge* e);

#endif
