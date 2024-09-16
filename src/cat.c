#include "cat.h"

void cat_init(Cat* cat, ResourceFactory* f)
{
  cat->bounds.left = 450;
  cat->bounds.width = 40;
  cat->bounds.top = 50;
  cat->bounds.height = 40;
  cat->speed = 1;
  cat->nextdirection = dDown;
  cat->sprite.anim = af_getanimation(f, "cat");
  cat->sprite.currentframe = 0;
  cat->sprite.lastticks = 0;
  cat->sprite.loop = 1;
  cat->sprite.state = asLooping;
  cat->state = csDead;
}

void cat_move(Cat** cats, int count, Brick** bricks, int brickcount, Bounds* bounds)
{
  for(int catno = 0; catno < count; catno++)
  {
    Cat* cat = cats[catno];

    if(cat->state != csAlive)
      continue;

    int cath = cat->bounds.height;
    int catw = cat->bounds.width;

    Bounds b = { .left = cat->bounds.left, .width = cat->bounds.width, .top = cat->bounds.top, .height = cat->bounds.height };

    switch(cat->nextdirection)
    {
      case dDown:
        b.bottom += cat->speed;
        b.top += cat->speed;
      break;
      case dUp:
        b.top -= cat->speed;
        b.bottom -= cat->speed;
      break;
      case dLeft:
        b.left -= cat->speed;
        b.right -= cat->speed;
      break;
      case dRight:
        b.right += cat->speed;
        b.left += cat->speed;
      break;
    }

    bool intersects = false;

    for(int i = 0; i < brickcount; i++)
    {
      Brick* brick = bricks[i];
      // Can't collide with a destroyed brick
      if(brick->hitcount == 0)
        continue;

      if(brick->counter > 0)
        continue;

      Bounds b1 = { .left = brick->left, .width = brick->right - brick->left, .top = brick->top, .height = brick->bottom - brick->top };
      // We have collided with a brick
      if(bounds_intersects(&b, &b1))
      {
        intersects = true;
        break; // Out of loop, set next direction
      }
    }

    // Test the arena edges
    if(b.left <= bounds->left)
    {
      cat->bounds.left = bounds->left + 1;
      //cat->bounds.right = cat->bounds.left + catw;
      intersects = true;
    }
    else if(b.left + b.width >= bounds->right)
    {
      //cat->bounds.right = bounds->right - 1;
      cat->bounds.left = bounds->right - catw - 1;
      intersects = true;
    }
    else if(b.top + b.height >= bounds->bottom)
    {
      //cat->bounds.bottom = bounds->bottom - 1;
      cat->bounds.top = bounds->bottom - cath - 1;
      intersects = true;
    }
    else if(b.top <= bounds->top)
    {
      cat->bounds.top = bounds->top + 1;
      //cat->bounds.bottom = cat->bounds.top + cath;
      intersects = true;
    }

    if(!intersects)
    {
      cat->bounds.left = b.left;
      //cat->bounds.right = b.right;
      cat->bounds.top = b.top;
      //cat->bounds.bottom = b.bottom;

      // Pick the next direction.
      int newdir = rand() % 25;

      // 8 / 10 chance we keep the same direction
      switch(cat->nextdirection)
      {
        case dDown:
          cat->nextdirection = newdir == 8 ? dLeft : newdir == 9 ? dRight : dDown;
        break;
        case dUp:
          cat->nextdirection = newdir == 8 ? dLeft : newdir == 9 ? dRight : dUp;
        break;
        case dLeft:
          cat->nextdirection = newdir < 23 ? dLeft : dDown;
        break;
        case dRight:
          cat->nextdirection = newdir < 23 ? dRight : dDown;
        break;
      }
    }
    else
    {
      int newdir = rand() % 2;

      // 8 / 10 chance we keep the same direction
      switch(cat->nextdirection)
      {
        case dDown:
          cat->nextdirection = newdir % 2 == 0 ? dLeft : dRight;
        break;
        case dUp:
          cat->nextdirection = newdir % 2 == 0 ? dLeft : dRight;
        break;
        case dLeft:
          cat->nextdirection = newdir % 2 == 0 ? dUp : dDown;
        break;
        case dRight:
          cat->nextdirection = newdir % 2 == 0 ? dUp : dDown;
        break;
      }
    }
  }
}

void cat_draw(Cat** cats, int count, SDL_Renderer* renderer)
{
  for(int catno = 0; catno < count; catno++)
  {
    Cat* cat = cats[catno];
    if(cat->state != csDead)
      a_drawsprite(&cat->sprite, renderer, cat->bounds.left, cat->bounds.top);
  }
}

void cat_spawn(Cat** cats, int count, ResourceFactory* factory)
{
  if(count==0)
    return;

  static Uint32 baddiecounter;
  if(baddiecounter == 0)
    baddiecounter = SDL_GetTicks();
  Uint32 currentcounter = SDL_GetTicks();

  int alivecount = 0;
  for(int i = 0; i < count; i++)
  {
    if(cats[i]->state != csDead)
      alivecount++;
  }

  if((alivecount == 0) && ((currentcounter - baddiecounter) > FIRSTBADDIE))
  {
    // set the position first?
    cats[0]->state = csSpawning;
    cats[0]->bounds.left = cats[0]->spawnx;
    cats[0]->bounds.top = cats[0]->spawny;
    af_setanimation(factory, &(cats[0]->sprite), "cat-spawn", 0, cat_afterspawn, (void*)(cats[0]), (void*)factory);
    baddiecounter = currentcounter;
  }
  else if((alivecount < count) && ((currentcounter - baddiecounter) > NEXTBADDIE))
  {
    for(int i = 0; i < count; i++)
    {
      if(cats[i]->state == csDead)
      {
        cats[i]->state = csSpawning;
        cats[i]->bounds.left = cats[i]->spawnx;
        cats[i]->bounds.top = cats[i]->spawny;
        af_setanimation(factory, &(cats[i]->sprite), "cat-spawn", 0, cat_afterspawn, (void*)(cats[i]), (void*)factory);
        baddiecounter = currentcounter;
        break;
      }
    }
  }
}

bool cat_collidesball(Cat** cats, int count, Ball* ball, ResourceFactory* factory)
{
  Edge edge = eNone;
  for(int i = 0; i < count; i++)
  {
    if(cats[i]->state == csAlive)
    {
      int d = 0;
      if(ball_collidesBounds(ball, &cats[i]->bounds, &edge, &d))
      {
        cats[i]->state = csDying;
        ball_ricochet(ball, edge);
        af_playsample(factory, "cat-hit");
        af_setanimation(factory, &(cats[i]->sprite), "cat-die", 0, cat_afterdie, (void*)(cats[i]), (void*)factory);
        return true;
      }
    }
  }
  return false;
}

bool cat_collidesbat(Cat** cats, int count, Bounds* bounds, ResourceFactory* factory)
{
  for(int i = 0; i < count; i++)
  {
    if(cats[i]->state == csAlive)
    {
      if(bounds_intersects(&cats[i]->bounds, bounds))
      {
        cats[i]->state = csDying;
        af_playsample(factory, "cat-hit");
        af_setanimation(factory, &(cats[i]->sprite), "cat-die", 0, cat_afterdie, (void*)(cats[i]), (void*)factory);
        return true;
      }
    }
  }
  return false;
}

void cat_afterdie(void* sender, void* data)
{
  ResourceFactory* factory = (ResourceFactory*)data;
  Cat* cat = (Cat*)sender;
  af_setanimation(factory, &cat->sprite, "cat", 1, NULL, NULL, NULL);
  cat->state = csDead;
}

void cat_afterspawn(void* sender, void* data)
{
  ResourceFactory* factory = (ResourceFactory*)data;
  Cat* cat = (Cat*)sender;
  af_setanimation(factory, &cat->sprite, "cat", 1, NULL, NULL, NULL);
  cat->sprite.state = asLooping;
  cat->state = csAlive;
}
