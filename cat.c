#include "cat.h"

void cat_move(Cat* cat, Brick** bricks, int brickcount, Bounds* bounds)
{
  //int newx = cat->bounds.left;
  //int newy = cat->bounds.top;

  Bounds b = { .left = cat->bounds.left, .right = cat->bounds.right, .top = cat->bounds.top, .bottom = cat->bounds.bottom };

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
    Bounds b1 = { .left = brick->left, .right = brick->right, .top = brick->top, .bottom = brick->bottom };
    // We have collided with a brick
    if(bounds_intersects(&b, &b1))
    {
      switch(cat->nextdirection)
      {
        case dDown:

        break;
        case dUp:

        break;
        case dLeft:

        break;
        case dRight:

        break;
      }
      intersects = true;
      break; // Out of loop, set next direction
    }
  }

  if(!intersects)
  {
    cat->bounds.left = b.left;
    cat->bounds.right = b.right;
    cat->bounds.top = b.top;
    cat->bounds.bottom = b.bottom;

    // Pick the next direction.
    int newdir = rand() % 30;

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
        cat->nextdirection = newdir == 8 ? dUp : newdir == 9 ? dDown : dLeft;
      break;
      case dRight:
        cat->nextdirection = newdir == 8 ? dUp : newdir == 9 ? dDown : dRight;
      break;
    }
  }
}

void cat_draw(Cat* cat, SDL_Renderer* renderer)
{
  a_drawsprite(&cat->sprite, renderer, cat->bounds.left, cat->bounds.top);
}
