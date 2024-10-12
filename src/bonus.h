#ifndef _BONUS_H_
#define _BONUS_H_

#include "resourcefactory.h"
#include "bounds.h"

#define BONUSFREQUENCY 6

typedef enum { boNone = 0, boShrink = 1, boDeadly = 2, boGrow = 3, boCatch = 4, boPlayer = 5, boLaser = 6, boWarp = 7 } Bonustype;

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
