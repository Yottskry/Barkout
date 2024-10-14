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


#include "intro.h"
#include <math.h>

void intro_init(Star stars[STARS])
{
  for(int i = 0; i < STARS; i++){
    int wcomp = (rand() % 25) + 220;
    stars[i].x = rand() % 800;
    stars[i].y = rand() % 600;
    stars[i].speed = (rand() % 5) + 1;
    stars[i].color = (SDL_Color){wcomp,wcomp,wcomp,wcomp};
  }
}

void intro_drawstars(SDL_Renderer* renderer, Star stars[STARS])
{
  for(int i = 0; i < STARS; i++){
    SDL_SetRenderDrawColor(renderer, stars[i].color.r, stars[i].color.g, stars[i].color.b, 255);
    SDL_RenderDrawPoint(renderer, stars[i].x, stars[i].y);
  }
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void intro_movestars(Star stars[STARS])
{
  for(int i = 0; i < STARS; i++){
    stars[i].x -= stars[i].speed;
    if(stars[i].x < 0)
      stars[i].x = 800;
  }
}

