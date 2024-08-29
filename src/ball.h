#ifndef _BALL_H_
#define _BALL_H_

#include "brick.h"
#include "bounds.h"
#include <stdbool.h>

#define PI 3.14159
#define MAXTRAILPARTICLES 255

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
  Sparkle sparkles[MAXTRAILPARTICLES];
  Brick* warpdest;
} Ball;

bool ball_collidesBounds(Ball* ball, Bounds* bounds, Edge* e, int* delta);
Brick* ball_collidesBricks(Ball* ball, Brick** bricks, int brickcount, Edge* e);
void ball_ricochet(Ball* ball, Edge hitedge);
void ball_drawBall(Ball* ball, SDL_Renderer* renderer);

#endif
