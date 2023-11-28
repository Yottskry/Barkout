#include "arena.h"

int arena_loadbricks(Arena* arena, ResourceFactory* factory, const char* fname)
{
  FILE* f = fopen(fname, "r");
  char rowdata[14];
  arena->brickcount = 0;
  int brickno = 0;
  int row = 0;

  // Read each line. This is the row position.
  while(fscanf(f, "%s", rowdata) > 0)
  {
    // Read each character. This is the column.
    for(int col = 0; col < strlen(rowdata); col++)
    {
      if(rowdata[col] == '.')
        continue;

      arena->bricks = realloc(arena->bricks, sizeof(Brick*) * ++arena->brickcount);

      arena->bricks[brickno] = malloc(sizeof(Brick));
      arena->bricks[brickno]->left = arena->bounds.left + (col * BRICKW);
      arena->bricks[brickno]->right = arena->bounds.left + (col * BRICKW) + BRICKW;
      arena->bricks[brickno]->top = arena->bounds.top + (row * BRICKH);
      arena->bricks[brickno]->bottom = arena->bounds.top + (row * BRICKH) + BRICKH;
      arena->bricks[brickno]->hitcount = 1;
      arena->bricks[brickno]->sprite = malloc(sizeof(Sprite));
      arena->bricks[brickno]->type = btNormal;

      Animation* brickanim;
      char c = rowdata[col];
      switch(c){
        case 'r':
          brickanim = af_getanimation(factory, "red");
        break;
        case 'b':
          brickanim = af_getanimation(factory, "blue");
        break;
        case 'g':
          brickanim = af_getanimation(factory, "green");
        break;
        case 'p':
          brickanim = af_getanimation(factory, "purple");
        break;
        case 'w':
          brickanim = af_getanimation(factory, "yellow");
        break;
        case 'y':
          brickanim = af_getanimation(factory, "grey");
        break;
        case 'G':
          arena->bricks[brickno]->hitcount = 2;
          arena->bricks[brickno]->type = btHard;
          brickanim = af_getanimation(factory, "darkgrey");
        break;
        case 'O':
          arena->bricks[brickno]->hitcount = -1;
          arena->bricks[brickno]->type = btIndestructible;
          brickanim = af_getanimation(factory, "orange");
        break;
      }
      arena->bricks[brickno]->sprite->anim = brickanim;
      arena->bricks[brickno]->sprite->state = asStatic;
      arena->bricks[brickno]->sprite->currentframe = 0;
      arena->bricks[brickno]->sprite->loop = 0;
      arena->bricks[brickno]->sprite->lastticks = 0;
      brickno++;
    }
    row++;
  }

  fclose(f);
  return 0;
}

int arena_drawbricks(Arena* arena, SDL_Renderer* renderer)
{
  for(int brickno = 0; brickno < arena->brickcount; brickno++)
  {
    if(arena->bricks[brickno]->hitcount != 0)
    {
      a_drawsprite(arena->bricks[brickno]->sprite, renderer, arena->bricks[brickno]->left, arena->bricks[brickno]->top);
    }
  }
  return 0;
}

int arena_freebricks(Arena* arena)
{
  for(int brickno = 0; brickno < arena->brickcount; brickno++)
  {
    free(arena->bricks[brickno]->sprite);
    free(arena->bricks[brickno]);
  }

  free(arena->bricks);
  return 0;
}

Bonus* arena_addbonus(Arena* arena, int x, int y, Bonustype type)
{
  Bonus* bonus = malloc(sizeof(Bonus));
  bonus->sprite = malloc(sizeof(Sprite));
  bonus->sprite->currentframe = 0;
  bonus->sprite->lastticks = 0;
  bonus->sprite->anim = af_getanimation(arena->factory, "bonus-d");
  bonus->sprite->loop = 1;
  bonus->sprite->state = asMoving;
  bonus->x = x;
  bonus->y = y;

  arena->bonuses = realloc(arena->bonuses, sizeof(Bonus*) * ++arena->bonuscount);
  arena->bonuses[arena->bonuscount-1] = bonus;

  return bonus;
}

int arena_movebonuses(Arena* arena, Bat* player)
{
  for(int i = 0; i < arena->bonuscount; i++)
  {
    Bonus* bonus = arena->bonuses[i];
    bonus->y += 1;
    if(bonus->y > arena->bounds.bottom)
    {
      arena_freebonus(arena, bonus);
    }
  }
}

int arena_freebonus(Arena* arena, Bonus* bonus)
{
  for(int i = 0; i < arena->bonuscount; i++)
  {
    // Find the item to be removed
    if(arena->bonuses[i] == bonus)
    {

    }
  }
  return 0;
}

int arena_drawbonuses(Arena* arena, SDL_Renderer* renderer)
{
  for(int i = 0; i < arena->bonuscount; i++)
  {
    a_drawsprite(arena->bonuses[i]->sprite, renderer, arena->bonuses[i]->x, arena->bonuses[i]->y);
  }
  return 0;
}
