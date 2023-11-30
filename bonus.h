#ifndef _BONUS_H_
#define _BONUS_H_

#define BONUSFREQUENCY 5

typedef enum { boDeadly, boFast, boSlow, boMultiply, boSkip } Bonustype;

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
