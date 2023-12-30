#ifndef _BAT_H_
#define _BAT_H_

#include "resourcefactory.h"
#include "bounds.h"
#include "bonus.h"
#include <stdbool.h>
#include <SDL.h>

typedef enum { psNormal = 79, psLong = 120, psShort = 60 } Playersize;
typedef enum { plNormal, plShort, plLong, plSticky, plLaser } Playerstate;
typedef enum { cmBarkanoid = 1, cmClassic = 2 } ControlMethod;

typedef struct
{
  int x;
  int y;
  Playersize w;
  int h;
  int maxspeed;
  int speed;
  int targetspeed;
  unsigned int lives;
  ControlMethod controlmethod;
  Playerstate state;
  bool warpenabled;
  Sprite sprite;
} Bat;

int bat_drawbat(Bat* player, SDL_Renderer* renderer, const Bounds bounds);
int bat_movebat(Bat* player, const Bounds bounds);
void bat_reset(Bat* player, ResourceFactory* factory);

#endif // _BAT_H_
