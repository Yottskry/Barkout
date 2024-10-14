/*
*
* Barkout - Copyright 2024 Stephen Branley.
*
* This file is part of Barkout.
*
* Barkout is free software: you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation, either version 3 of the License, 
* or (at your option) any later version.
*
* Barkout is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*
*/


#include "ball.h"
#include "config.h"

// Brought this out into its own function so we can use it
// to test collisions with baddies too, and thus also
// change the ball's angle based on the collision
bool ball_collidesBounds(Ball* ball, Bounds* bounds, Edge* e, int* delta)
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

    // Bug: it's possible to be travelling upwards (bearing 290 - up and left)
    // and still collide with the left edge of a moving brick if the brick
    // moves into the ball. This needs to be accounted for.

    if((ball->bearing < 90) || (ball->bearing > 270))
    {
      // Ball is travelling vaguely up
      // Check bottom side of brick

      *e = (deltar < deltal) ? eRight : eLeft;
      *delta = *e == eRight ? deltar : deltal;
      // Now compare the bottom distance to whichever edge was hit
      *e = (*delta < deltab) ? *e : eBottom;
      // If the bottom delta is smaller, delta becomes that, otherwise it stays the same.
      *delta = *e == eBottom ? deltab : *delta;
    }
    else
    {
      // Ball is travelling vaguely down
      // Check top side of brick

      *e = (deltar < deltal) ? eRight : eLeft;
      *delta = *e == eRight ? deltar : deltal;
      // Now compare the top distance to whichever edge was hit
      *e = (*delta < deltat) ? *e : eTop;
      // If the bottom delta is smaller, delta becomes that, otherwise it stays the same.
      *delta = *e == eTop ? deltat : *delta;
    }

    return true;
  }
  return false;
}


Brick* ball_collidesBricks(Ball* ball, Brick** bricks, Bat* player, int brickcount, long* bonuscounter, Edge* e)
{
  int lastd = 0;
  Brick* returnbrick = NULL;
  Edge hitedge = eNone;
  for(int brickno = brickcount-1; brickno >= 0; brickno--)
  {
    Brick* brick = bricks[brickno];

    if(brick->hitcount == 0)
      continue;

    // Resurrecting brick is invisible BUT... counter is not
    // non-zero until animation has finished.
    if(((brick->type & btResurrecting) == btResurrecting) && ((brick->counter > 0) || (brick->hitcount == 0)))
      continue;

    Bounds bounds = {
      .left = brick->left,
      .width = brick->right - brick->left,
      .top = brick->top,
      .height = brick->bottom - brick->top
    };

    // We don't want to stop on the first collision we find
    // as it may hit two bricks at once and we want the closest one
    int d = 0;
    if(ball_collidesBounds(ball, &bounds, e, &d))
    {
      // Only destroy brick if it is vulnerable on that edge
      // and don't decrease hitcount for switch bricks as it
      // can cause odd behaviour later when we try to swap
      // the open/closed state of those bricks
      if((brick->solidedges & *e) != *e)
      {
        if(((brick->type & btSwitch) != btSwitch) && ((brick->type & btResurrecting) != btResurrecting))
        {
   //       brick->hitcount--;
     //     player->score += BRICKSCORE;
        }
        else if((brick->type & btResurrecting) == btResurrecting)
        {
          brick->sprite->state = asPlayToEnd;
          brick->counter = RESURRECTTIMER;
        }
      }

      // Keep track of our bonus creation counter. Increment it on destruction of a normal brick
      if(((brick->type & btNormal) == btNormal) && (brick->hitcount == 0))
        (*bonuscounter)++;

      if((d < lastd) || (lastd == 0))
      {
        returnbrick = brick;
        hitedge = *e;
        lastd = d;
      }
    }
  }

      if(returnbrick != NULL)
        if(((returnbrick->type & btSwitch) != btSwitch) && ((returnbrick->type & btResurrecting) != btResurrecting))
        {
          returnbrick->hitcount--;
          player->score += BRICKSCORE;
          if(((returnbrick->type & btNormal) == btNormal) && (returnbrick->hitcount == 0))
            (*bonuscounter)++;
        }

  *e = hitedge;
  return returnbrick;
}

void ball_ricochet(Ball* ball, Edge hitedge)
{
  switch(hitedge)
  {
    case eLeft:
      if (ball->bearing < 180)
        ball->bearing = 360 - ball->bearing;
    break;
    case eRight:
      if (ball->bearing > 180)
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

void ball_drawBall(Ball* ball, SDL_Renderer* renderer)
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

      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

      if(ball->state == bsDeadly)
        SDL_SetRenderDrawColor(renderer, 255 - (ball->sparkles[i].gdiff * 2),255 - (ball->sparkles[i].gdiff),255,(Uint8)a);
      else
        SDL_SetRenderDrawColor(renderer, 255 - (ball->sparkles[i].gdiff * 2),255, 255 - (ball->sparkles[i].gdiff),(Uint8)a);
      SDL_RenderFillRect(renderer, &(SDL_Rect){.x = ball->sparkles[i].x, .y = ball->sparkles[i].y, .w = 3, .h = 3});

      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

      if(ball->sparkles[i].alpha < 6)
        ball->sparkles[i].alpha = 0;
      else
        ball->sparkles[i].alpha -= 6;

    }
  }
  a_drawsprite(&ball->sprite, renderer, ball->cx - ball->radius, ball->cy - ball->radius);
}

int ball_collidesBat(Ball* ball, Bat* player, Edge* e)
{
  if((ball->bearing >= 270) || (ball->bearing <= 90))
    return 0;

  if ((ball->cy - (player->y + (player->h - 10)) < ball->radius) &&
      (player->x - ball->cx < ball->radius) &&
      (player->y - ball->cy < ball->radius) &&
      (ball->cx - (player->x + (int)(player->w)) < ball->radius))
  {
    *e = eTop;
    if(ball->state == bsLoose)
      ball->cy = player->y - ball->radius - 1;

    // Barkanoid control method, allow spin
    if(*config_getcontrolmethod() == cmBarkanoid)
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
