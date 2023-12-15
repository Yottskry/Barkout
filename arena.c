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
    for(unsigned int col = 0; col < strlen(rowdata); col++)
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

      //af_setanimation(arena->bricks[brickno]->sprite, brickanim, 0, NULL, NULL);

      arena->bricks[brickno]->sprite->anim = brickanim;
      arena->bricks[brickno]->sprite->state = asStatic;
      arena->bricks[brickno]->sprite->currentframe = 0;
      arena->bricks[brickno]->sprite->loop = 0;
      arena->bricks[brickno]->sprite->lastticks = 0;
      arena->bricks[brickno]->sprite->onanimfinished = NULL;
      arena->bricks[brickno]->sprite->data = NULL;
      brickno++;
    }
    row++;
  }

  fclose(f);
  return 0;
}

void arena_drawbricks(Arena* arena, SDL_Renderer* renderer)
{
  for(unsigned int brickno = 0; brickno < arena->brickcount; brickno++)
  {
    if(arena->bricks[brickno]->hitcount != 0)
    {
      a_drawsprite(arena->bricks[brickno]->sprite, renderer, arena->bricks[brickno]->left, arena->bricks[brickno]->top);
    }
  }
}

void arena_resetbricks(Arena* arena)
{
  for(unsigned int brickno = 0; brickno < arena->brickcount; brickno++)
  {
    switch(arena->bricks[brickno]->type)
    {
      case btNormal: arena->bricks[brickno]->hitcount = 1; break;
      case btHard: arena->bricks[brickno]->hitcount = 2; break;
      case btIndestructible: arena->bricks[brickno]->hitcount = -1; break;
    }
  }
}

void arena_freebricks(Arena* arena)
{
  for(unsigned int brickno = 0; brickno < arena->brickcount; brickno++)
  {
    free(arena->bricks[brickno]->sprite);
    free(arena->bricks[brickno]);
  }

  free(arena->bricks);
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
    case boSlow: bonus->sprite->anim = af_getanimation(arena->factory, "bonus-e"); break;
    case boShrink: bonus->sprite->anim = af_getanimation(arena->factory, "bonus-s"); break;
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

int arena_movebonuses(Arena* arena, Bat* player)
{
  for(unsigned int i = 0; i < arena->bonuscount; i++)
  {
    Bonus* bonus = arena->bonuses[i];
    bonus->y += 1;
    if(bonus->y > arena->bounds.bottom)
    {
      arena_freebonus(arena, bonus);
    }
  }
  return 0;
}

int arena_freebonus(Arena* arena, Bonus* bonus)
{
  for(unsigned int i = 0; i < arena->bonuscount; i++)
  {
    // Find the item to be removed
    if(arena->bonuses[i] == bonus)
    {
      // sprite-anim is managed by the resourcefactory,
      // so we don't free it here.
      free(bonus->sprite);
      free(bonus);

      // Move all subsequent items up one
      for(int j = i; j < arena->bonuscount - 1; j++)
        arena->bonuses[j] = arena->bonuses[j+1];
    }

    arena->bonuscount--;
    arena->bonuses = realloc(arena->bonuses, sizeof(Bonus*) * arena->bonuscount);
  }
  return 0;
}

int arena_freebonuses(Arena* arena)
{
  for(unsigned int i = arena->bonuscount - 1; i >= 0; i--)
  {
    free(arena->bonuses[i]->sprite);
    free(arena->bonuses[i]);
  }
  free(arena->bonuses);
  return 0;
}

int arena_drawbonuses(Arena* arena, SDL_Renderer* renderer)
{
  for(unsigned int i = 0; i < arena->bonuscount; i++)
  {
    a_drawsprite(arena->bonuses[i]->sprite, renderer, arena->bonuses[i]->x, arena->bonuses[i]->y);
  }
  return 0;
}

Bonus* arena_batcollidesbonus(Arena* arena, Bat* player, Ball* ball)
{
  for(unsigned int i = 0; i < arena->bonuscount; i++)
  {
    int bx = arena->bonuses[i]->x + arena->bonuses[i]->w;
    int by = arena->bonuses[i]->y + arena->bonuses[i]->h;
    int br = (int)(arena->bonuses[i]->w / 2);


    // FOr now, we always catch the bonus
    if((by > player->y) &&
        (bx+br > player->x) &&
        (bx-br < player->x + player->w))
    {
      switch(arena->bonuses[i]->type)
      {
        case boShrink:
          if(player->state != plShort)
            af_setanimation(arena->factory, &(player->sprite),"bat-shrink", 0, bat_aftershrink, (void*)arena, (void*)player);
          // Ball should revert to normal state and animation
          ball->state = bsNormal;
          af_setanimation(arena->factory, &(ball->sprite), "ball", 1, NULL, NULL, NULL);
          player->state = plShort;
        break;
        case boDeadly:
          ball->state = bsDeadly;
          af_setanimation(arena->factory, &(ball->sprite), "ball-deadly", 1, NULL, NULL, NULL);
          // Bat should revert to normal state
          af_setanimation(arena->factory, &(player->sprite), "bat", 1, NULL, NULL, NULL);
        break;
      }
      arena_freebonus(arena, arena->bonuses[i]);
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
  printf("Player width is...%d x \n", player->w);
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

      b = ball_collidesbricks(arena, ball, &hitedge);
      // We've hit a brick. Ball will be positioned
      // on the brick edge
      if(b!=NULL)
      {
        arena->score += 100;

        b->sprite->state = asPlayAndReset;
        if((arena->bonuscounter % BONUSFREQUENCY == 0) && (b->type == btNormal) && (arena->bonuscount < 2))
        {
          Bonustype botype = rand() % 3;
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

      if(1 == ball_collidesbat(ball, player, &hitedge))
      {
        af_playsample(arena->factory, "bat");
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

    switch(hitedge)
    {
      case eLeft:
        ball->bearing = 360 - ball->bearing;
        arena->bonuscounter++;
      break;
      case eRight:
        ball->bearing = 360 - ball->bearing;
        arena->bonuscounter++;
      break;
      case eTop:
        if(ball->bearing < 180)
          ball->bearing = 180 - ball->bearing;
        else
          ball->bearing = 360 - (ball->bearing - 180);
        arena->bonuscounter += 2;
      break;
      case eBottom:
        if(ball->bearing < 90)
          ball->bearing = 180 - ball->bearing;
        else
          ball->bearing = 180 + (360 - ball->bearing);
        arena->bonuscounter += 2;
      break;
      case eTopLeft:
        ball->bearing += 180;
        arena->bonuscounter += 3;
      break;
      case eTopRight:
        ball->bearing -= 180;
        arena->bonuscounter += 3;
      break;
      case eBottomLeft:
        ball->bearing = ball->bearing + 180;
        arena->bonuscounter += 3;
      break;
      case eBottomRight:
        ball->bearing = ball->bearing - 180;
        arena->bonuscounter += 3;
      break;
    }

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

Brick* ball_collidesbricks(Arena* arena, Ball* ball, Edge* e)
{
  for(int brickno = arena->brickcount-1; brickno >= 0; brickno--)
  {
    if(arena->bricks[brickno]->hitcount == 0)
      continue;

    Brick* brick = arena->bricks[brickno];

    if((ball->cy - brick->bottom <= ball->radius) &&
       (brick->left - ball->cx <= ball->radius) &&
       (brick->top - ball->cy <= ball->radius) &&
       (ball->cx - brick->right <= ball->radius))
    {
      // Determin distance to each each of the brick so we
      // can work out which the ball should reflect off
      int deltar = abs(ball->cx - ball->radius - brick->right);
      int deltal = abs(ball->cx + ball->radius - brick->left);
      int deltat = abs(ball->cy + ball->radius - brick->top);
      int deltab = abs(ball->cy - ball->radius - brick->bottom);


      brick->hitcount--;


      if((ball->bearing < 90) || (ball->bearing > 270))
      {
        // Ball is travelling vaguely up
        // Check bottom side of brick
        if(ball->bearing < 90)
        {
          // If one is closer than the other, pick that edge, if both are equal pick the corner
          //*e = (deltab < deltal) ? eBottom : (deltab > deltal) ? eLeft : eBottomLeft;
          *e = (deltab < deltal) ? eBottom : eLeft;
        }
        else
        {
          // Ball is travelling up and left
          // Check right edge
          //*e = (deltab < deltar) ? eBottom : (deltab > deltar) ? eRight : eBottomRight;
          *e = (deltab < deltar) ? eBottom : eRight;
        }
      }
      else
      {
        // Ball is travelling vaguely down
        // Check top side of brick

        if(ball->bearing < 180)
        {
          // Ball is travelling down and right
          // Check left edge
          *e = (deltat < deltal) ? eTop : eLeft;
        }
        else
        {
          // Ball is travelling down and left
          // Check right edge
          *e = (deltat < deltar) ? eTop : eRight;
        }
      }

      return brick;
    }
  }

  return NULL;
}

int ball_collidesbat(Ball* ball, Bat* player, Edge* e)
{
  if((ball->cy - (player->y + player->h) < ball->radius) &&
       (player->x - ball->cx < ball->radius) &&
       (player->y - ball->cy < ball->radius) &&
       (ball->cx - (player->x + player->w) < ball->radius))
  {
    *e = eTop;
    ball->cy = player->y - ball->radius - 1;
    //if(ball->cx + 2 < player->x && ball->bearing < 180)
    //  *e = eTopLeft;
    //else if((ball->cx - 2 > (player->x + player->w)) && (ball->bearing > 180))
    //  *e = eTopRight;
    //else
    if((abs(player->speed) > 3) && ((ball->bearing > 90) && (ball->bearing < 270)))
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

        // Don't allow entirely horizontal travel
        //if(ball->bearing <= 100)
        //  ball->bearing = 120;
        //else if(ball->bearing >= 260)
        //  ball->bearing = 240;
    }

    return 1;
  }
  return 0;
}
