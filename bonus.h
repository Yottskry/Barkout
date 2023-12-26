#ifndef _BONUS_H_
#define _BONUS_H_

#define BONUSFREQUENCY 5

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

#endif
