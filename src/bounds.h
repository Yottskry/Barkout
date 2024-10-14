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
