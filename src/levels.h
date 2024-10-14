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


#ifndef _LEVELS_H_
#define _LEVELS_H_

#include "bounds.h"
#include "brick.h"
#include "cat.h"
#include "app.h"
#include "vector.h"
#include "resourcefactory.h"
#ifndef _WIN32
#include <dirent.h>
#endif
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BRICKW 40
#define BRICKH 25

typedef struct
{
  int level;
  int brickcount;
  int maxbonuslevel;
  Animation* bg;
  Animation* mg;
  Animation* fg;
  Brick** bricks;
  Vector* cats;
  void (*onlevelend)(void*);
} Level;

void levels_compile(const char* folder, const char* ofolder);
void levels_processRow(ResourceFactory* factory, Level* level, Bounds* bounds, const char* rowdata, int row, int* brickno);
void arena_brickFinished(void* sender, void* data);
void arena_brickRepaired(void* sender, void* data);
#endif // _LEVELS_H_
