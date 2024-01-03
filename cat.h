#ifndef _CAT_H_
#define _CAT_H_

#include "resourcefactory.h"

// The bad guys!

typedef enum { dUp, dDown, dLeft, dRight } Direction;

typedef struct
{
  Sprite* sprite;
  Bounds bounds; // Need to get away from separate x, y, w, h, etc
  int speed;
  Direction nextdirection;
} Cat;


Cat* cat_create();
void cat_free(Cat* cat);
#endif // _CAT_H_
