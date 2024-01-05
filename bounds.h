#ifndef _BOUNDS_H_
#define _BOUNDS_H_

#include <stdbool.h>

typedef struct
{
  int left;
  int top;
  int right;
  int bottom;
} Bounds;

bool bounds_intersects(Bounds* b1, Bounds* b2);

#endif // _BOUNDS_H_
