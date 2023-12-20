#ifndef _RESOURCE_FACTORY_H_
#define _RESOURCE_FACTORY_H_

//#include "vector.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

typedef enum { asStatic, asPlayToEnd, asPlayAndReset, asLooping } AnimState;

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
  void (*onanimfinished)(void*, void*);
  void* sender;
  void* data;
} Sprite;

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

int a_freeanimation(Animation* anim);

int a_drawstaticframe(Animation* anim, SDL_Renderer* renderer, int x, int y, int frameno);

void a_drawsprite(Sprite* sprite, SDL_Renderer* renderer, int x, int y);

int af_freeanimations(ResourceFactory* factory);

void af_setanimation(ResourceFactory* factory, Sprite* sprite, char name[50], int loop, void (*f)(void*, void*), void* sender, void* data);

/* Mixer / Sample related functions */

Mix_Chunk* af_loadsample(ResourceFactory* factory, const char* filename, char name[50]);

int af_playsample(ResourceFactory* factory, const char* name);

int af_freesamples(ResourceFactory* factory);

#endif
