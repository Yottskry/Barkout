#include "ball.h"
#include <math.h>

int ball_moveball(Ball* ball, Arena* arena, Bat* player)
{
  // Using the speed as the hypotenuse of the triangle,
  // use trig to work out the next X and Y coordinates.

  ball->x = ball->cx - ball->radius;
  ball->y = ball->cy - ball->radius;

  // Convert degrees to radians
  double rads = ball->bearing * (PI / 180);
  Edge hitedge = eNone;
  Brick* b = NULL;

  int ballx = ball->x;
  int bally = ball->y;

  // Check all the points on the path along which the
  // ball will move in this iteration.
  for(int spd = 1; spd <= ball->speed; spd++)
  {
    int nextx = spd * sinl(rads);
    int nexty = spd * cosl(rads);

    if(ball->bearing < 180)
    {
      ball->y = bally - nexty;
      ball->x = ballx + nextx;
    }
    else if (ball->bearing < 270)
    {
      ball->y = bally - nexty;
      ball->x = ballx + nextx;
    }
    else
    {
      ball->y = bally - nexty;
      ball->x = ballx + nextx;
    }

    b = ball_collidesbricks(arena, ball, &hitedge);
    // We've hit a brick. Ball will be positioned
    // on the brick edge
    if(b!=NULL)
      break;

    if(ball->y < arena->top)
    {
      ball->y = arena->top;
      hitedge = eBottom;
    }

    if(ball->y + 15 > arena->bottom)
    {
      ball->y = arena->bottom - 15;
      hitedge = eTop;
    }

    if(ball->x + 15 > arena->right)
    {
      ball->x = arena->right - 15;
      hitedge = eLeft;
    }

    if(ball->x < arena->left)
    {

      ball->x = arena->left;
      hitedge = eRight;
    }
  }



  switch(hitedge)
  {
    case eLeft:
      ball->bearing = 360 - ball->bearing;
      ball->x = b != NULL ? b->left - (2 * ball->radius) : ball->x;
    break;
    case eRight:
      ball->bearing = 360 - ball->bearing;
      ball->x = b != NULL ? b->right : ball->x;
    break;
    case eTop:
      if(ball->bearing < 180)
        ball->bearing = 180 - ball->bearing;
      else
        ball->bearing = 360 - (ball->bearing - 180);
      ball->y = b != NULL ? b->top - (2 * ball->radius) : ball->y;
    break;
    case eBottom:
      if(ball->bearing < 90)
        ball->bearing = 180 - ball->bearing;
      else
        ball->bearing = 180 + (360 - ball->bearing);
      ball->y = b != NULL ? b->bottom : ball->y;
    break;
  }

  ball->cx = ball->x + ball->radius;
  ball->cy = ball->y + ball->radius;

  return 0;
}

Brick* ball_collidesbricks(Arena* arena, Ball* ball, Edge* e)
{
  for(int brickno = 0; brickno < arena->brickcount; brickno++)
  {
    Brick* brick = arena->bricks[brickno];

    if(brick->hitcount == 0)
      continue;

    // Ball is travelling up and right
    // Check left edge and bottom
    if((ball->cy - brick->bottom < ball->radius) &&
       (brick->left - ball->cx < ball->radius) &&
       (brick->top - ball->cy < ball->radius) &&
       (ball->cx - brick->right < ball->radius))
    {
      int deltar = abs(ball->cx - brick->right);
      int deltal = abs(ball->cx - brick->left);
      int deltat = abs(ball->cy - brick->top);
      int deltab = abs(ball->cy - brick->bottom);


      brick->hitcount--;


      if((ball->bearing < 90) || (ball->bearing > 270))
      {
        // Ball is travelling vaguely up
        // Check bottom side of brick
        if(ball->bearing < 90)
        {
          *e = (deltab < deltal) ? eBottom : eLeft;
        }
        else
        {
          // Ball is travelling up and left
          // Check right edge
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
