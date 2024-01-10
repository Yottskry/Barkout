#include "ball.h"

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
    // Determin distance to each each of the brick so we
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
