#include "scores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <winreg.h>
#else
#include <sys/stat.h>
#endif

int loadHighScore()
{
  int score = 0;
  #ifdef _WIN32
  HKEY newKey;
  long retval;
  retval = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\FatHorseGames", 0, KEY_READ, &newKey);
  if(retval == ERROR_SUCCESS)
  {
    long unsigned int lpSize = sizeof(int);
    retval = RegQueryValueExA(newKey, "highscore", 0, NULL, (BYTE*)(&score), &lpSize);
    if(retval != ERROR_SUCCESS)
    {
      score = 0;
    }
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
  strcat(barkdata, "/.barkanoid/data");
  FILE* scorefile = fopen(barkdata, "r");
  if(scorefile != NULL)
  {
    fscanf(scorefile, "%d", &score);
    fclose(scorefile);
  }
  #endif
  return score;
}

void saveHighScore(int* score)
{
  #ifdef _WIN32
  HKEY newKey;
  long retval;
  retval = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\FatHorseGames", 0, "", 0, KEY_WRITE, 0, &newKey, NULL);
  if(retval == ERROR_SUCCESS)
  {
    RegSetValueExA(newKey, "highscore", 0, REG_DWORD, (BYTE*)score, sizeof(int));
    RegCloseKey(newKey);
  }
  #else
  char* homedir = getenv("HOME");
  if(homedir == NULL)
    return;
  char barkdata[255] = "";
  strcat(barkdata, homedir);
  strcat(barkdata, "/.barkanoid");
  mkdir(barkdata, 0777);
  strcat(barkdata, "/data");
  remove(barkdata);
  FILE* scorefile = fopen(barkdata, "w");
  fprintf(scorefile, "%d", *score);
  fclose(scorefile);
  #endif
}
