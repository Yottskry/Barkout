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
