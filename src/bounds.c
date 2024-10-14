/*
*
* Barkout - Copyright 2024 Stephen Branley.
*
* This file is part of Barkout.
*
* Barkout is free software: you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation, either version 3 of the License, 
* or (at your option) any later version.
*
* Barkout is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*
*/


#include "bounds.h"

bool bounds_intersects(Bounds* b1, Bounds* b2)
{
  // Test that they can't possibly intersect

  if(((b1->left + b1->width) <= b2->left) ||
     (b1->left >= (b2->left + b2->width)) ||
     ((b1->top + b1->height) <= b2->top) ||
     (b1->top >= (b2->top + b2->height)))
  {
    return false;
  }
  return true;
}
