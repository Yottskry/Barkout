#include "resourcefactory.h"
#include "ball.h"
#include "bat.h"
#include "app.h"
#include "text.h"
#include <SDL.h>
#include <SDL_ttf.h>

/* Have 13 bricks per level width. Let's say 600x600 playing area with 200x600 score area
-----------------
|           |   |
|           |   |
|           |   |
|           |   |
|           |   |
|           |   |
-----------------

40px * 13 bricks = 520px + 40 each side for walls = 600

*/

// Draw "Get Ready!" text and wait for three seconds
int reset(App* app, Ball* ball, Bat* player, Arena* arena, Gamestate* gamestate)
{
  player->x = arena->width / 2;
  ball->state = bsSticky;
  ball->cx = player->x + (player->w / 2);
  ball->cy = player->y - (ball->radius * 2);
  *gamestate = gsGetReady;
  text_drawtext(app, "Get Ready!", 200, 300, (SDL_Color){255,255,255});
  return 0;
}

int getready(Ball* ball, Bat* player, Gamestate* gamestate)
{
  SDL_Delay(3000);
  *gamestate = gsRunning;
  return 0;
}

int main(int argc, char** argv)
{
	App app;
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return 1;
	}

	IMG_Init(IMG_INIT_PNG);

	if(TTF_Init() == -1)
	{
    printf("TTF_Init: %s\n", TTF_GetError());
    return 0;
	}

	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);

  app.font = TTF_OpenFont("pixpopenei.ttf", 30);
	app.window = SDL_CreateWindow("Barkanoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

  ResourceFactory f = { .anims = NULL, .animationcount = 0, .samples = NULL, .samplecount = 0 };

  // Load some sprites
  af_loadanimation(&f, app.renderer, "red.png", "red", 44, 29);
  af_loadanimation(&f, app.renderer, "blue.png", "blue", 44, 29);
  af_loadanimation(&f, app.renderer, "green.png", "green", 44, 29);
  af_loadanimation(&f, app.renderer, "darkgrey.png", "darkgrey", 44, 29);
  af_loadanimation(&f, app.renderer, "orange.png", "orange", 44, 29);
  af_loadanimation(&f, app.renderer, "grey.png", "grey", 44, 29);
  af_loadanimation(&f, app.renderer, "yellow.png", "yellow", 44, 29);
  af_loadanimation(&f, app.renderer, "bg1.png", "bg1", 600, 600);
  af_loadanimation(&f, app.renderer, "bat.png", "bat", 82, 29);

  af_loadsample(&f, "barkanoid-getready.wav", "getready");

  Bat player = { .x = 100, .y = 520, .w = 80, .h = 25, .maxspeed = 8, .speed = 0, .targetspeed = 0 };
  player.anim = af_getanimation(&f, "bat");

  // Set up the ball
  Ball ball = { .cx = player.x + 40, .cy = 310, .speed = 8, .bearing = 60, .radius = 7, .state = bsSticky };
  ball.anim = af_loadanimation(&f, app.renderer, "ball.png", "ball", 15, 15);

  // Set up the level
  Arena arena = {.top = 50, .bottom = 550, .left = 40, .right = 560, .width = 520 };
  arena_loadbricks(&arena, &f, "level1.lvl");

  Gamestate gamestate = gsNewLevel;

  while(1)
  {
    // Compare this to the end of the loop to set the frame rate
    Uint32 startticks = SDL_GetTicks();

    SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}

			if (e.type == SDL_KEYUP)
      {
        switch(e.key.keysym.sym)
        {
          case SDLK_z:
          case SDLK_LEFT: player.targetspeed = player.speed > 0 ? player.targetspeed : 0; break;
          case SDLK_x:
          case SDLK_RIGHT: player.targetspeed = player.speed < 0 ? player.targetspeed : 0; break;
          case SDLK_SPACE:
            ball.bearing = ball.state == bsSticky ? 30 : ball.bearing;
            ball.state = ball.state == bsSticky ? bsNormal : ball.state;
          break;
        }
      }

      if (e.type == SDL_KEYDOWN)
      {
        // Not part of the switch because we need "break" to break the loop...
        if(e.key.keysym.sym == SDLK_ESCAPE)
        {
          break;
        }

        switch(e.key.keysym.sym)
        {
          case SDLK_z:
          case SDLK_LEFT: player.targetspeed = -1 * player.maxspeed; break;
          case SDLK_x:
          case SDLK_RIGHT: player.targetspeed = player.maxspeed; break;
          case SDLK_UP: ball.bearing += 5; break;
          case SDLK_DOWN: ball.bearing -= 5; break;
        }
      }
		}

    // Clear the screen
	  SDL_RenderClear(app.renderer);

	  // Draw the background
	  a_drawstaticframe(af_getanimation(&f, "bg1"), app.renderer, 0, 0);

	  text_drawtext(&app, "BARKANOID", 602, 20, (SDL_Color){255, 0, 0});

	  // Draw all bricks
	  arena_drawbricks(&arena, app.renderer);

	  switch(gamestate)
    {
      case gsNewLevel:
        reset(&app, &ball, &player, &arena, &gamestate);
      break;
      case gsRunning:
        // Move the ball, check for collisions with bat, arena, and bricks
        // In the event of losing the ball, reset the level
        if(1 == ball_moveball(&ball, &arena, &player))
        {
          //SDL_RenderPresent(app.renderer);
          reset(&app, &ball, &player, &arena, &gamestate);
        }

        // Move the bat, check we're within the arena
        bat_movebat(&player, &arena);
      break;
      case gsGetReady:
        af_playsample(&f, "getready");
        getready(&ball, &player, &gamestate);
      break;
    }

	  // Draw the ball
	  a_drawstaticframe(ball.anim, app.renderer, ball.cx - ball.radius, ball.cy - ball.radius);

	  // Draw the bat
	  bat_drawbat(&player, app.renderer);

    // Display everything on the screen
    SDL_RenderPresent(app.renderer);

    Uint32 endticks = SDL_GetTicks();
		// Fix refresh rate as 60fps
    // This means we need to spend 16.6667 miliseconds per frame,
    // so if less time than that has expired, delay for the remainder.
    if(endticks - startticks < 16.6667)
      SDL_Delay(floor(16.6667 - (endticks - startticks)));
  }

  // Exiting the program, so free all allocated memory
  //af_freeanimation(&f, "ball");
  arena_freebricks(&arena);

  af_freesamples(&f);
  af_freeanimations(&f);

  SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);

	Mix_CloseAudio();
	TTF_CloseFont(app.font);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
