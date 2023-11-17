#include "bat.h"

int bat_drawbat(Bat* player, SDL_Renderer* renderer)
{
  a_drawanimation(player->anim, renderer, player->x, player->y);
  return 0;
}

int bat_movebat(Bat* player, Arena* arena)
{
  player->speed = player->speed > player->targetspeed ? player->speed - 1 : player->speed;
  player->speed = player->speed < player->targetspeed ? player->speed + 1 : player->speed;

  player->x = player->x + player->speed;

  if(player->x < arena->left)
  {
    player->x = arena->left;
    player->speed = 0;
    player->targetspeed = 0;
  }
  if(player->x + player->w > arena->right)
  {
    player->x = arena->right - player->w;
    player->speed = 0;
    player->targetspeed = 0;
  }
  return 0;
}
