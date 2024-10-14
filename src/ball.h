/*
*
* Barkout - Copyright 2024 Stephen Branley.
*
* This file is part of Barkout.
*
* Barkout is free software: you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation, either version 3 of the License, 
* or (at your option) any later version.
*
* Barkout is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*
*/


#ifndef _BALL_H_
#define _BALL_H_

#include "brick.h"
#include "bounds.h"
#include "bat.h"
#include <stdbool.h>
#include <math.h>

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
Brick* ball_collidesBricks(Ball* ball, Brick** bricks, Bat* player, int brickcount, long* bonuscounter, Edge* e);
void ball_ricochet(Ball* ball, Edge hitedge);
void ball_drawBall(Ball* ball, SDL_Renderer* renderer);
int ball_collidesBat(Ball* ball, Bat* player, Edge* e);

#endif
