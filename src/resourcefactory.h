/*
*
* Barkout - Copyright 2024 Stephen Branley.
*
* This file is part of Barkout.
*
* Barkout is free software: you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation, either version 3 of the License, 
* or (at your option) any later version.
*
* Barkout is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*
*/


#ifndef _RESOURCE_FACTORY_H_
#define _RESOURCE_FACTORY_H_

#include "config.h"
#include "app.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

typedef enum { asStatic, asPlayToEnd, asPlayAndReset, asLooping } AnimState;

typedef struct
{
	char name[50];
	int size; // number of frames
	int framewidth;
	int frameheight;
	SDL_Texture* sheet;
	Uint8 keycolorr;
	Uint8 keycolorg;
	Uint8 keycolorb;
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

int a_drawstaticframe(Animation* anim, SDL_Renderer* renderer, int x, int y, int frameno, Uint8 alpha);
int a_drawpartialframe(Animation* anim, SDL_Renderer* renderer, SDL_Rect src, int x, int y, int frameno, Uint8 alpha);

void a_drawsprite(Sprite* sprite, SDL_Renderer* renderer, int x, int y);

void a_drawclippedsprite(Sprite* sprite, SDL_Renderer* renderer, int x, int y, SDL_Rect cliprect);

int af_freeanimations(ResourceFactory* factory);

void af_setanimation(ResourceFactory* factory, Sprite* sprite, char name[50], int loop, void (*f)(void*, void*), void* sender, void* data);

/* Mixer / Sample related functions */

Mix_Chunk* af_loadsample(ResourceFactory* factory, const char* filename, char name[50]);

int af_playsample(ResourceFactory* factory, const char* name);

int af_playsampleforced(ResourceFactory* factory, const char* name, const int channel);

int af_freesamples(ResourceFactory* factory);

Mix_Music* af_loadmusic(const char* filename);

#endif
