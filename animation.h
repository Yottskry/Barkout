#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <SDL.h>

typedef enum { asStatic, asMoving } AnimState;

typedef struct
{
	char name[50];
	int size; // number of frames
	int framewidth;
	int frameheight;
	SDL_Texture* sheet;
} Animation;

typedef struct
{
  int currentframe;
  int loop;
  Uint32 lastticks;
  AnimState state;
  Animation* anim;
} Sprite;

int a_freeanimation(Animation* anim);

int a_drawanimation(Animation* anim, SDL_Renderer* renderer, int x, int y, int* frame);

int a_drawstaticframe(Animation* anim, SDL_Renderer* renderer, int x, int y);

int a_drawsprite(Sprite* sprite, SDL_Renderer* renderer, int x, int y);

#endif
