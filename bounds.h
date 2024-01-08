#ifndef _BOUNDS_H_
#define _BOUNDS_H_

#include <stdbool.h>

typedef struct
{
  int left;
  int top;

  union {
    int right;
    int width;
  };

  union {
    int bottom;
    int height;
  };

} Bounds;

bool bounds_intersects(Bounds* b1, Bounds* b2);

#endif // _BOUNDS_H_
