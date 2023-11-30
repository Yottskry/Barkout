#include "bat.h"

int bat_drawbat(Bat* player, SDL_Renderer* renderer)
{
  a_drawsprite(&(player->sprite), renderer, player->x, player->y);
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
  if(player->x + player->w > bounds.right)
  {
    player->x = bounds.right - player->w;
    player->speed = 0;
    player->targetspeed = 0;
  }
  return 0;
}

int bat_setplayerlong(Bat* player)
{
  player->w = psLong;
  return 0;
}
