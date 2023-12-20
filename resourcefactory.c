#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <assert.h>
#include "resourcefactory.h"
//#include "vector.h"

Animation* af_loadanimation(ResourceFactory* factory, SDL_Renderer* renderer, char* filename, char name[50], int w, int h)
{
  // Load an image into a temporary surface.
	SDL_Surface* tmp = IMG_Load(filename);
	// Literally can't remember why I do this. Transparency? Can't remember...
	SDL_SetSurfaceAlphaMod(tmp, 255);
	SDL_Surface* sfc = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGBA32, 0);
	// Free the original temporary surface
	SDL_FreeSurface(tmp);

	int nframes = (int)(sfc->w / w);

	// Throw an error if there are no frames in the animation/sprite
	assert(nframes > 0);

	// Surfaces can only be used with slow software rendering, so convert
	// to a texture for hardware (accelerated) rendering
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, sfc);

	SDL_FreeSurface(sfc);

  // Increase count before adding so that it's correct for the realloc call
  int c = (factory->animationcount);
  c++;
  // Use realloc. Initially factory->anims is null, so this behaves as malloc
  // !IMPORTANT - this is memory allocation for enough
  // **POINTERS** to each Animation struture, not the memory
  // for the structures themselves - that comes later
  factory->anims = realloc(factory->anims, sizeof(Animation*) * c);

  // NOW allocate memory for the structure itself.
  factory->anims[c - 1] = malloc(sizeof(Animation));

  Animation* anim = (Animation*)(factory->anims[c - 1]);
  anim->sheet = tex;
  anim->size = nframes;
  anim->framewidth = w;
  anim->frameheight = h;
	strcpy(anim->name, name);

  factory->animationcount++;

	return factory->anims[factory->animationcount - 1];
}

Animation* af_getanimation(ResourceFactory* factory, char name[50])
{
  for(int i = 0; i < factory->animationcount; i++)
  {
    // Found a match
    if(strcmp(name, factory->anims[i]->name) == 0)
    {
      return factory->anims[i];
    }
  }

  return NULL;
}

int a_freeanimation(Animation* anim)
{
  if(anim->sheet != NULL)
  {
    SDL_DestroyTexture(anim->sheet);
    anim->sheet = NULL;
  }
  return 0;
}

int a_drawstaticframe(Animation* anim, SDL_Renderer* renderer, int x, int y, int frameno)
{
  SDL_Rect src = {anim->framewidth * frameno, 0, anim->framewidth, anim->frameheight};
  SDL_Rect dst = {x, y, anim->framewidth, anim->frameheight};
  SDL_RenderCopy(renderer, anim->sheet, &src, &dst);
  return 0;
}

void a_drawsprite(Sprite* sprite, SDL_Renderer* renderer, int x, int y)
{
  Animation* anim = sprite->anim;
  SDL_Rect src = {anim->framewidth * sprite->currentframe, 0, anim->framewidth, anim->frameheight};
  SDL_Rect dst = {x, y, anim->framewidth, anim->frameheight};
  SDL_RenderCopy(renderer, anim->sheet, &src, &dst);

  if(sprite->state != asStatic)
  {
    Uint32 ticks = SDL_GetTicks();
    if(ticks - sprite->lastticks >= 50)
    {
      (sprite->currentframe)++;
      sprite->lastticks = ticks;
    }

    if(sprite->currentframe == anim->size)
    {
      if(sprite->state != asPlayToEnd)
        sprite->currentframe = 0;
      else
        sprite->currentframe = anim->size - 1;

      if(sprite->state != asLooping)
      {
        sprite->state = asStatic;
        if(sprite->onanimfinished != NULL)
          sprite->onanimfinished(sprite->sender, sprite->data);
      }

    }
  }
}

int af_freeanimations(ResourceFactory* factory)
{
  for(int i = 0; i < factory->animationcount; i++)
  {
    a_freeanimation(factory->anims[i]);
    free(factory->anims[i]);
  }
  free(factory->anims);
  factory->anims = NULL;
  return 0;
}

void af_setanimation(ResourceFactory* factory, Sprite* sprite, char name[50], int loop, void (*f)(void*, void*), void* sender, void* data)
{
  Animation* anim = af_getanimation(factory, name);

  assert(anim != NULL);

  sprite->anim = anim;
  sprite->currentframe = 0;
  sprite->lastticks = 0;
  sprite->loop = loop;
  sprite->state = loop == 0 ? asPlayAndReset : asLooping;
  sprite->onanimfinished = f;
  sprite->sender = sender;
  sprite->data = data;
}

Mix_Chunk* af_loadsample(ResourceFactory* factory, const char* filename, char name[50])
{
  factory->samples = realloc(factory->samples, sizeof(Sample*) * (factory->samplecount + 1));
  factory->samples[factory->samplecount] = malloc(sizeof(Sample));
  factory->samples[factory->samplecount]->sample = Mix_LoadWAV(filename);
  strcpy(factory->samples[factory->samplecount]->name, name);
  factory->samplecount++;
  return factory->samples[factory->samplecount-1]->sample;
}

int af_playsample(ResourceFactory* factory, const char* name)
{
  for(int i = 0; i < factory->samplecount; i++)
  {
    if(strcmp(factory->samples[i]->name,name) == 0)
    {
      Mix_PlayChannel(-1, factory->samples[i]->sample, 0);
    }
  }
  return 0;
}

int af_freesamples(ResourceFactory* factory)
{
  for(int i = 0; i < factory->samplecount; i++)
  {
    Mix_FreeChunk(factory->samples[i]->sample);
    free(factory->samples[i]);
  }
  free(factory->samples);
  factory->samples = NULL;
  return 0;
}
