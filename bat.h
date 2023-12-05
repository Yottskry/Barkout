#ifndef _BAT_H_
#define _BAT_H_

#include "resourcefactory.h"
#include "bounds.h"
#include "bonus.h"
#include <SDL.h>

typedef enum { psNormal = 80, psLong = 100, psShort = 60 } Playersize;
typedef enum { plNormal, plShort, plLong, plSticky, plLaser } Playerstate;

typedef struct
{
  int x;
  int y;
  int w;
  int h;
  int maxspeed;
  int speed;
  int targetspeed;
  unsigned int score;
  unsigned int lives;
  Playerstate state;
  Sprite sprite;
} Bat;

int bat_drawbat(Bat* player, SDL_Renderer* renderer);
int bat_movebat(Bat* player, const Bounds bounds);
void bat_reset(Bat* player, ResourceFactory* factory);

#endif // _BAT_H_
