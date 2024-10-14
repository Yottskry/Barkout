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


#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdio.h>
#include <stdbool.h>

typedef enum { cmClassic = 0, cmBarkanoid = 1 } ControlMethod;

typedef struct {
  ControlMethod controlmethod;
  int brickparticles;
  int trailparticles;
  int startlives;
  int ballspeed;
  bool fullscreen;
  bool installed;
  bool debug;
} Config;

// No need to return anything as we're having a single stack copy of this that is privately accessed within this unit
void config_load();
int config_getbrickparticles();
int config_gettrailparticles();
int config_getstartlives();
int config_getballspeed();
ControlMethod* config_getcontrolmethod();
bool config_getinstalled();
bool config_getdebug();
bool* config_getfullscreen();
bool config_isfullscreen();
void config_setbrickparticles(int brickparticles);
void config_settrailparticles(int brickparticles);
void config_setcontrolmethod(ControlMethod method);
void config_setfullscreen(bool fullscreen);
void config_setdebug(bool debug);
void config_save(void);

#endif // _CONFIG_H_
