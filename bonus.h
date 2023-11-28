#ifndef _BONUS_H_
#define _BONUS_H_

typedef enum { boDeadly, boFast, boSlow, boMultiply, boSkip } Bonustype;

typedef struct
{
  Sprite* sprite;
  int x;
  int y;
  Bonustype type;
} Bonus;

#endif
