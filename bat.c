#include "bat.h"

int bat_drawbat(Bat* player, SDL_Renderer* renderer, const Bounds bounds)
{
  SDL_Rect r = { .x = 0, .y = 0, .h = player->sprite.anim->frameheight, .w = 0 };
  int w = bounds.right - player->x;
  int pwidth = player->sprite.anim->framewidth;
  if(w < pwidth)
    r.w = w;
  else
    r.w = pwidth;

  a_drawclippedsprite(&(player->sprite), renderer, player->x, player->y, r);
  return 0;
}

int bat_movebat(Bat* player, const Bounds bounds)
{
  player->speed = player->speed > player->targetspeed ? player->speed - 1 : player->speed;
  player->speed = player->speed < player->targetspeed ? player->speed + 1 : player->speed;

  player->x = player->x + player->speed;

  if(player->x < bounds.left)
  {
    player->x = bounds.left;
    player->speed = 0;
    player->targetspeed = 0;
  }

  if((player->x + player->w > bounds.right) && (!player->warpenabled))
  {
    player->x = bounds.right - player->w;
    player->speed = 0;
    player->targetspeed = 0;
  }
  else if(player->x + player->w > bounds.right)
  {
    player->targetspeed = 2;
    // Return 1 to show we're currently warping...

    return 1;
  }
  return 0;
}

void bat_reset(Bat* player, ResourceFactory* factory)
{
  player->warpenabled = false;
  player->state = plNormal;
  player->w = psNormal;
  af_setanimation(factory, &(player->sprite),"bat", 1, NULL, NULL, NULL);
}
