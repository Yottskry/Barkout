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

void cat_move(Cat* cat, Brick** bricks, int brickcount, Bounds* bounds)
{
  //int newx = cat->bounds.left;
  //int newy = cat->bounds.top;

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
    Bounds b1 = { .left = brick->left, .width = brick->right - brick->left, .top = brick->top, .height = brick->bottom - brick->top };
    // We have collided with a brick
    if(bounds_intersects(&b, &b1))
    {
      // Set the position to the edge of the brick
      switch(cat->nextdirection)
      {
        case dDown:
          cat->bounds.top = b1.top - 1 - cath;
        break;
        case dUp:
          cat->bounds.top = b1.top + b1.height + 1;
        break;
        case dLeft:
          cat->bounds.left = b1.left + b1.width + 1;
        break;
        case dRight:
          cat->bounds.left = b1.left - 1 - catw;
        break;
      }
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

void cat_draw(Cat* cat, SDL_Renderer* renderer)
{
  a_drawsprite(&cat->sprite, renderer, cat->bounds.left, cat->bounds.top);
}
