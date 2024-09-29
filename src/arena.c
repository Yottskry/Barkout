#include "arena.h"
#include "config.h"
#include <assert.h>
#ifndef _WIN32
#include <dirent.h>
#else
#include <windows.h>
#endif // WIN32

static void arena_resetBrickParticles(Brick* brick, bool hide)
{
  for(int j = 0; j < MAXBRICKPARTICLES; j++)
  {
    brick->particles[j].alpha = hide ? 0 : (rand() % 55) + 201;
    brick->particles[j].gdiff = 0;
    brick->particles[j].x = rand() % (brick->right - brick->left);
    brick->particles[j].y = rand() % (brick->bottom - brick->top);
    brick->particles[j].yv = ((rand() % 6) + 1) * -1;
    if(brick->particles[j].x < 10)
      brick->particles[j].xv = ((rand() % 3) * -1) - 3; // -6 to -3
    else if(brick->particles[j].x < 20)
      brick->particles[j].xv = (rand() % 3) * -1; // -2 to 0
    else if(brick->particles[j].x < 30)
      brick->particles[j].xv = (rand() % 3); // 0 to 2
    else
      brick->particles[j].xv = (rand() % 3) + 3; // 3 to 6
    brick->particles[j].x += brick->left;
    brick->particles[j].y += brick->top;
  }
}

int arena_loadBinary(ResourceFactory* factory, Arena* arena, char* fname)
{
  Uint32 count = 0;

  FILE* f = fopen(fname, "rb");

  TEST_LOADED(f, fname);

  assert(f != NULL);
  // Read number of levels
  fread(&count, sizeof(Uint32), 1, f);

  arena->numlevels = count;
  arena->levels = malloc(arena->numlevels * sizeof(Level));

  TEST_ALLOC(arena->levels)

  // Loop through levels
  for(unsigned int i = 0; i < count; i++)
  {
    if(config_getdebug())
      printf("Loading level %d\n", i+1);
    else
      printf("#");
    Uint32 levelsize = 0;
    // How many bytes for this level?
    fread(&levelsize, sizeof(Uint32), 1, f);

    // We know levels are going to be small, so read the whole level in one go
    char buffer[levelsize + 1]; // Allow space for a LF
    memset(buffer, 0, levelsize + 1);
    fread(buffer, sizeof(char), levelsize, f);

    for(Uint32 j = 0; j < levelsize; j++)
    {
      // Flip the bits (input file obfuscated slightly)
      buffer[j] = buffer[j] ^ 0xFF;
    }

    // Now what? Buffer is basically the level file.
    // Could just build the level data from this point.
    Level* level = &(arena->levels[i]);
    level->bricks = NULL;
    level->onlevelend = NULL;
    level->maxbonuslevel = 7;
    level->brickcount = 0;
    level->cats = vector_new();

    // Avoid warning about different signedness
    if((int)i == arena->numlevels-1)
      level->onlevelend = arena_finalLevelEnd;

    int brickno = 0;
    int row = 0;

    // strtok treats contiguous delimiters as a single delimiter, so whether we end in \n or \r\n it will work
    char* rowdata = strtok(buffer, "\r\n");
    // First row is the background
    char bgname[4] = "";
    char mgname[8] = "";
    char fgname[8] = "";
    strcpy(bgname, buffer);
    strcpy(mgname, bgname);
    strcpy(fgname, bgname);
    strcat(mgname, "-mg");
    strcat(fgname, "-fg");
    level->bg = af_getanimation(factory, bgname);
    level->mg = af_getanimation(factory, mgname);
    level->fg = af_getanimation(factory, fgname);

    rowdata = strtok(NULL, "\r\n");
    // If there was no max bonus level specified in the file, don't get another token
    if(sscanf(rowdata, "%d", &level->maxbonuslevel) == 1)
      rowdata = strtok(NULL, "\r\n");
    // Read each line. This is the row position.
    // Next token should be the level data itself

    while(rowdata != NULL)
    {
      if(config_getdebug())
        printf("%s\n", rowdata);
      levels_processRow(factory, level, &(arena->bounds), rowdata, row, &brickno);
      rowdata = strtok(NULL, "\r\n");
      row++;
    }
  } // end level
  if(!config_getdebug())
    printf("\n");
  fclose(f);
  return 0;
}

int arena_loadLevels(Arena* arena, ResourceFactory* factory)
{
  arena->numlevels = 0;
  char apath[255] = "./Levels/";

  #ifdef INSTALLDIR
  if(config_getinstalled())
    strcpy(apath, INSTALLDIR "/Levels/");
  #endif

  #ifndef _WIN32
  DIR* d;
  struct dirent* dir;
  d = opendir(apath);

  assert(d);

  if(d)
  {
    while((dir = readdir(d))!=NULL)
    {
      if(dir->d_type != DT_REG)
        continue;
      int len = strlen(dir->d_name);
      char* cpy = calloc(sizeof(char), len+1);
      TEST_ALLOC(cpy)
      strcpy(cpy, dir->d_name);
      if(strcmp(cpy+(len-4), ".lvl")==0)
        arena->numlevels++;
      free(cpy);
    }
    closedir(d);
  }
  #else
  HANDLE hFind;
  WIN32_FIND_DATA fd;

  strcat(apath, "*.lvl");

  hFind = FindFirstFile("./Levels/*.lvl", &fd);
  if(hFind != INVALID_HANDLE_VALUE)
  {
    arena->numlevels++;
    while(FindNextFile(hFind, &fd))
    {
      arena->numlevels++;
    }
    FindClose(hFind);
  }
  #endif // _WIN32

  arena->levels = malloc(arena->numlevels * sizeof(Level));

  TEST_ALLOC(arena->levels)

  for(int i = 0; i < arena->numlevels; i++)
  {
    arena->levels[i].level = i + 1;

    Level* level = &(arena->levels[i]);
    level->bricks = NULL;
    level->onlevelend = NULL;
    level->cats = vector_new();

    if(i == arena->numlevels-1)
      level->onlevelend = arena_finalLevelEnd;

    char apath[255] = "./Levels/";

    #ifdef INSTALLDIR
    if(config_getinstalled())
      strcpy(apath, INSTALLDIR "/Levels/");
    #endif // INSTALLDIR


    char fname[20] = "";
    sprintf(fname, "/level%d.lvl", level->level);
    strcat(apath, fname);

    FILE* f = fopen(apath, "r");
    char rowdata[14];

    level->brickcount = 0;
    int brickno = 0;
    int row = 0;

    char bgname[4] = "bg1";

    fscanf(f, "%s", bgname);

    level->maxbonuslevel = 7;
    fscanf(f, "%d", &level->maxbonuslevel);

    char mgname[8] = "";
    char fgname[8] = "";

    strcpy(mgname, bgname);
    strcpy(fgname, bgname);

    strcat(mgname, "-mg");
    strcat(fgname, "-fg");

    level->bg = af_getanimation(factory, bgname);
    level->mg = af_getanimation(factory, mgname);
    level->fg = af_getanimation(factory, fgname);

    // Read each line. This is the row position.
    while(fscanf(f, "%s", rowdata) > 0)
    {
      levels_processRow(factory, level, &arena->bounds, rowdata, row, &brickno);
      row++;
    }

    fclose(f);
  }
  return 0;
}

void arena_loadBricks(Arena* arena, int level)
{
  arena->bricks = arena->levels[level-1].bricks;
  arena->brickcount = arena->levels[level-1].brickcount;
  arena->bg = arena->levels[level-1].bg;
  arena->mg = arena->levels[level-1].mg;
  arena->fg = arena->levels[level-1].fg;
  arena->remaining = 0;

  for(int i = 0; i < arena->levels[level-1].brickcount; i++)
  {
    Brick* brick = arena->levels[level-1].bricks[i];

    if(((brick->type & btIndestructible) != btIndestructible) &&
       (brick->type != btWormhole) &&
       (brick->type != btResurrecting))
    {
      arena->remaining++;
    }

    brick->hitcount = brick->starthitcount * arena->multiplier;
    brick->isdead = false;
    brick->left = brick->startleft;
    brick->right = brick->startright;

    if((brick->type & btResurrecting) == btResurrecting)
    {
      brick->isdead = true; // avoids particles
      brick->hitcount = 1;
      brick->counter = 0;
      brick->sprite->currentframe = 0;
      af_setanimation(arena->factory, brick->sprite, "grey-broken", 0, arena_brickFinished, (void*)brick, (void*)(arena->factory));
      brick->sprite->state = asStatic;
    }

    if((brick->type & btSwitch) == btSwitch)
    {
      arena_resetBrickParticles(brick, true);
    }
    else
    {
      arena_resetBrickParticles(brick, false);
    }
  }
}

void arena_drawBricks(Arena* arena, SDL_Renderer* renderer)
{
  // Draw wormholes first so they appear below other bricks in Z-order
  for(int brickno = 0; brickno < arena->brickcount; brickno++)
  {
    Brick* brick = arena->bricks[brickno];
    if(brick->type != btWormhole)
      continue;

    a_drawsprite(brick->sprite, renderer, brick->left, brick->top);
  }

  for(int brickno = 0; brickno < arena->brickcount; brickno++)
  {
    Brick* brick = arena->bricks[brickno];

    if(brick->type == btWormhole)
      continue;

    Animation* anim = brick->sprite->anim;

    if(brick->counter > 0)
    {
      brick->counter--;
      if(brick->counter == 0)
      {
        af_playsample(arena->factory, "wormhole-out");
        //brick->hitcount = 1;
        //brick->isdead = false;
        //continue;
      }
    }

    if((brick->hitcount != 0) || (((brick->type & btResurrecting) == btResurrecting) && (brick->counter == 0)))
    {
      a_drawsprite(brick->sprite, renderer, brick->left, brick->top);
    }
    else if(!brick->isdead)
    {
      brick->isdead = true;



      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      for(int j = 0; j < config_getbrickparticles(); j++)
      {
        if(brick->particles[j].alpha > 0)
        {
          brick->isdead = false;
          int w = (rand() % 4) + 1;
          SDL_SetRenderDrawColor(renderer, anim->keycolorr, anim->keycolorg, anim->keycolorb, (Uint8)brick->particles[j].alpha);
          SDL_RenderFillRect(renderer, &(SDL_Rect){.x = brick->particles[j].x, .y = brick->particles[j].y, .w = w, .h = w});
          brick->particles[j].x += brick->particles[j].xv;
          brick->particles[j].y += brick->particles[j].yv;

          brick->particles[j].yv = brick->particles[j].yv < 5 ? brick->particles[j].yv + 1 : brick->particles[j].yv;

          if(brick->particles[j].alpha < BRICKDECAY)
            brick->particles[j].alpha = 0;
          else
            brick->particles[j].alpha -= rand() % BRICKDECAY;
        }
      }
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

      // Only reduce the brick count for non-switch type bricks.
      if(((brick->type & btSwitch) != btSwitch) && ((brick->type & btResurrecting) != btResurrecting))
      {
        if (brick->isdead)
        {
        arena->remaining--;
        if(arena->remaining == 0)
          if(arena->levels[arena->level-1].onlevelend != NULL)
          {
            arena->levels[arena->level-1].onlevelend(arena);
            return;
          }
        }
        else if(arena->remaining == 1)
        {
          // Borrow this for another purpose (stopping the ball)
          arena->bonuscounter = -1;
        }
      }
    }
  }
}

void arena_moveBricks(Arena* arena, Ball* ball)
{
  for(int brickno = 0; brickno < arena->brickcount; brickno++)
  {
    Brick* brick = arena->bricks[brickno];

    if((brick->type & btMoving) == btMoving)
    {
      // Test this brick against all other bricks for a collision... shame we're not using a grid
      // then we'd need test only those in the same row. Oh well.
      // Only if a brick collides with no others can it move.
      Bounds b1 = { .left = brick->left,
                    .top = brick->top,
                    .width = brick->right - brick->left,
                    .height = brick->bottom - brick->top };

      // Need to check the brick in each position until it reaches its final destination
      // to see if it's moving into the ball

      int targetx = b1.left + brick->speed;
      bool leave = false;
      do
      {
        //leave = false;
        b1.left += brick->speed < 0 ? -1 : 1;
        Edge e = eNone;
        int delta = 0;
        // Check for collision with ball
        if(ball_collidesBounds(ball, &b1, &e, &delta))
        {
          // Collision, so deflect the ball (but do nothing to the brick)
          // Actually....don't we only want to ricochet if the ball is travelling
          // the opposite direction to the brick? If it's the same direction, the ball
          // should not then change to moving towards the brick.
          ball_ricochet(ball, e);

          switch(e)
          {
            case eRight:
              ball->cx = b1.left + b1.width + ball->radius + 1;
            break;
            case eLeft:
              ball->cx = b1.left - (ball->radius + 1);
            break;
            case eBottom:
              ball->cy = b1.top + b1.height + ball->radius + 1;
            break;
            case eTop:
              ball->cy = b1.top - (ball->radius + 1);
            break;
            default:
            break;
          };
        }

        if((b1.left < arena->bounds.left) || ((b1.left + b1.width) > arena->bounds.right))
        {
          brick->speed *= -1;
          return;
        }

        for(int brickno2 = 0; brickno2 < arena->brickcount; brickno2++)
        {
          Brick* brick2 = arena->bricks[brickno2];

          if(brick == brick2)
            continue;

          if(brick2->hitcount == 0)
            continue;

          Bounds b2 = { .left = brick2->left,
                        .top = brick2->top,
                        .width = brick2->right - brick2->left,
                        .height = brick2->bottom - brick2->top };

          if(bounds_intersects(&b1, &b2))
          {
            brick->speed *= -1;
            leave = true;
            break;
          }

        }

        if(leave)
          break;

      } while(b1.left != targetx);

      // No collision, move brick
      if(!leave)
      {
        brick->left = b1.left;
        brick->right = b1.left + b1.width;
      }
    }
  }
}

void arena_resetBricks(Arena* arena)
{
  for(int brickno = 0; brickno < arena->brickcount; brickno++)
  {
    arena->bricks[brickno]->counter = 0;
    arena->bricks[brickno]->hitcount = arena->bricks[brickno]->starthitcount;
    arena->bricks[brickno]->isdead = false;
    arena->bricks[brickno]->left = arena->bricks[brickno]->startleft;
    arena->bricks[brickno]->right = arena->bricks[brickno]->startright;

    if((arena->bricks[brickno]->type & btResurrecting) == btResurrecting)
    {
      arena->bricks[brickno]->hitcount = 1;
      arena->bricks[brickno]->isdead = true;
      af_setanimation(arena->factory, arena->bricks[brickno]->sprite, "grey-broken", 0, arena_brickFinished, (void*)arena->bricks[brickno], (void*)arena->factory);
      arena->bricks[brickno]->sprite->state = asStatic;
      arena->bricks[brickno]->sprite->currentframe = 0;
    }
  }
}

void arena_freeLevels(Arena* arena)
{
  for(int levno = 0; levno < arena->numlevels; levno++)
  {
    for(int brickno = 0; brickno < arena->levels[levno].brickcount; brickno++)
    {
      free(arena->levels[levno].bricks[brickno]->sprite);
      free(arena->levels[levno].bricks[brickno]);
    }

    free(arena->levels[levno].bricks);
    arena->levels[levno].bricks = NULL;
    arena->levels[levno].brickcount = 0;
    for(int cn = 0; cn < arena->levels[levno].cats->size; cn++)
      free(arena->levels[levno].cats->elements[cn]);
    vector_free(arena->levels[levno].cats);
  }

  free(arena->levels);
}

Bonus* arena_addBonus(Arena* arena, int x, int y, Bonustype type)
{
  // There was a memory leak reported by valgrind here...
  // but actually the leak is because I hadn't yet freed
  // arena->bonuses (et al) before the program exits
  Bonus* result = NULL;
  Bonus** tmp = realloc(arena->bonuses, sizeof(Bonus*) * ++arena->bonuscount);
  if(tmp != NULL)
  {
    arena->bonuses = tmp;

    TEST_ALLOC(arena->bonuses)

    arena->bonuses[arena->bonuscount - 1] = malloc(sizeof(Bonus));

    TEST_ALLOC(arena->bonuses[arena->bonuscount - 1])

    Bonus* bonus = arena->bonuses[arena->bonuscount - 1];
    bonus->sprite = malloc(sizeof(Sprite));

    TEST_ALLOC(bonus->sprite)

    bonus->sprite->currentframe = 0;
    bonus->sprite->lastticks = 0;
    switch(type)
    {
      case boDeadly: bonus->sprite->anim = af_getanimation(arena->factory, "bonus-d"); break;
      case boShrink: bonus->sprite->anim = af_getanimation(arena->factory, "bonus-s"); break;
      case boCatch: bonus->sprite->anim = af_getanimation(arena->factory, "bonus-c"); break;
      case boPlayer: bonus->sprite->anim = af_getanimation(arena->factory, "bonus-p"); break;
      case boGrow: bonus->sprite->anim = af_getanimation(arena->factory, "bonus-e"); break;
      case boLaser: bonus->sprite->anim = af_getanimation(arena->factory, "bonus-l"); break;
      case boWarp: bonus->sprite->anim = af_getanimation(arena->factory, "bonus-w"); break;
      //case boSlow: break;
      case boNone: break;
    }

    bonus->sprite->loop = 1;
    bonus->sprite->state = asLooping;
    bonus->x = x;
    bonus->y = y;
    bonus->w = 43;
    bonus->h = 25;
    bonus->type = type;

    arena->bonuses[arena->bonuscount-1] = bonus;
    result = bonus;
  }
  return result;
}

Bonus* arena_batCollidesBonus(Arena* arena, Bat* player, Ball* ball)
{
  for(int i = 0; i < arena->bonuscount; i++)
  {
    int bx = arena->bonuses[i]->x + arena->bonuses[i]->w;
    int by = arena->bonuses[i]->y + arena->bonuses[i]->h;
    int br = (int)(arena->bonuses[i]->w / 2);


    if((by > player->y) &&
        (bx+br > player->x) &&
        (bx-br < player->x + (int)(player->w)))
    {
      // Lose any existing Deadly or Catch power.
      ball->state = bsNormal;
      player->state = plNormal;
      af_setanimation(arena->factory, &(ball->sprite), "ball", 1, NULL, NULL, NULL);
      af_setanimation(arena->factory, &(player->sprite), "bat", 1, NULL, NULL, NULL);

      switch(arena->bonuses[i]->type)
      {
        case boShrink:
          // If the bat size is currently long, don't play the grow animation
          if(player->w != psShort)
            af_setanimation(arena->factory, &(player->sprite),"bat-shrink", 0, bat_afterShrink, (void*)arena, (void*)player);
          else
            bat_afterShrink((void*)arena, (void*)player);
          ball->state = bsNormal;
          player->state = plShort;
        break;
        case boDeadly:
          ball->state = bsDeadly;
          af_setanimation(arena->factory, &(ball->sprite), "ball-deadly", 1, NULL, NULL, NULL);
        break;
        case boCatch: ball->state = bsLoose; break;
        case boPlayer:
          player->lives++;
          af_playsample(arena->factory, "1up");
        break;
        case boGrow:
          // If the bat size is currently short, don't play the grow animation
          if(player->w != psLong)
            af_setanimation(arena->factory, &(player->sprite),"bat-grow", 0, bat_afterGrow, (void*)arena, (void*)player);
          else
            bat_afterGrow((void*)arena, (void*)player);
          ball->state = bsNormal;
          player->state = plLong;
        break;
        case boLaser:
          af_setanimation(arena->factory, &(player->sprite),"bat-laserify", 0, bat_afterLaser, (void*)arena, (void*)player);
          player->sprite.state = asPlayToEnd;
          player->state = plLaser;
        break;
        case boWarp:
          player->warpenabled = true;
        break;
        //case boSlow: break;
        case boNone: break;
      }

      if(player->state != plShort && player->state != plLong)
        player->w = psNormal;

      bonus_freebonus(&arena->bonuses, &arena->bonuscount, arena->bonuses[i]);
    }
  }
  return NULL;
}

void bat_afterShrink(void* sender, void* data)
{
  Bat* player = (Bat*)data;
  Arena* arena = (Arena*)sender;
  af_setanimation(arena->factory, &(player->sprite),"bat-s", 1, NULL, NULL, NULL);
  player->sprite.state = asLooping;
  player->w = psShort;
}

void bat_afterGrow(void* sender, void* data)
{
  Bat* player = (Bat*)data;
  Arena* arena = (Arena*)sender;
  af_setanimation(arena->factory, &(player->sprite),"bat-l", 1, NULL, NULL, NULL);
  player->sprite.state = asLooping;
  player->w = psLong;
}

void bat_afterLaser(void* sender, void* data)
{
  Bat* player = (Bat*)data;
  Arena* arena = (Arena*)sender;
  af_setanimation(arena->factory, &(player->sprite), "bat-laser", 1, NULL, NULL, NULL);
  player->sprite.state = asLooping;
}

int ball_moveBall(Ball* ball, Arena* arena, Bat* player)
{
  // Using the speed as the hypotenuse of the triangle,
  // use trig to work out the next X and Y coordinates.

  if((ball->bearing > 360) || (ball->bearing < 0))
    printf("Ball bearing is %f\n", ball->bearing);

  if (ball->state == bsNormal || ball->state == bsLoose || ball->state == bsDeadly)
    {
    // Convert degrees to radians
    double rads = ball->bearing * (PI / 180);
    Edge hitedge = eNone;
    Brick* b = NULL;

    int ballx = ball->cx;
    int bally = ball->cy;

    // Hold the last known non-collision position
    // so we can revert to it on detecting a collision
    int lastx = ball->cx;
    int lasty = ball->cy;

    // Check all the points on the path along which the
    // ball will move in this iteration.
    for(int spd = 0; spd <= ball->speed; spd++)
    {
      int nextx = spd * sinl(rads);
      int nexty = spd * cosl(rads);
/*
      if((nextx == 0) && (nexty == 0))
      {
        lastx = ball->cx;
        lasty = ball->cy;
        continue;
      }
*/
      if(ball->bearing < 180)
      {
        ball->cy = bally - nexty;
        ball->cx = ballx + nextx;
      }
      else if (ball->bearing < 270)
      {
        ball->cy = bally - nexty;
        ball->cx = ballx + nextx;
      }
      else
      {
        ball->cy = bally - nexty;
        ball->cx = ballx + nextx;
      }

      ball->x = ball->cx - ball->radius;
      ball->y = ball->cy - ball->radius;

      b = ball_collidesBricks(ball, arena->bricks, player, arena->brickcount, &(arena->bonuscounter), &hitedge);

      // We've hit a brick. Ball will be positioned
      // on the brick edge
      if(b!=NULL)
      {
        if(b->type == btWormhole)
        {
          // If we hit a wormhole, check the collision again to
          // ensure it's in the middle section, not an edge, but
          // if it is an edge we don't yet treat that as no collision.

          Bounds wbound = { .left = b->left + 27, .width = 25, .height = 25, .top = b->top + 12 };

          int delta = 0;
          if(ball_collidesBounds(ball, &wbound, &hitedge, &delta))
          {
            // Prevent ricochet from the wormhole
            hitedge = eNone;
            if(b != ball->warpdest)
            {
              int index = 0;
              // Find the other wormhole brick
              for(int j = 0; j < arena->brickcount; j++)
              {
                if(arena->bricks[j] == b)
                {
                  index = j;
                }
              }

              int j = index;
              j++;
              while(j != index)
              {
                if(j==arena->brickcount)
                  j = 0;
                // Allow us to warp to ourselves as a failsafe in case only one wormhole is present
                if((arena->bricks[j]->type == btWormhole) /*&& (arena->bricks[j] != b)*/)
                {
                  ball->warpdest = arena->bricks[j];
                  ball->cx = arena->bricks[j]->left + 40;
                  ball->cy = arena->bricks[j]->top + 24;
                  ball->x = ball->cx - ball->radius;
                  ball->y = ball->cy - ball->radius;
                  af_playsample(arena->factory, "wormhole-in");
                  return 0;
                }
                j++;
              }
            }
            // Uncomment this to have the ball slowly expelled from the hole!
            //return 0;
            //break;
          }
          else
          {
            b = NULL;
            hitedge = eNone;
          }
        }
        else if((b->type & btSwitch) == btSwitch)
        {
          b->hitcount--;
          for(int j = 0; j < arena->brickcount; j++)
          {
            if(arena->bricks[j] != b)
            {
              if((arena->bricks[j]->type & btSwitch) == btSwitch)
              {
                if(arena->bricks[j]->hitcount == 0)
                {
                  arena->bricks[j]->hitcount = 1;
                  arena->bricks[j]->isdead = false;
                  arena_resetBrickParticles(arena->bricks[j], false);
                }
                else
                {
                  arena->bricks[j]->hitcount = 0;
                }
              }
            }
          }
        }
        else if((b->type & btResurrecting) == btResurrecting)
        {
          // What if we hit more than one resurrecting brick at once?

          // Collided with a resurrecting brick that is invisible. Do nothing.
          //if(b->sprite->currentframe > 0)
          //{
            // vanish animation is playing, don't register another hit
          //  b = NULL;
          //  hitedge = eNone;
          //}
          //else
          //{
            // first hit before animation starts
            //b->sprite->state = asPlayToEnd;
            player->score += BRICKSCORE;
            af_playsample(arena->factory, "brick");
          //}

        }
        else
        {
          // Bug... if multiple bricks are destroyed we're only adding on the score for one
          //arena->score += BRICKSCORE;

          //if((b->type != btIndestructible) && (b->type != btWormhole) && (b->type != btResurrecting) && (b->hitcount == 0))
            // The hit edge is not solid (i.e. it can be destroyed on that edge)
            //if(!(b->solidedges & hitedge))
            //  arena->remaining--;

          b->sprite->state = asPlayAndReset;

          // There's a one in BONUSFREQUENCY chance of a bonus being created
          // If it's normal and not a boss, decide whether to create a bonus
          if((rand() % BONUSFREQUENCY == 0) && ((b->type & btNormal) == btNormal) && ((b->type & btBoss) != btBoss) && (arena->bonuscount < 2))
          {
            long bonusscore = arena->bonuscounter;
            int bonustype = bonusscore % (arena->levels[arena->level].maxbonuslevel);
            Bonustype bt = (Bonustype)(bonustype+1);
            arena_addBonus(arena, b->left, b->bottom, bt);
            arena->bonuscounter = 0;

         /*
          if((arena->bonuscounter % BONUSFREQUENCY == 0) &&
             (b->hitcount == 0) &&  &&
              && ((b->solidedges & hitedge) != hitedge))
          {
            int bonusscore = rand() % 100;
            Bonustype botype = boNone;

            if(bonusscore > 90)
              botype = boGrow;
            else if (bonusscore > 85)
              botype = boDeadly;
            else if (bonusscore > 82)
              botype = boPlayer;
            else if (bonusscore > 75)
              botype = boCatch;
            else if (bonusscore > 70)
              botype = boLaser;
            else if (bonusscore > 63)
              botype = boShrink;
            else if (bonusscore > 60)
              botype = boWarp;

            // We can limit the maximum bonus on each level if we want
            // e.g. to prevent laser or warp on the boss level
            if((botype != boNone) && ((int)botype <= arena->levels[arena->level - 1].maxbonuslevel))
              arena_addBonus(arena, b->left, b->bottom, botype);
            */
            af_playsample(arena->factory, "brick");
          }
          else if((b->type & btBoss) == btBoss)
          {
            if(b->hitcount == 0)
              af_playsample(arena->factory, "meow");
            else
              af_playsample(arena->factory, "ow");
          }
          else if((b->type & btNormal) == btNormal)
          {
            af_playsample(arena->factory, "brick");
          }
          else
          {
            af_playsample(arena->factory, "brick-high");
            // This is where we check for moving bricks and position the ball accordingly.
            // If we hit a moving brick we should move the ball to the edge of that brick
            // Should we ever have moving normal bricks we'll need to account for the deadly
            // powerup and not reposition the ball at the brick's edge

            // Note: best to do this for all indestructible bricks, or we risk a moving
            // brick pushing the ball into a static brick and it sticking there.

            //if((b->type & btMoving) == btMoving)
            //{
              switch(hitedge)
              {
                case eRight:
                  lastx = b->right + b->speed + ball->radius + 1;
                break;
                case eLeft:
                  lastx = b->left + b->speed - (ball->radius + 1);
                break;
                case eBottom:
                  lasty = b->bottom + ball->radius + 1;
                break;
                case eTop:
                  lasty = b->top - (ball->radius + 1);
                break;
                default:
                break;
              };
            //}

          }

          // Normal brick, deadly ball, so no ricochet
          if(((b->type & btNormal) == btNormal) && (ball->state == bsDeadly) && (!(b->solidedges & hitedge)))
            hitedge = eNone;

          // Because we break here, lastx/y are not updated
          // so retain the last non-collision position

          //break; // Go to EndLoop so we stop testing for collisions and just destroy the brick and carry on
        }
        break; // We've hit a brick, so don't keep processing the path of the ball and instead ricochet
      }
      else // b == null
      {
        // No contact with any brick so we can say we're clear of the warp destination
        ball->warpdest = NULL;
      }

      if(1 == ball_collidesBat(ball, player, &hitedge))
      {
        af_playsample(arena->factory, "bat");
        if(ball->state == bsLoose)
          ball->state = bsStuck;

        Uint32 curticks = SDL_GetTicks();

        if(curticks - arena->counter >= 60000)
        {
          if(ball->speed < MAXSPEED)
            ball->speed++;
          arena->counter = curticks;
        }

        break;
      }

      if(ball->cy - ball->radius < arena->bounds.top)
      {
        ball->cy = arena->bounds.top + ball->radius;
        hitedge = eBottom;
      }

      if(ball->cy + ball->radius > arena->bounds.bottom)
      {
        ball->cy = arena->bounds.bottom - ball->radius;
        hitedge = eTop;
        return 1;
      }

      if(ball->cx + ball->radius > arena->bounds.right)
      {
        ball->cx = arena->bounds.right - ball->radius;
        hitedge = eLeft;
      }

      if(ball->cx - ball->radius < arena->bounds.left)
      {

        ball->cx = arena->bounds.left + ball->radius;
        hitedge = eRight;
      }
      // If we've hit a brick, we've broken out already
      // so these store the last non-collision position.
      // If we haven't, these hold the correct position.
      lastx = ball->cx;
      lasty = ball->cy;
    } // EndLoop

    if(b!=NULL)
    {
      ball->cx = lastx;
      ball->cy = lasty;
    }

    //if(hitedge != eNone)
    //  arena->bonuscounter++;

    ball_ricochet(ball, hitedge);

  }
  else if(ball->state == bsSticky || ball->state == bsStuck)
  {
    // Move the ball's position with the bat if stuck to it
    ball->cx += player->speed;
    ball->cy = player->y - (ball->radius);
  }

  ball->x = ball->cx - ball->radius;
  ball->y = ball->cy - ball->radius;

  return 0;
}

void arena_addBullet(Arena* arena, Bat* player)
{
  if(arena->bulletcount >= MAXBULLETS)
    return;

  arena->bulletcount++;
  arena->bullets = realloc(arena->bullets, sizeof(Bullet*) * arena->bulletcount);
  TEST_ALLOC(arena->bullets)
  arena->bullets[arena->bulletcount - 1] = malloc(sizeof(Bullet));
  TEST_ALLOC(arena->bullets[arena->bulletcount - 1])
  arena->bullets[arena->bulletcount - 1]->speed = 5;
  arena->bullets[arena->bulletcount - 1]->x = player->x + 22;
  arena->bullets[arena->bulletcount - 1]->y = player->y;

  arena->bulletcount++;
  arena->bullets = realloc(arena->bullets, sizeof(Bullet*) * arena->bulletcount);
  TEST_ALLOC(arena->bullets)
  arena->bullets[arena->bulletcount - 1] = malloc(sizeof(Bullet));
  TEST_ALLOC(arena->bullets[arena->bulletcount - 1])
  arena->bullets[arena->bulletcount - 1]->speed = 5;
  arena->bullets[arena->bulletcount - 1]->x = player->x + player->w - 22 - 5; // 5 for bullet width
  arena->bullets[arena->bulletcount - 1]->y = player->y;
}

void arena_moveBullets(Arena* arena)
{
  for(int i = 0; i < arena->bulletcount; i++)
  {
    arena->bullets[i]->y -= arena->bullets[i]->speed;
  }
}

void arena_drawBullets(Arena* arena, SDL_Renderer* renderer)
{
  for(int i = 0; i < arena->bulletcount; i++)
  {
    Bullet* b = arena->bullets[i];
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rect = { .x = b->x, .y = b->y, .w = 5, .h = 8 };
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect2 = { .x = b->x+1, .y = b->y+1, .w = 3, .h = 6 };
    SDL_RenderFillRect(renderer, &rect2);
  }
}

void arena_freeBullet(Arena* arena, int index)
{
  free(arena->bullets[index]);
  arena->bullets[index] = NULL;
  arena->bulletcount--;

  for(int i = index; i < arena->bulletcount; i++)
  {
    arena->bullets[i] = arena->bullets[i+1];
  }

  if(arena->bulletcount > 0)
    arena->bullets = realloc(arena->bullets, sizeof(Bullet*) * arena->bulletcount);
  else
  {
    free(arena->bullets);
    arena->bullets = NULL;
  }
}

void arena_freeBullets(Arena* arena)
{
  for(int i = arena->bulletcount-1; i >= 0; i--)
  {
    arena_freeBullet(arena, i);
  }
}

void arena_checkBulletCollisions(Arena* arena, Bat* player)
{
  // Check for bullets leaving the top of the arena
  for(int i = arena->bulletcount - 1; i >= 0; i--)
  {
    Bullet* bullet = arena->bullets[i];
    if(bullet->y < arena->bounds.top)
    {
      arena_freeBullet(arena, i);
      continue;
    }
  }

  // We could have included this within the loop above
  // but it would make it harder for two bullets hitting
  // one brick to only register as a single hit (which is what we want)
  for(int j = 0; j < arena->brickcount; j++)
  {
    bool hit = false;
    Brick* b = arena->bricks[j];
    // Skip invincible bricks and those that
    // are already knocked out.
    if((b->hitcount <= 0) && ((b->type & btIndestructible) != btIndestructible) && ((b->type & btResurrecting) != btResurrecting))
      continue;

    if(((b->type & btResurrecting) == btResurrecting) && (b->counter > 0))
      continue;

    if(((b->type & btSwitch) == btSwitch) && (b->hitcount == 0))
      continue;

    for(int i = arena->bulletcount - 1; i >= 0; i--)
    {
      Bullet* bullet = arena->bullets[i];
      if((bullet->y < b->bottom) &&
         (bullet->x + 5 > b->left) &&
         (bullet->x < b->right))
      {
          arena_freeBullet(arena, i);
          hit = true;
          continue; // test next bullet
      }
    }

    if((hit) && ((b->type & btIndestructible) != btIndestructible) && ((b->type & btResurrecting) != btResurrecting)){
      b->hitcount--;
      if(b->hitcount == 0)
      {
        player->score += BRICKSCORE;
        //arena->remaining--;
      }
      af_playsample(arena->factory, "brick-laser");
    }
  }
}

void arena_finalLevelEnd(void* sender)
{
  Arena* arena = (Arena*)(sender);
  af_playsampleforced(arena->factory, "victory", 1);
  arena_resetExplosions(arena);
}

void arena_resetExplosions(Arena* arena)
{
  Uint32 startticks = SDL_GetTicks();
  for(int i = 0; i < NUMEXPLOSIONS; i++)
  {
    arena->explosions[i].isdead = false;
    arena->explosions[i].x = rand() % arena->width;
    arena->explosions[i].y = rand() % 600;
    arena->explosions[i].startdelay = (rand() % 200) * 40;
    arena->explosions[i].color = (SDL_Color){rand() % 100 + 155, rand() % 100 + 155, rand() % 100 + 155, 255};
    arena->explosions[i].starttime = startticks;
    for(int j = 0; j < EXPLOSIONPARTICLES; j++)
    {
      arena->explosions[i].particles[j].alpha = 255;
      arena->explosions[i].particles[j].gdiff = 0;
      arena->explosions[i].particles[j].x = (rand() % 20) + arena->explosions[i].x;
      arena->explosions[i].particles[j].y = (rand() % 20) + arena->explosions[i].y;
      arena->explosions[i].particles[j].xv =(rand() % 15) - 8;
      arena->explosions[i].particles[j].yv =(rand() % 15) - 8;
    }
  }
}

bool arena_drawExplosions(Arena* arena, SDL_Renderer* renderer)
{
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  bool alldead = true;
  for(int i = 0; i < NUMEXPLOSIONS; i++)
  {
    Explosion* e = &arena->explosions[i];
    // If we've not yet reached the start time for this explosion, skip it
    if((SDL_GetTicks() - e->starttime) < e->startdelay)
    {
      // Can't be dead if not yet started
      alldead = false;
      continue;
    }

    // Play explosion only on first particle - after this, alldead will be false
      if(e->particles[0].alpha == 255)
        af_playsampleforced(arena->factory, "explosion", 2);

    for(int j = 0; j < EXPLOSIONPARTICLES; j++)
    {
      Sparkle* s = &e->particles[j];
      if(s->alpha > 0)
      {
        // if not yet invisible then not dead
        alldead = false;
        Uint16 a = s->alpha;
        SDL_SetRenderDrawColor(renderer, e->color.r, e->color.g, e->color.b, (Uint8)(a));
        SDL_Rect r = { .x = s->x, .y = s->y, .w = 2 , .h = 2 };
        SDL_RenderDrawRect(renderer, &r);
        s->x = s->x + s->xv;
        s->y = s->y + s->yv;
        s->alpha -= s->alpha < 2 ? s->alpha : 2;
      }
    }
  }
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
  return alldead;
}
