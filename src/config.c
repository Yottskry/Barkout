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

void config_load()
{
  config.fullscreen = false;
  config.brickparticles = 35;
  config.trailparticles = 50;
  config.controlmethod = cmClassic;
  config.installed = false;
  config.debug = false;

  #ifdef _WIN32
  HKEY newKey;
  long retval;
  retval = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\FatHorseGames", 0, KEY_READ, &newKey);
  if(retval == ERROR_SUCCESS)
  {
    long unsigned int lpSize = sizeof(int);
    retval = RegQueryValueExA(newKey, "brickparticles", 0, NULL, (BYTE*)(&config.brickparticles), &lpSize);
    retval = RegQueryValueExA(newKey, "trailparticles", 0, NULL, (BYTE*)(&config.trailparticles), &lpSize);
    retval = RegQueryValueExA(newKey, "fullscreen", 0, NULL, (BYTE*)(&config.fullscreen), &lpSize);
    retval = RegQueryValueExA(newKey, "ctrlmethod", 0, NULL, (BYTE*)(&config.controlmethod), &lpSize);
  }
  else
  {
    RegCloseKey(newKey);
  }
  #else // assume linux / BSD
  char* homedir = getenv("HOME");
  if(homedir == NULL)
    return;
  char barkdata[255] = "";
  strcat(barkdata, homedir);
  strcat(barkdata, "/.barkout/options");
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
      if(strcmp("FULLSCREEN", key) == 0)
        config.fullscreen = (bool)val;
      if(strcmp("CTRLMETHOD", key) == 0)
        config.controlmethod = (ControlMethod)val;
    }

    fclose(datafile);
  }

  // Test for installed resource file

  #ifdef INSTALLDIR
  FILE *testfile = fopen(INSTALLDIR "/Sprites/ball.png", "r");
  if(testfile != NULL)
  {
    config.installed = true;
    fclose(testfile);
  }
  #endif // INSTALLDIR

  #endif
}

int config_getbrickparticles()
{
  return config.brickparticles;
}

int config_gettrailparticles()
{
  return config.trailparticles;
}

ControlMethod* config_getcontrolmethod()
{
  return &(config.controlmethod);
}

bool config_getinstalled()
{
  return config.installed;
}

bool config_getdebug()
{
  return config.debug;
}

bool* config_getfullscreen()
{
  return &(config.fullscreen);
}

bool config_isfullscreen()
{
  return config.fullscreen;
}

void config_setbrickparticles(int brickparticles)
{
}

void config_settrailparticles(int brickparticles)
{
}

void config_setcontrolmethod(ControlMethod method)
{
  config.controlmethod = method;
}

void config_setfullscreen(bool fullscreen)
{
  config.fullscreen = fullscreen;
}

void config_setdebug(bool debug)
{
  config.debug = debug;
}

void config_save(void)
{
  #ifdef _WIN32
  HKEY newKey;
  long retval;
  retval = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\FatHorseGames", 0, "", 0, KEY_WRITE, 0, &newKey, NULL);
  if(retval == ERROR_SUCCESS)
  {
    RegSetValueExA(newKey, "fullscreen", 0, REG_DWORD, (BYTE*)(&config.fullscreen), sizeof(int));
    RegSetValueExA(newKey, "ctrlmethod", 0, REG_DWORD, (BYTE*)(&config.controlmethod), sizeof(int));
    RegCloseKey(newKey);
  }
  #else // Assume linux / BSD
  char* homedir = getenv("HOME");
  if(homedir == NULL)
    return;
  char barkdata[255] = "";
  strcat(barkdata, homedir);
  strcat(barkdata, "/.barkanoid");
  mkdir(barkdata, 0777);
  strcat(barkdata, "/options");
  remove(barkdata);
  FILE* optionsfile = fopen(barkdata, "w");
  fprintf(optionsfile, "FULLSCREEN %d\n", config.fullscreen);
  fprintf(optionsfile, "TRAILPARTICLES %d\n", config.trailparticles);
  fprintf(optionsfile, "BRICKPARTICLES %d\n", config.brickparticles);
  fprintf(optionsfile, "CTRLMETHOD %d\n", (int)config.controlmethod);
  fclose(optionsfile);
  #endif
}
