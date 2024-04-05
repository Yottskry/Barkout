#include "config.h"
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <winreg.h>
#else
#include <sys/stat.h>
#endif

static Config config;

Config* config_load()
{
  config.brickparticles = 15;
  config.trailparticles = 20;
  config.controlmethod = cmClassic;

  #ifdef _WIN32
  HKEY newKey;
  long retval;
  retval = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\FatHorseGames", 0, KEY_READ, &newKey);
  if(retval == ERROR_SUCCESS)
  {
    long unsigned int lpSize = sizeof(int);
    retval = RegQueryValueExA(newKey, "brickparticles", 0, NULL, (BYTE*)(&config.brickparticles), &lpSize);
    retval = RegQueryValueExA(newKey, "trailparticles", 0, NULL, (BYTE*)(&config.brickparticles), &lpSize);
  }
  else
  {
    RegCloseKey(newKey);
  }
  #else // assume linux / BSD
  char* homedir = getenv("HOME");
  if(homedir == NULL)
    return 0;
  char barkdata[255] = "";
  strcat(barkdata, homedir);
  strcat(barkdata, "/.barkanoid/options");
  FILE* datafile = fopen(barkdata, "r");
  if(datafile != NULL)
  {
    char key[100] = "";
    int val = 0;
    while(fscanf(datafile, "%s%d", key, &val) == 2)
    {
      if(strcmp("BRICKPARTICLES", key) == 0)
        config.brickparticles = val;
      if(strcmp("TRAILPARTICLES", key) == 0)
        config.trailparticles = val;
    }
    //fscanf(datafile, "BRICKPARTICLES:%d", &config.brickparticles);
    fclose(datafile);
  }
  #endif
  return &config;
}

int config_getbrickparticles()
{
  return config.brickparticles;
}

int config_gettrailparticles()
{
  return config.trailparticles;
}

ControlMethod config_getcontrolmethod()
{
  return config.controlmethod;
}

void config_setbrickparticles(int brickparticles)
{
}

void config_settrailparticles(int brickparticles)
{
}

void config_setcontrolmethod(ControlMethod method)
{
}

void config_setfullscreen(bool fullscreen)
{
  config.fullscreen = fullscreen;
}

void config_save(Config* config)
{
}
