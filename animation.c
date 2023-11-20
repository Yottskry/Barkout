#include "animation.h"

int a_freeanimation(Animation* anim)
{
  if(anim->sheet != NULL)
  {
    SDL_DestroyTexture(anim->sheet);
  }
  return 0;
}

int a_drawanimation(Animation* anim, SDL_Renderer* renderer, int x, int y, int* frame)
{
  SDL_Rect src = {anim->framewidth * *frame, 0, anim->framewidth, anim->frameheight};
  SDL_Rect dst = {x, y, anim->framewidth, anim->frameheight};
  SDL_RenderCopy(renderer, anim->sheet, &src, &dst);
  (*frame)++;
  if(*frame >= anim->size)
    *frame = 0;
  return 0;
}

int a_drawstaticframe(Animation* anim, SDL_Renderer* renderer, int x, int y)
{
  SDL_Rect src = {0, 0, anim->framewidth, anim->frameheight};
  SDL_Rect dst = {x, y, anim->framewidth, anim->frameheight};
  SDL_RenderCopy(renderer, anim->sheet, &src, &dst);
  return 0;
}

int a_drawsprite(Sprite* sprite, SDL_Renderer* renderer, int x, int y)
{
  Animation* anim = sprite->anim;
  SDL_Rect src = {anim->framewidth * sprite->currentframe, 0, anim->framewidth, anim->frameheight};
  SDL_Rect dst = {x, y, anim->framewidth, anim->frameheight};
  SDL_RenderCopy(renderer, anim->sheet, &src, &dst);

  if(sprite->state == asMoving)
  {
    Uint32 ticks = SDL_GetTicks();
    if(ticks - sprite->lastticks >= 70)
    {
      (sprite->currentframe)++;
      sprite->lastticks = ticks;
    }

    if(sprite->currentframe == anim->size)
    {
      sprite->currentframe = 0;
      if(sprite->loop != 1)
        sprite->state = asStatic;
    }
  }
  return 0;
}
