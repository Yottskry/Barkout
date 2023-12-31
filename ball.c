#include "ball.h"

Brick* ball_collidesbricks(Ball* ball, Brick** bricks, int brickcount, Edge* e)
{
  for(int brickno = brickcount-1; brickno >= 0; brickno--)
  {
    if(bricks[brickno]->hitcount == 0)
      continue;

    Brick* brick = bricks[brickno];

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
