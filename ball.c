#include "ball.h"
#include "config.h"

// Brought this out into its own function so we can use it
// to test collisions with baddies too, and thus also
// change the ball's angle based on the collision
bool ball_collidesbounds(Ball* ball, Bounds* bounds, Edge* e)
{
  if((ball->cy - (bounds->top + bounds->height) <= ball->radius) &&
       (bounds->left - ball->cx <= ball->radius) &&
       (bounds->top - ball->cy <= ball->radius) &&
       (ball->cx - (bounds->left + bounds->width) <= ball->radius))
  {
    // Determine distance to each each of the brick so we
    // can work out which the ball should reflect off
    int deltar = abs(ball->cx - ball->radius - (bounds->left + bounds->width));
    int deltal = abs(ball->cx + ball->radius - bounds->left);
    int deltat = abs(ball->cy + ball->radius - bounds->top);
    int deltab = abs(ball->cy - ball->radius - (bounds->top + bounds->height));

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

    return true;
  }
  return false;
}


Brick* ball_collidesbricks(Ball* ball, Brick** bricks, int brickcount, Edge* e)
{
  for(int brickno = brickcount-1; brickno >= 0; brickno--)
  {
    if(bricks[brickno]->hitcount == 0)
      continue;

    Brick* brick = bricks[brickno];

    Bounds bounds = {
      .left = brick->left,
      .width = brick->right - brick->left,
      .top = brick->top,
      .height = brick->bottom - brick->top
    };

    if(ball_collidesbounds(ball, &bounds, e))
    {
      brick->hitcount--;
      return brick;
    }
  }

  return NULL;
}

void ball_ricochet(Ball* ball, Edge hitedge)
{
  switch(hitedge)
  {
    case eLeft:
      ball->bearing = 360 - ball->bearing;
    break;
    case eRight:
      ball->bearing = 360 - ball->bearing;
    break;
    case eTop:
      if(ball->bearing < 180)
        ball->bearing = 180 - ball->bearing;
      else
        ball->bearing = 360 - (ball->bearing - 180);
    break;
    case eBottom:
      if(ball->bearing < 90)
        ball->bearing = 180 - ball->bearing;
      else
        ball->bearing = 180 + (360 - ball->bearing);
    break;
    case eTopLeft:
      ball->bearing += 180;
    break;
    case eTopRight:
      ball->bearing -= 180;
    break;
    case eBottomLeft:
      ball->bearing = ball->bearing + 180;
    break;
    case eBottomRight:
      ball->bearing = ball->bearing - 180;
    break;
    case eNone: break;
  }
}

void ball_drawball(Ball* ball, SDL_Renderer* renderer)
{
  // Draw the sparkles...
  if((ball->state == bsDeadly) || (ball->state == bsStuck) || (ball->state == bsLoose))
  {
    for(int i = 0; i < config_gettrailparticles(); i++)
    {
      if(ball->sparkles[i].alpha == 0)
      {
        ball->sparkles[i].alpha = (rand() % 150) + 150;
        // Set new position
        ball->sparkles[i].x = ball->cx + (rand() % 18) - 9;
        ball->sparkles[i].y = ball->cy + (rand() % 18) - 9;
        // The amount to reduce the green component by (for Deadly)
        // The amount to reduce the red and blue components by (for Sticky)
        // This is doubled for the red component to give a shade of blue.
        ball->sparkles[i].gdiff = rand() % 100;
      }

      Uint16 a = ball->sparkles[i].alpha;

      if(a > 255)
        a = 255 - (a - 255);

      if(ball->state == bsDeadly)
        SDL_SetRenderDrawColor(renderer, 255 - (ball->sparkles[i].gdiff * 2),255 - (ball->sparkles[i].gdiff),255,(Uint8)a);
      else
        SDL_SetRenderDrawColor(renderer, 255 - (ball->sparkles[i].gdiff * 2),255, 255 - (ball->sparkles[i].gdiff),(Uint8)a);
      SDL_RenderFillRect(renderer, &(SDL_Rect){.x = ball->sparkles[i].x, .y = ball->sparkles[i].y, .w = 2, .h = 2});
      if(ball->sparkles[i].alpha < 12)
        ball->sparkles[i].alpha = 0;
      else
        ball->sparkles[i].alpha -= 12;

    }
  }
  a_drawsprite(&ball->sprite, renderer, ball->cx - ball->radius, ball->cy - ball->radius);
}
