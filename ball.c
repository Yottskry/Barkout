#include "ball.h"
#include <math.h>

int ball_moveball(Ball* ball, Arena* arena, Bat* player)
{
  // Using the speed as the hypotenuse of the triangle,
  // use trig to work out the next X and Y coordinates.

  if (ball->state == bsNormal || ball->state == bsLoose)
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
        b->sprite->state = asMoving;
        // Because we break here, lastx/y are not updated
        // so retain the last non-collision position
        break;
      }

      if(1 == ball_collidesbat(ball, player, &hitedge))
        break;

      if(ball->cy - ball->radius < arena->top)
      {
        ball->cy = arena->top + ball->radius;
        hitedge = eBottom;
      }

      if(ball->cy + ball->radius > arena->bottom)
      {
        ball->cy = arena->bottom - ball->radius;
        hitedge = eTop;
      }

      if(ball->cx + ball->radius > arena->right)
      {
        ball->cx = arena->right - ball->radius;
        hitedge = eLeft;
      }

      if(ball->cx - ball->radius < arena->left)
      {

        ball->cx = arena->left + ball->radius;
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
      case eTopLeft: break;
      case eTopRight: break;
      case eBottomLeft:
        ball->bearing = ball->bearing + 180;
      break;
      case eBottomRight:
        ball->bearing = ball->bearing - 180;
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
    return 1;
  }
  return 0;
}
