#include "bonus.h"

// The *** is becuase we need to be able to realloc the property in Arena so we need its address, not just what it points to
int bonus_freebonus(Bonus*** bonuses, int* bonuscount, Bonus* bonus)
{
  for(int i = 0; i < *bonuscount; i++)
  {
    // Find the item to be removed
    if((*bonuses)[i] == bonus)
    {
      // sprite-anim is managed by the resourcefactory,
      // so we don't free it here.
      free(bonus->sprite);
      free(bonus);
      (*bonuses)[i] = NULL;

      // Move all subsequent items up one
      for(int j = i; j < (*bonuscount) - 1; j++)
        (*bonuses)[j] = (*bonuses)[j+1];
    }

    (*bonuscount)--;
    (*bonuses) = realloc(*bonuses, sizeof(Bonus*) * (*bonuscount));
  }
  return 0;
}

int bonus_freebonuses(Bonus*** bonuses, int* bonuscount)
{
  for(int i = 0; i < *bonuscount; i++)
  {
    free((*bonuses)[i]->sprite);
    free((*bonuses)[i]);
    (*bonuses)[i] = NULL;
  }
  free(*bonuses);
  (*bonuscount) = 0;
  *bonuses = NULL;
  return 0;
}

// The *** is becuase we need to pass the address of Arena's bonuses property
int bonus_movebonuses(Bonus*** bonuses, int* bonuscount, Bounds bounds)
{
  for(int i = 0; i < (*bonuscount); i++)
  {
    Bonus* bonus = (*bonuses)[i];
    bonus->y += 1;
    if(bonus->y > bounds.bottom)
    {
      bonus_freebonus(bonuses, bonuscount, bonus);
    }
  }
  return 0;
}

int bonus_drawbonuses(Bonus** bonuses, int bonuscount, SDL_Renderer* renderer)
{
  for(int i = 0; i < bonuscount; i++)
  {
    a_drawsprite(bonuses[i]->sprite, renderer, bonuses[i]->x, bonuses[i]->y);
  }
  return 0;
}
