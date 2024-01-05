#include "bounds.h"

bool bounds_intersects(Bounds* b1, Bounds* b2)
{
  // Test that they can't possibly intersect
  if((b1->right < b2->left) ||
     (b1->left > b2->right) ||
     (b1->bottom < b2->top) ||
     (b1->top > b2->bottom))
  {
    return false;
  }
  return true;
}
