#include "bounds.h"

bool bounds_intersects(Bounds* b1, Bounds* b2)
{
  // Test that they can't possibly intersect

  if(((b1->left + b1->width) < b2->left) ||
     (b1->left > (b2->left + b2->width)) ||
     ((b1->top + b1->height) < b2->top) ||
     (b1->top > (b2->top + b2->height)))
  {
    return false;
  }
  return true;
}
