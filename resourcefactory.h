#ifndef _RESOURCE_FACTORY_H_
#define _RESOURCE_FACTORY_H_

//#include "vector.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "animation.h"

typedef struct
{
  char name[50];
  Mix_Chunk* sample;
} Sample;

// Creates and stores animations
typedef struct
{
	int animationcount;
  Animation** anims;
  int samplecount;
  Sample** samples;
} ResourceFactory;

/* Animation related functions */

Animation* af_loadanimation(ResourceFactory* factory, SDL_Renderer* renderer, char* filename, char name[50], int w, int h);

Animation* af_getanimation(ResourceFactory* factory, char name[50]);

int af_freeanimation(ResourceFactory* factory, char name[50]);

int af_freeanimations(ResourceFactory* factory);

/* Mixer / Sample related functions */

Mix_Chunk* af_loadsample(ResourceFactory* factory, const char* filename, char name[50]);

int af_playsample(ResourceFactory* factory, const char* name);

int af_freesamples(ResourceFactory* factory);

#endif
