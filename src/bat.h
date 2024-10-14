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


#ifndef _BAT_H_
#define _BAT_H_

#include "resourcefactory.h"
#include "bounds.h"
#include "bonus.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef enum { psNormal = 79, psLong = 120, psShort = 60 } Playersize;
typedef enum { plNormal, plShort, plLong, plSticky, plLaser } Playerstate;

typedef struct
{
  int x;
  int y;
  Playersize w;
  int h;
  int maxspeed;
  int speed;
  int targetspeed;
  int score;
  int lives;
  Playerstate state;
  bool warpenabled;
  Sprite sprite;
} Bat;

int bat_drawbat(Bat* player, SDL_Renderer* renderer, const Bounds bounds);
int bat_movebat(Bat* player, const Bounds bounds);
void bat_reset(Bat* player, ResourceFactory* factory);

#endif // _BAT_H_
