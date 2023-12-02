#ifndef _BONUS_H_
#define _BONUS_H_

#define BONUSFREQUENCY 5

typedef enum { boShrink = 0, boFast = 1, boSlow = 2, boMultiply = 3, boSkip = 4, boDeadly = 5, boGrow = 6 } Bonustype;

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
