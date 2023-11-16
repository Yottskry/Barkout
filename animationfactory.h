#ifndef _ANIMATION_FACTORY_H_
#define _ANIMATION_FACTORY_H_

//#include "vector.h"
#include <SDL.h>
#include <SDL_image.h>
#include "animation.h"

// Creates and stores animations
typedef struct
{
	int count;
  Animation** anims;
} AnimationFactory;

Animation* af_loadanimation(AnimationFactory* factory, SDL_Renderer* renderer, char* filename, char name[50], int w, int h);
Animation* af_getanimation(AnimationFactory* factory, char name[50]);
int af_freeanimation(AnimationFactory* factory, char name[50]);
int af_freeanimations(AnimationFactory* factory);

#endif
