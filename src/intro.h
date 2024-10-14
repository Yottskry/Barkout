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


#ifndef _INTRO_H_
#define _INTRO_H_

#include "resourcefactory.h"

#define STARS 50

typedef struct
{
  int x;
  int y;
  int speed;
  SDL_Color color;
} Star;

void intro_init(Star stars[STARS]);
void intro_drawstars(SDL_Renderer* renderer, Star stars[STARS]);
void intro_movestars(Star stars[STARS]);


#endif
