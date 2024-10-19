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


#ifndef _BONUS_H_
#define _BONUS_H_

#include "resourcefactory.h"
#include "bounds.h"

#define BONUSFREQUENCY 6
#define BONUSDELAY 2000 // 2 seconds must elapse between bonuses

typedef enum { boNone = 0, boGrow = 1, boCatch = 2, boShrink = 3, boLaser = 4, boDeadly = 5, boPlayer = 6, boWarp = 7 } Bonustype;

typedef struct
{
  Sprite* sprite;
  int x;
  int y;
  int w;
  int h;
  Bonustype type;
} Bonus;

int bonus_freebonus(Bonus*** bonuses, int* bonuscount, Bonus* bonus);
int bonus_freebonuses(Bonus*** bonuses, int* bonuscount);
int bonus_movebonuses(Bonus*** bonuses, int* bonuscount, Bounds bounds);
int bonus_drawbonuses(Bonus** bonuses, int bonuscount, SDL_Renderer* renderer);

#endif
