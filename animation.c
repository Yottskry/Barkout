#include "animation.h"

int a_freeanimation(Animation* anim)
{
  if(anim->sheet != NULL)
  {
    SDL_DestroyTexture(anim->sheet);
  }
  return 0;
}


int a_drawanimation(Animation* anim, SDL_Renderer* renderer, int x, int y)
{
  SDL_Rect src = {0, 0, anim->framewidth, anim->frameheight};
  SDL_Rect dst = {x, y, anim->framewidth, anim->frameheight};
  SDL_RenderCopy(renderer, anim->sheet, &src, &dst);
  return 0;
}
