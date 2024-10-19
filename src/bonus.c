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


#include "bonus.h"

// The *** is becuase we need to be able to realloc the property in Arena so we need its address, not just what it points to
int bonus_freebonus(Bonus*** bonuses, int* bonuscount, Bonus* bonus)
{
  for(int i = (*bonuscount)-1; i >= 0; i--)
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


      (*bonuscount)--;

      if(*bonuscount == 0)
      {
        free(*bonuses);
        *bonuses = NULL;
      }
      else
      {
        // We'll assume that shrinking the buffer won't fail
        (*bonuses) = realloc(*bonuses, sizeof(Bonus*) * (*bonuscount));
      //  if(tmp != NULL)
      //    (*bonuses) = tmp;
      //  else // What do we do here? The original pointer still points to *bonuses
      //    free(*bonuses);
      }
    }
  }
  return 0;
}

int bonus_freebonuses(Bonus*** bonuses, int* bonuscount)
{
  for(int i = (*bonuscount)-1; i >= 0; i--)
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
  for(int i = (*bonuscount)-1; i >= 0; i--)
  {
    Bonus* bonus = (*bonuses)[i];
    bonus->y += 2;
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
