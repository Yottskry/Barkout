#ifndef _SAVE_H_
#define _SAVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

long FromBase36(char* str);
char* ToBase36(int imp, int minLength);
long GetCheckTotal(char* encLevel);
int DecodeLevel(char* encLevel, long* lev, long* liv, long* scr, long* pow);
char* EncodeLevel(char* encScore, int level, int lives, int powerup);
char* AddCheckDigit(char* encLevel);

#endif
