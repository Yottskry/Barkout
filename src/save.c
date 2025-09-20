#include "save.h"

#define chars "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

long FromBase36(char* str)
{
	int len = strlen(str);
	int tot = 0;
	for(int i = 0; i < len; i++)
	{
		char c = str[i];
		// No error checking. Assume a properly constructed b36 number will only have valid characters
		char* found = strchr(chars, c);
		int pos = found - chars;
		//int val = chars[pos] - 48;

		long sub = pos * (long)powl(36, (len - i) - 1);
		tot = tot + sub;
	}
	return tot;
}

char* ToBase36(int imp, int minLength)
{
	int rem = imp;
	int cnt = 0;
	int len = 8;

	// Find the required length of the output
	while((rem / powl(36, len)) < 1)
	{
		//printf("%Lf\n", powl(36, len));
		len--;
	}

	len++;

	if(len < minLength)
		len = minLength;

	len++; // allow for the NULL terminator

	char* out = calloc(len, sizeof(char));
	memset(out, 48, len-1);
	for(int i = len-2; i > 0; i--)
	{
		cnt = rem / powl(36, i);
		rem = rem % (long)powl(36, i);
		out[(len-2)-i] = chars[cnt];
	}
	out[len-2] = chars[rem];

	return out;
}


long GetCheckTotal(char* encLevel)
{
	long tot = 0;
	for(int i = 0; i < strlen(encLevel); i++)
	{
		char* c = calloc(2, sizeof(char));
		c[0] = encLevel[i];
		long l = FromBase36(c);
		free(c);
		tot = tot + l;
	}
	return tot;
}

int DecodeLevel(char* encLevel, long* lev, long* liv, long* scr, long* pow)
{
	// Get the check digit and remove it from the string
	char dig = encLevel[strlen(encLevel)-1];
	long chk = FromBase36(&(encLevel[strlen(encLevel)-1]));

	char* code = calloc(strlen(encLevel), sizeof(char)); // strlen omits the \0 so we don't need to add 1
	strncpy(code, encLevel, strlen(encLevel)-1);
	long tot = GetCheckTotal(code);
	free(code);

	if(chk != (tot % 11))
	{
		return -1;
	}

	char* lives = calloc(2, sizeof(char));
	char* level = calloc(2, sizeof(char));	
	char* power = calloc(2, sizeof(char));
	char* score = calloc(strlen(encLevel) - 3, sizeof(char));

	strncpy(level, encLevel, 1);
	strncpy(lives, &(encLevel[1]), 1);
	strncpy(power, &(encLevel[2]), 1);
	strncpy(score, &(encLevel[3]), strlen(encLevel) - 4);

	*lev = FromBase36(level);
	*liv = FromBase36(lives);
	*scr = FromBase36(score);
	*pow = FromBase36(power);

	free(score);
	free(lives);
	free(level);	
	free(power);

	return 0;
}

char* EncodeLevel(char* encScore, int level, int lives, int powerup)
{
	// Codes are only offered up every 4 levels, so "level" is just how many multiples of 4 apply
	level = (int)(level / 4);
	char* lev = ToBase36(level, 1);
	char* liv = ToBase36(lives, 1);
	char* pow = ToBase36(powerup, 1);
	int sz = strlen(encScore) + strlen(lev) + strlen(liv) + strlen(pow);
	char* out = calloc(sz+1, sizeof(char));
	strncpy(out, lev, 1);
	strncpy(&(out[1]), liv, 1);
	strncpy(&(out[2]), pow, 1);
	strncpy(&(out[3]), encScore, strlen(encScore));
	free(lev);
	free(liv);
	free(pow);
	return out;
}

char* AddCheckDigit(char* encLevel)
{
	long tot = GetCheckTotal(encLevel);

	char* out = calloc(strlen(encLevel) + 2, sizeof(char));
	strcpy(out, encLevel);

	char* dig = ToBase36(tot % 11, 1);
	out[strlen(out)] = dig[0];
	free(dig);
	return out;
}
