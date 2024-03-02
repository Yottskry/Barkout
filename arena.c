#include "arena.h"
#include "config.h"
#ifndef _WIN32
#include <dirent.h>
#else
#include <windows.h>
#endif // WIN32
/*  To avoid plaintext files, we take a normal level file (in the Delphi editor)
    we take each character in the file in turn and bit shift it left (in groups of 4)
    to form a Uint32. We then XOR that number with FFFFFFFF to invert it and save a
    file in that format. The very first UInt32 in the file is the number of Uints to
    read.

    So a file consists of a Uint32 counter (X) + X * Uint32s, including CRLF info.

*/

char* arena_loadbinary(char* fname)
{
  Uint32 i;

  FILE* f = fopen(fname, "rb");
  fread(&i, sizeof(Uint32), 1, f);
  Uint32 fsize = i;
  char* rowdata = calloc(i + 1, sizeof(Uint32));
  int j = 0;
  for(Uint32 x = 0; x < fsize; x++)
  {
    fread(&i, sizeof(Uint32), 1, f);
    i = i ^ 0xFFFFFFFF;
    // Got four characters.
    if ((i & 0x000000FF) != 0)
      rowdata[j++] = i & 0x000000FF;
    if (((i & 0x0000FF00) >> (1 * 8)) != 0)
      rowdata[j++] = ((i & 0x0000FF00) >> (1 * 8));
    if (((i & 0x00FF0000) >> (1 * 8)) != 0)
      rowdata[j++] = ((i & 0x00FF0000) >> (2 * 8));
    if (((i & 0xFF000000) >> (1 * 8)) != 0)
      rowdata[j++] = ((i & 0xFF000000) >> (3 * 8));
  }
  return rowdata;
}

int arena_loadlevels(Arena* arena, ResourceFactory* factory)
{
  arena->numlevels = 0;
  #ifndef _WIN32
  DIR* d;
  struct dirent* dir;
  d = opendir("./Levels/");
  if(d)
  {
    while((dir = readdir(d))!=NULL)
    {
      if(dir->d_type != DT_REG)
        continue;
      int len = strlen(dir->d_name);
      char* cpy = calloc(sizeof(char), len+1);
      strncpy(cpy, dir->d_name, len);
      if(strcmp(cpy+(len-4), ".lvl")==0)
        arena->numlevels++;
      free(cpy);
    }
    closedir(d);
  }
  #else
  HANDLE hFind;
  WIN32_FIND_DATA fd;
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

  for(int i = 0; i < arena->numlevels; i++)
  {
    arena->levels[i].level = i + 1;

    Level* level = &(arena->levels[i]);
    level->bricks = NULL;

    char fname[255] = "";

    sprintf(fname, "./Levels/level%d.lvl", level->level);


    //char fname2[255] = "";

    //sprintf(fname2, "./Levels/level%d.dat", 1);

    //char* rows = arena_loadbinary(fname2);

    FILE* f = fopen(fname, "r");
    char rowdata[14];

    level->brickcount = 0;
    int brickno = 0;
    int row = 0;
    //arena->remaining = 0;

    //char* rowdata = strtok(rows, "\r\n");

    char bgname[4] = "bg1";
//  strcpy(bgname, rowdata);
    fscanf(f, "%s", bgname);

    char mgname[8] = "";
    char fgname[8] = "";

    strcpy(mgname, bgname);
    strcpy(fgname, bgname);

    strcat(mgname, "-mg");
    strcat(fgname, "-fg");

    level->bg = af_getanimation(factory, bgname);
    level->mg = af_getanimation(factory, mgname);
    level->fg = af_getanimation(factory, fgname);

    level->spawnx = arena->bounds.left + (6 * BRICKW); // middle column by default
    level->spawny = arena->bounds.top;
    // Read each line. This is the row position.
    while(fscanf(f, "%s", rowdata) > 0)
  //rowdata = strtok(NULL, "\r\n");
    //while(rowdata != NULL)
    {
      // Read each character. This is the column.
      for(size_t col = 0; col < strlen(rowdata); col++)
      {
        if(rowdata[col] == '.')
          continue;

        if(rowdata[col] == '@')
        {
          level->spawnx = arena->bounds.left + (col * BRICKW);
          level->spawny = arena->bounds.top + (row * BRICKH);
          continue;
        }

        level->bricks = realloc(level->bricks, sizeof(Brick*) * ++level->brickcount);

        level->bricks[brickno] = malloc(sizeof(Brick));
        level->bricks[brickno]->left = arena->bounds.left + (col * BRICKW);
        level->bricks[brickno]->right = arena->bounds.left + (col * BRICKW) + BRICKW;
        level->bricks[brickno]->top = arena->bounds.top + (row * BRICKH);
        level->bricks[brickno]->bottom = arena->bounds.top + (row * BRICKH) + BRICKH;
        level->bricks[brickno]->hitcount = 1;
        level->bricks[brickno]->sprite = malloc(sizeof(Sprite));
        level->bricks[brickno]->type = btNormal;

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
          case 't':
            brickanim = af_getanimation(factory, "white");
          break;
          case 'G':
            level->bricks[brickno]->hitcount = 2;
            level->bricks[brickno]->type = btHard;
            brickanim = af_getanimation(factory, "darkgrey");
          break;
          case 'O':
            level->bricks[brickno]->hitcount = -1;
            level->bricks[brickno]->type = btIndestructible;
            brickanim = af_getanimation(factory, "orange");
          break;
          case '*':
            level->bricks[brickno]->hitcount = -1;
            level->bricks[brickno]->type = btWormhole;
            brickanim = af_getanimation(factory, "wormhole");
          break;
        }

        level->bricks[brickno]->sprite->anim = brickanim;
        level->bricks[brickno]->sprite->state = level->bricks[brickno]->type == btWormhole ? asLooping : asStatic;
        level->bricks[brickno]->sprite->currentframe = 0;
        level->bricks[brickno]->sprite->loop = level->bricks[brickno]->type == btWormhole ? 1 : 0;
        level->bricks[brickno]->sprite->lastticks = 0;
        level->bricks[brickno]->sprite->onanimfinished = NULL;
        level->bricks[brickno]->sprite->data = NULL;
        brickno++;
      }
      row++;
      //rowdata = strtok(NULL, "\r\n");
    } // _WHERE_

    fclose(f);
  }
  return 0;
}

void arena_loadbricks(Arena* arena, int level)
{
  arena->bricks = arena->levels[level-1].bricks;
  arena->brickcount = arena->levels[level-1].brickcount;
  arena->bg = arena->levels[level-1].bg;
  arena->mg = arena->levels[level-1].mg;
  arena->fg = arena->levels[level-1].fg;
  arena->remaining = 0;
  arena->spawnx = arena->levels[level-1].spawnx;
  arena->spawny = arena->levels[level-1].spawny;

  for(int i = 0; i < arena->levels[level-1].brickcount; i++)
  {
    Brick* brick = arena->levels[level-1].bricks[i];

    if((arena->levels[level-1].bricks[i]->type != btIndestructible) &&
       (arena->levels[level-1].bricks[i]->type != btWormhole))
    {
      arena->remaining++;
    }

    switch(brick->type)
    {
      case btNormal: arena->levels[level-1].bricks[i]->hitcount = 1; break;
      case btHard: arena->levels[level-1].bricks[i]->hitcount = 2; break;
      case btIndestructible: arena->levels[level-1].bricks[i]->hitcount = -1; break;
      case btWormhole: arena->levels[level-1].bricks[i]->hitcount = -1; break;
    }

    for(int j = 0; j < MAXBRICKPARTICLES; j++)
    {
      brick->particles[j].alpha = (rand() % 55) + 201;
      brick->particles[j].gdiff = 0;
      brick->particles[j].x = rand() % BRICKW;
      brick->particles[j].y = rand() % BRICKH;
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
}

void arena_drawbricks(Arena* arena, SDL_Renderer* renderer)
{
  for(int brickno = 0; brickno < arena->brickcount; brickno++)
  {
    Brick* brick = arena->bricks[brickno];
    Animation* anim = brick->sprite->anim;
    if(brick->hitcount != 0)
    {
      a_drawsprite(brick->sprite, renderer, brick->left, brick->top);
    }
    else
    {
      for(int j = 0; j < config_getbrickparticles(); j++)
      {
        if(brick->particles[j].alpha > 0)
        {
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
    }
  }
}

void arena_resetbricks(Arena* arena)
{
  for(int brickno = 0; brickno < arena->brickcount; brickno++)
  {
    switch(arena->bricks[brickno]->type)
    {
      case btNormal: arena->bricks[brickno]->hitcount = 1; break;
      case btHard: arena->bricks[brickno]->hitcount = 2; break;
      case btIndestructible: arena->bricks[brickno]->hitcount = -1; break;
      case btWormhole: arena->bricks[brickno]->hitcount = -1; break;
    }
  }
}

void arena_freelevels(Arena* arena)
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
  }
  free(arena->levels);
}

Bonus* arena_addbonus(Arena* arena, int x, int y, Bonustype type)
{
  // There was a memory leak reported by valgrind here...
  // but actually the leak is because I hadn't yet freed
  // arena->bonuses (et al) before the program exits
  arena->bonuses = realloc(arena->bonuses, sizeof(Bonus*) * ++arena->bonuscount);
  arena->bonuses[arena->bonuscount - 1] = malloc(sizeof(Bonus));
  Bonus* bonus = arena->bonuses[arena->bonuscount - 1];
  bonus->sprite = malloc(sizeof(Sprite));
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
    case boSlow: break;
  }

  bonus->sprite->loop = 1;
  bonus->sprite->state = asLooping;
  bonus->x = x;
  bonus->y = y;
  bonus->w = 43;
  bonus->h = 25;
  bonus->type = type;

  arena->bonuses[arena->bonuscount-1] = bonus;

  return bonus;
}

Bonus* arena_batcollidesbonus(Arena* arena, Bat* player, Ball* ball)
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
            af_setanimation(arena->factory, &(player->sprite),"bat-shrink", 0, bat_aftershrink, (void*)arena, (void*)player);
          else
            bat_aftershrink((void*)arena, (void*)player);
          ball->state = bsNormal;
          player->state = plShort;
        break;
        case boDeadly:
          ball->state = bsDeadly;
          af_setanimation(arena->factory, &(ball->sprite), "ball-deadly", 1, NULL, NULL, NULL);
        break;
        case boCatch: ball->state = bsLoose; break;
        case boPlayer:
          arena->lives++;
          af_playsample(arena->factory, "1up");
        break;
        case boGrow:
          // If the bat size is currently short, don't play the grow animation
          if(player->w != psLong)
            af_setanimation(arena->factory, &(player->sprite),"bat-grow", 0, bat_aftergrow, (void*)arena, (void*)player);
          else
            bat_aftergrow((void*)arena, (void*)player);
          ball->state = bsNormal;
          player->state = plLong;
        break;
        case boLaser:
          af_setanimation(arena->factory, &(player->sprite),"bat-laserify", 0, bat_afterlaser, (void*)arena, (void*)player);
          player->sprite.state = asPlayToEnd;
          player->state = plLaser;
        break;
        case boWarp:
          player->warpenabled = true;
        break;
        case boSlow: break;
      }

      if(player->state != plShort && player->state != plLong)
        player->w = psNormal;

      bonus_freebonus(&arena->bonuses, &arena->bonuscount, arena->bonuses[i]);
    }
  }
  return NULL;
}

void bat_aftershrink(void* sender, void* data)
{
  Bat* player = (Bat*)data;
  Arena* arena = (Arena*)sender;
  af_setanimation(arena->factory, &(player->sprite),"bat-s", 1, NULL, NULL, NULL);
  player->sprite.state = asLooping;
  player->w = psShort;
}

void bat_aftergrow(void* sender, void* data)
{
  Bat* player = (Bat*)data;
  Arena* arena = (Arena*)sender;
  af_setanimation(arena->factory, &(player->sprite),"bat-l", 1, NULL, NULL, NULL);
  player->sprite.state = asLooping;
  player->w = psLong;
}

void bat_afterlaser(void* sender, void* data)
{
  Bat* player = (Bat*)data;
  Arena* arena = (Arena*)sender;
  af_setanimation(arena->factory, &(player->sprite), "bat-laser", 1, NULL, NULL, NULL);
  player->sprite.state = asLooping;
}

int ball_moveball(Ball* ball, Arena* arena, Bat* player)
{
  // Using the speed as the hypotenuse of the triangle,
  // use trig to work out the next X and Y coordinates.

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
    for(int spd = 1; spd <= ball->speed; spd++)
    {
      int nextx = spd * sinl(rads);
      int nexty = spd * cosl(rads);

      if((nextx == 0) && (nexty == 0))
      {
        lastx = ball->cx;
        lasty = ball->cy;
        continue;
      }

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

      b = ball_collidesbricks(ball, arena->bricks, arena->brickcount, &hitedge);

      // We've hit a brick. Ball will be positioned
      // on the brick edge
      if(b!=NULL)
      {
        if(b->type == btWormhole)
        {
          // If we hit a wormhole, check the collision again to
          // ensure it's in the middle section, not an edge, but
          // if it is an edge we don't yet treat that as no collision.

          Bounds wbound = { .left = b->left + 9, .width = 25, .height = 25, .top = b->top };

          if(ball_collidesbounds(ball, &wbound, &hitedge))
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
                  ball->cx = arena->bricks[j]->left + 20;
                  ball->cy = arena->bricks[j]->top + 12;
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
        else
        {

          arena->score += BRICKSCORE;

          if((b->type != btIndestructible) && (b->type != btWormhole) && (b->hitcount == 0))
            arena->remaining--;

          b->sprite->state = asPlayAndReset;

          // Chance of bonus no more frequently than every BONUSFREQUENCY bricks
          // and no more than two bonuses on screen at once
          if((arena->bonuscounter % BONUSFREQUENCY == 0) && (b->type == btNormal) && (arena->bonuscount < 2))
          {
            int bonusscore = rand() % 100;
            Bonustype botype;

            if(bonusscore > 90)
              botype = boGrow;
            else if (bonusscore > 85)
              botype = boDeadly;
            else if (bonusscore > 82)
              botype = boPlayer;
            else if (bonusscore > 70)
              botype = boCatch;
            else if (bonusscore > 63)
              botype = boLaser;
            else if (bonusscore > 63)
              botype = boShrink;
            else if (bonusscore > 60)
              botype = boWarp;

            if(bonusscore > 60)
              arena_addbonus(arena, b->left, b->bottom, botype);

            af_playsample(arena->factory, "brick");
          }
          else if(b->type == btNormal)
          {
            af_playsample(arena->factory, "brick");
          }
          else
          {
            af_playsample(arena->factory, "brick-high");
          }

          if((b->type == btNormal) && (ball->state == bsDeadly))
            hitedge = eNone;

          // Because we break here, lastx/y are not updated
          // so retain the last non-collision position

          break;
        }
      }

      if(b==NULL) // b == null
      {
        // No contact with any brick so we can say we're clear of the warp destination
        ball->warpdest = NULL;
      }

      if(1 == ball_collidesbat(ball, player, &hitedge))
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
    }

    if(b!=NULL)
    {
      ball->cx = lastx;
      ball->cy = lasty;
    }

    if(hitedge != eNone)
      arena->bonuscounter++;

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

int ball_collidesbat(Ball* ball, Bat* player, Edge* e)
{
  if ((ball->cy - (player->y + player->h) < ball->radius) &&
      (player->x - ball->cx < ball->radius) &&
      (player->y - ball->cy < ball->radius) &&
      (ball->cx - (player->x + (int)(player->w)) < ball->radius))
  {
    *e = eTop;
    ball->cy = player->y - ball->radius - 1;

    // Barkanoid control method, allow spin
    if(config_getcontrolmethod() == cmBarkanoid)
    {
      if((abs(player->speed) >= 3) && ((ball->bearing > 90) && (ball->bearing < 270)))
      {
          // we need a right-angled triangle
          double rads = (180 - ball->bearing) * (PI / 180);
          double nextx = ball->speed * sinl(rads);
          double nexty = ball->speed * cosl(rads);

          double newx = (int)(nextx + player->speed);

          //now we need a new "hypotenuse"
          double nspd = sqrt((newx*newx) + (nexty*nexty));

          rads = asinl((double)(newx/nspd));
          double bearing = rads / (PI / 180);

          if((ball->bearing + bearing > 110) && (ball->bearing + bearing < 250))
            ball->bearing += bearing;
      }

    }
    else // Control method = Classic, no spin, but angle based on bat segment hit
    {
      int e1 = player->w / 10;
      int q1 = player->w / 5;
      int q2 = player->w / 2;
      int q3 = player->w - q1;
      int q4 = player->w - e1;

      if(ball->cx < player->x + e1)
      {
        ball->bearing = 250;
      }
      else if(ball->cx < player->x + q1)
      {
        ball->bearing = 240;
      }
      else if(ball->cx < player->x + q2)
      {
        ball->bearing = 210;
      }
      else if(ball->cx < player->x + q3)
      {
        ball->bearing = 150;
      }
      else if(ball->cx < player->x + q4)
      {
        ball->bearing = 120;
      }
      else // right edge
      {
        ball->bearing = 110;
      }
    }

    return 1;
  }
  return 0;
}

void arena_addbullet(Arena* arena, Bat* player)
{
  if(arena->bulletcount >= MAXBULLETS)
    return;

  arena->bulletcount++;
  arena->bullets = realloc(arena->bullets, sizeof(Bullet*) * arena->bulletcount);
  arena->bullets[arena->bulletcount - 1] = malloc(sizeof(Bullet));
  arena->bullets[arena->bulletcount - 1]->speed = 5;
  arena->bullets[arena->bulletcount - 1]->x = player->x + 22;
  arena->bullets[arena->bulletcount - 1]->y = player->y;

  arena->bulletcount++;
  arena->bullets = realloc(arena->bullets, sizeof(Bullet*) * arena->bulletcount);
  arena->bullets[arena->bulletcount - 1] = malloc(sizeof(Bullet));
  arena->bullets[arena->bulletcount - 1]->speed = 5;
  arena->bullets[arena->bulletcount - 1]->x = player->x + player->w - 22 - 5; // 5 for bullet width
  arena->bullets[arena->bulletcount - 1]->y = player->y;
}

void arena_movebullets(Arena* arena)
{
  for(int i = 0; i < arena->bulletcount; i++)
  {
    arena->bullets[i]->y -= arena->bullets[i]->speed;
  }
}

void arena_drawbullets(Arena* arena, SDL_Renderer* renderer)
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

void arena_freebullet(Arena* arena, int index)
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

void arena_freebullets(Arena* arena)
{
  for(int i = arena->bulletcount-1; i >= 0; i--)
  {
    arena_freebullet(arena, i);
  }
}

void arena_checkbulletcollisions(Arena* arena)
{
  for(int i = arena->bulletcount - 1; i >= 0; i--)
  {
    Bullet* bullet = arena->bullets[i];
    if(bullet->y < arena->bounds.top)
    {
      arena_freebullet(arena, i);
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
    if((b->hitcount <= 0) && (b->type != btIndestructible))
      continue;

    for(int i = arena->bulletcount - 1; i >= 0; i--)
    {
      Bullet* bullet = arena->bullets[i];
      if((bullet->y < b->bottom) &&
         (bullet->x + 5 > b->left) &&
         (bullet->x < b->right))
      {
          arena_freebullet(arena, i);
          hit = true;
          continue; // test next bullet
      }
    }

    if((hit) && (b->type != btIndestructible)){
      b->hitcount--;
      if(b->hitcount == 0)
      {
        arena->score += BRICKSCORE;
        arena->remaining--;
      }
      af_playsample(arena->factory, "brick-laser");
    }
  }
}

void arena_drawlives(Arena* arena, App* app)
{
  for(int i = 0; i < arena->lives; i++)
    a_drawstaticframe(af_getanimation(arena->factory, "life"), app->renderer, 40+(40*i), 560, 0);
}
