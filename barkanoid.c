#include "resourcefactory.h"
#include "arena.h"
#include "app.h"
#include "text.h"
#include "intro.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

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

void gameover(App* app, Ball* ball, Bat* player, Arena* arena, Gamestate* gamestate)
{
  *gamestate = gsDying;
  text_drawtext(app, "Game Over!", 202, 302, (SDL_Color){0,0,0,255}, 0);
  text_drawtext(app, "Game Over!", 200, 300, (SDL_Color){255,255,255,255}, 0);
}

// Draw "Get Ready!" text and wait for three seconds
int reset(App* app, Ball* ball, Bat* player, Arena* arena, Gamestate* gamestate)
{
  player->x = arena->width / 2;
  bat_reset(player, arena->factory);
  af_setanimation(arena->factory, &(ball->sprite), "ball", 1, NULL, NULL, NULL);
  ball->state = bsSticky;
  ball->cx = player->x + (player->w / 2);
  ball->cy = player->y - (ball->radius * 2) + 2;
  *gamestate = gsGetReady;
  text_drawtext(app, "Get Ready!", 202, 302, (SDL_Color){0,0,0,255}, 0);
  text_drawtext(app, "Get Ready!", 200, 300, (SDL_Color){255,255,255,255}, 0);
  return 0;
}

int getready(Ball* ball, Bat* player, Gamestate* gamestate, Gamestate nextstate)
{
  SDL_Delay(3000);
  *gamestate = nextstate;
  return 0;
}

int main(int argc, char** argv)
{
	App app;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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

	if(Mix_Init(MIX_INIT_MP3) == -1)
	{
    printf("MIX_Init: %s\n", Mix_GetError());
    return 0;
	}

	int startlevel = 1;
	Uint32 flags = SDL_WINDOW_OPENGL;

	for(int i = 0; i < argc; i++)
	{
    if(strcmp(argv[i], "-f") == 0)
      flags |= SDL_WINDOW_FULLSCREEN;
    if(strcmp(argv[i], "-l") == 0)
    {
      if(i+1 < argc)
        sscanf(argv[i+1], "%d", &startlevel);
    }
	}

  app.font = TTF_OpenFont("Nordine.ttf", 32);
	app.window = SDL_CreateWindow("Barkanoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, flags);
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
	app.music = Mix_LoadMUS("barkanoidii.mp3");

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(app.renderer, 800, 600);

  ResourceFactory f = { .anims = NULL, .animationcount = 0, .samples = NULL, .samplecount = 0 };

  // Load some sprites
  af_loadanimation(&f, app.renderer, "red.png", "red", 44, 29);
  af_loadanimation(&f, app.renderer, "blue.png", "blue", 44, 29);
  af_loadanimation(&f, app.renderer, "green.png", "green", 44, 29);
  af_loadanimation(&f, app.renderer, "darkgrey.png", "darkgrey", 44, 29);
  af_loadanimation(&f, app.renderer, "orange.png", "orange", 44, 29);
  af_loadanimation(&f, app.renderer, "purple.png", "purple", 44, 29);
  af_loadanimation(&f, app.renderer, "grey.png", "grey", 44, 29);
  af_loadanimation(&f, app.renderer, "yellow.png", "yellow", 44, 29);
  af_loadanimation(&f, app.renderer, "bg1.png", "bg1", 600, 600);
  af_loadanimation(&f, app.renderer, "scores.png", "scores", 200, 600);
  af_loadanimation(&f, app.renderer, "bat.png", "bat", 82, 29);
  af_loadanimation(&f, app.renderer, "ball.png", "ball", 17, 17);
  af_loadanimation(&f, app.renderer, "bat_shrink.png", "bat-shrink", 82, 29);
  af_loadanimation(&f, app.renderer, "bonus.png", "bonus-d", 43, 25);
  af_loadanimation(&f, app.renderer, "bonus-s.png", "bonus-s", 43, 25);
  af_loadanimation(&f, app.renderer, "bonus-e.png", "bonus-e", 43, 25);
  af_loadanimation(&f, app.renderer, "bat_small.png", "bat-s", 68, 29);
  af_loadanimation(&f, app.renderer, "ball-deadly.png", "ball-deadly", 17, 17);
  af_loadanimation(&f, app.renderer, "barkanoid-intro.png", "intro", 400, 75);
  af_loadanimation(&f, app.renderer, "life.png", "life", 38, 16);

  af_loadsample(&f, "barkanoid-getready.wav", "getready");
  af_loadsample(&f, "barkanoid-brick.wav", "brick");
  af_loadsample(&f, "barkanoid-bat.wav", "bat");

  Bat player = { .x = 100, .y = 520, .w = psNormal, .h = 25, .maxspeed = 8, .speed = 0, .targetspeed = 0, .lives = 3 };
  player.sprite.anim = af_getanimation(&f, "bat");
  player.sprite.currentframe = 0;
  player.sprite.lastticks = 0;
  player.sprite.loop = 1;
  player.sprite.state = asLooping;

  // Set up the ball
  Ball ball = { .cx = player.x + 40, .cy = 310, .speed = 6, .bearing = 60, .radius = 7, .state = bsSticky };
  ball.sprite.anim = af_getanimation(&f, "ball");
  ball.sprite.currentframe = 0;
  ball.sprite.lastticks = 0;
  ball.sprite.loop = 1;
  ball.sprite.state = asLooping;

  Sprite intro = { .currentframe = 0, .lastticks = 0, .loop = 0, .state = asPlayToEnd };
  intro.anim = af_getanimation(&f, "intro");
  intro.data = NULL;
  intro.sender = NULL;
  intro.onanimfinished = NULL;

  // Set up the level
  Arena arena = { .bounds = { .top = 50, .bottom = 550, .left = 40, .right = 560 },
                  .width = 520,
                  .bonuscounter = 0,
                  .bonuscount = 0,
                  .factory = &f,
                  .bonuses = NULL,
                  .score = 0,
                  .lives = 3
                };

  char levelfile[50] = "";

  sprintf(levelfile, "level%d.lvl", startlevel);

  arena_loadbricks(&arena, &f, levelfile);

  Star stars[STARS];
  intro_init(stars);
  Gamestate gamestate = gsTitle;

  FlashText pressstart = { .text = "Press 1P Start", .alpha = 0, .targetalpha = 255, .duration = 0 };

  FlashStory story1 = {
    .current = 0,
    .count = 4,
    .texts = {
      "After the mothership was destroyed,",
      "only to be warped in space,",
      " ",
      " "
    }
  };

  FlashStory story2 = {
    .current = 0,
    .count = 4,
    .texts = {
      "only ace pilot",
      "trapped on the unforgiving surface",
      " ",
      "Take off every Twig!"
    }
  };

  FlashStory story3 = {
    .current = 0,
    .count = 4,
    .texts = {
      "Willow \"Twiglet\" Rubington III survived,",
      "of a hitherto unknown planet...",
      " ",
      " "
    }
  };


  FlashText txt1 = { .alpha = 0, .targetalpha = 255, .duration = 0 };
  FlashText txt2 = { .alpha = 0, .targetalpha = 255, .duration = 0 };
  FlashText txt3 = { .alpha = 0, .targetalpha = 255, .duration = 0 };
  FlashText fathorse = { .alpha = 0, .targetalpha = 255, .duration =0, .text = "Fat Horse Games presents" };
  bool titlefinished = false;

  while(1)
  {
    // Compare this to the end of the loop to set the frame rate
    Uint32 startticks = SDL_GetTicks();

    // Clear the screen
	  SDL_RenderClear(app.renderer);


	  if((gamestate != gsTitle) && (gamestate != gsStory))
	  {
      // Draw the background
      a_drawstaticframe(af_getanimation(&f, "bg1"), app.renderer, 0, 0, 0);
      a_drawstaticframe(af_getanimation(&f, "scores"), app.renderer, 600, 0, 0);

      for(int i = 0; i < arena.lives; i++)
        a_drawstaticframe(af_getanimation(&f, "life"), app.renderer, 20+(40*i), 560, 0);

      text_drawtext(&app, "BARKANOID", 612, 22, (SDL_Color){0, 0, 0, 255}, 0);
      text_drawtext(&app, "BARKANOID", 610, 20, (SDL_Color){255, 255, 255, 255}, 0);

      char scores[10] = "";

      sprintf(scores, "%08d", arena.score);

      text_drawtext(&app, scores, 612, 82, (SDL_Color){0, 0, 0, 255}, 0);
      text_drawtext(&app, scores, 610, 80, (SDL_Color){255, 255, 255, 255}, 0);

      // bonuses will appear above bricks due to the order here
      arena_drawbricks(&arena, app.renderer);
      arena_drawbonuses(&arena, app.renderer);
    }

	  switch(gamestate)
    {
      case gsTitle:
        if(Mix_PlayingMusic() == 0)
          Mix_PlayMusic(app.music, 0);

        if(!titlefinished){
          // Returns true when text has completed fade in and out
          titlefinished = text_drawflashtext(&app, &fathorse, 200, 160);
        }
        intro_drawstars(app.renderer, stars);
        if(titlefinished){
          a_drawsprite(&intro, app.renderer, 200, 220);
          text_drawflashtext(&app, &pressstart, 260, 300);
        }
        intro_movestars(stars);
      break;
      case gsStory:
        intro_drawstars(app.renderer, stars);
        a_drawsprite(&intro, app.renderer, 200, 220);
        text_drawflashstory(&app, &story1, &txt1, 300);
        text_drawflashstory(&app, &story2, &txt2, 340);
        if(text_drawflashstory(&app, &story3, &txt3, 380))
          gamestate = gsNewLevel;
        intro_movestars(stars);
      break;
      case gsNewLevel:
        if(Mix_PlayingMusic() != 0)
          Mix_HaltMusic();
        reset(&app, &ball, &player, &arena, &gamestate);
      break;
      case gsDying:
        getready(&ball, &player, &gamestate, gsTitle);
      break;
      case gsRunning:
        // Move the ball, check for collisions with bat, arena, and bricks
        // In the event of losing the ball, reset the level
        if(1 == ball_moveball(&ball, &arena, &player))
        {
          arena.lives--;
          if(arena.lives >= 0)
            reset(&app, &ball, &player, &arena, &gamestate);
          else
          {
            gameover(&app, &ball, &player, &arena, &gamestate);
          }
        }

        // Move the bat, check we're within the arena
        bat_movebat(&player, arena.bounds);
        arena_movebonuses(&arena, &player);
        arena_batcollidesbonus(&arena, &player, &ball);
      break;
      case gsGetReady:
        af_playsample(&f, "getready");
        getready(&ball, &player, &gamestate, gsRunning);
      break;
      // Handle pause but do nothing
      case gsPaused: break;
    }

    //a_drawsprite(&bonus, app.renderer, 202, 450);
	  if((gamestate != gsTitle) && (gamestate != gsStory) && (gamestate != gsDying))
	  {
      // Draw the ball
      a_drawsprite(&(ball.sprite), app.renderer, ball.cx - ball.radius, ball.cy - ball.radius);

      // Draw the bat
      bat_drawbat(&player, app.renderer);
    }
    // Display everything on the screen
    SDL_RenderPresent(app.renderer);

    Uint32 endticks = SDL_GetTicks();
		// Fix refresh rate as 60fps
    // This means we need to spend 16.6667 miliseconds per frame,
    // so if less time than that has expired, delay for the remainder.
    if(endticks - startticks < 16.6667)
      SDL_Delay(floor(16.6667 - (endticks - startticks)));

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
          case SDLK_p: gamestate = gamestate == gsRunning ? gsPaused : gsRunning; break;
          case SDLK_SPACE:
          case SDLK_RETURN:
            if(gamestate == gsTitle)
              gamestate = gsStory;
            else if(gamestate == gsStory)
              gamestate = gsNewLevel;
          break;
        }
      }
		}
  }

  // Exiting the program, so free all allocated memory
  //af_freeanimation(&f, "ball");
  arena_freebonuses(&arena);
  arena_freebricks(&arena);

  af_freesamples(&f);
  af_freeanimations(&f);

  SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);

	Mix_FreeMusic(app.music);
	Mix_CloseAudio();
	TTF_CloseFont(app.font);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
