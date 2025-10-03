#ifndef _SAVE_H_
#define _SAVE_H_

#include "app.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define LETTERS 37
#define CODELENGTH 9
#define chars "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

typedef struct { 
	int row;
	int col;
	char chr[3];
} Letter;

typedef struct {
	int collimit;
	int selected;
	Letter letters[LETTERS];
	char code[CODELENGTH];
} Save;

long FromBase36(char* str);
char* ToBase36(int imp, int minLength);
long GetCheckTotal(char* encLevel);
int save_decodeLevel(char* encLevel, long* lev, long* liv, long* scr, long* pow);
char* EncodeLevel(char* encScore, int level, int lives, int powerup);
char* AddCheckDigit(char* encLevel);
int save_createLetters(Save* save);
int save_drawLetters(App* app, Save* save);
int save_moveLetter(Save* save, bool forward, bool multiple);
char* save_selectLetter(Save* save);

#endif
