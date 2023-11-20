#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <assert.h>
#include "animationfactory.h"
#include "animation.h"
//#include "vector.h"

Animation* af_loadanimation(AnimationFactory* factory, SDL_Renderer* renderer, char* filename, char name[50], int w, int h)
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
  int c = (factory->count);
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

  factory->count++;

	return factory->anims[factory->count - 1];
}

Animation* af_getanimation(AnimationFactory* factory, char name[50])
{
  for(int i = 0; i < factory->count; i++)
  {
    // Found a match
    if(strcmp(name, factory->anims[i]->name) == 0)
    {
      return factory->anims[i];
    }
  }

  return NULL;
}

int af_freeanimation(AnimationFactory* factory, char name[50])
{
  for(int i = 0; i < factory->count; i++)
  {
    if(strcmp(name, factory->anims[i]->name) == 0)
    {
      a_freeanimation(factory->anims[i]);
      free(factory->anims[i]);
      int j = i;

      // Work forwards from the end, moving all
      // animations down one place << and then reallocate the memory
      while(j < factory->count - 1)
      {
        factory->anims[j] = factory->anims[j+1];
        j++;
      }

      factory->count--;

      factory->anims = realloc(factory->anims, sizeof(Animation*) * factory->count);
    }
  }

  return 0;
}

int af_freeanimations(AnimationFactory* factory)
{
//  for(int i = 0; i < factory->count; i++)
//    af_freeanimation();
    //SDL_DestroyTexture(factory->anims[i]);
//  free(factory->anims);
  return 0;
}

