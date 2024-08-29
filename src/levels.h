#ifndef _LEVELS_H_
#define _LEVELS_H_

#include "bounds.h"
#include "brick.h"
#include "cat.h"
#include "app.h"
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
  int catterycount;
  int spawnx;
  int spawny;
  int maxbonuslevel;
  Animation* bg;
  Animation* mg;
  Animation* fg;
  Brick** bricks;
  Cattery** catteries;
  void (*onlevelend)(void*);
} Level;

void levels_compile(const char* folder, const char* ofolder);
void levels_processRow(ResourceFactory* factory, Level* level, Bounds* bounds, const char* rowdata, int row, int* brickno);
void arena_brickFinished(void* sender, void* data);
void arena_brickRepaired(void* sender, void* data);
#endif // _LEVELS_H_
