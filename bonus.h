#ifndef _BONUS_H_
#define _BONUS_H_

#include "resourcefactory.h"
#include "bounds.h"

#define BONUSFREQUENCY 4

typedef enum { boShrink = 0, boDeadly = 1, boGrow = 2, boPlayer = 3, boCatch = 4, boLaser = 5, boSlow = 6, boWarp = 7 } Bonustype;

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
