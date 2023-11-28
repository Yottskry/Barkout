#ifndef _BAT_H_
#define _BAT_H_

#include "animation.h"
#include "bounds.h"
#include <SDL.h>

typedef struct
{
  int x;
  int y;
  int w;
  int h;
  int maxspeed;
  int speed;
  int targetspeed;
  Animation* anim;
} Bat;

int bat_drawbat(Bat* player, SDL_Renderer* renderer);
int bat_movebat(Bat* player, const Bounds bounds);

#endif // _BAT_H_
