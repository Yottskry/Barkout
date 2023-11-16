#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <SDL.h>

typedef struct
{
	char name[50];
	int size; // number of frames
	int frame; // current frame
	int framewidth;
	int frameheight;
	SDL_Texture* sheet;
} Animation;

int a_freeanimation(Animation* anim);

int a_drawanimation(Animation* anim, SDL_Renderer* renderer, int x, int y);

#endif
