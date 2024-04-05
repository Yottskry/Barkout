#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdio.h>
#include <stdbool.h>

typedef enum { cmBarkanoid = 0, cmClassic = 1 } ControlMethod;

typedef struct {
  ControlMethod controlmethod;
  int brickparticles;
  int trailparticles;
  bool fullscreen;
} Config;

Config* config_load();
int config_getbrickparticles();
int config_gettrailparticles();
ControlMethod config_getcontrolmethod();
void config_setbrickparticles(int brickparticles);
void config_settrailparticles(int brickparticles);
void config_setcontrolmethod(ControlMethod method);
void config_setfullscreen(bool fullscreen);
void config_save(Config* config);

#endif // _CONFIG_H_
