#include "resourcefactory.h"
#include "arena.h"
#include "app.h"
#include "text.h"
#include "intro.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#include <winreg.h>
#endif

#define STARTLIVES 3

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

int loadhighscore()
{
  int score = 0;
  #ifdef _WIN32
  HKEY newKey;
  long retval;
  retval = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\FatHorseGames", 0, KEY_READ, &newKey);
  if(retval == ERROR_SUCCESS)
  {
    long unsigned int lpSize = sizeof(int);
    retval = RegQueryValueExA(newKey, "highscore", 0, NULL, (BYTE*)(&score), &lpSize);
    if(retval != ERROR_SUCCESS)
    {
      score = 0;
    }

  }
  else
  {
    RegCloseKey(newKey);
  }
  #endif
  return score;
}

void savehighscore(int* score)
{
  #ifdef _WIN32
  HKEY newKey;
  long retval;
  retval = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\FatHorseGames", 0, "", 0, KEY_WRITE, 0, &newKey, NULL);
  if(retval == ERROR_SUCCESS)
  {
    RegSetValueExA(newKey, "highscore", 0, REG_DWORD, (BYTE*)score, sizeof(int));
    RegCloseKey(newKey);
  }
  #endif
}

void loadresources(ResourceFactory* f, SDL_Renderer* renderer)
{
  // Load some sprites
  af_loadanimation(f, renderer, "red.png", "red", 44, 29);
  af_loadanimation(f, renderer, "blue.png", "blue", 44, 29);
  af_loadanimation(f, renderer, "green.png", "green", 44, 29);
  af_loadanimation(f, renderer, "darkgrey.png", "darkgrey", 44, 29);
  af_loadanimation(f, renderer, "orange.png", "orange", 44, 29);
  af_loadanimation(f, renderer, "purple.png", "purple", 44, 29);
  af_loadanimation(f, renderer, "grey.png", "grey", 44, 29);
  af_loadanimation(f, renderer, "yellow.png", "yellow", 44, 29);
  af_loadanimation(f, renderer, "bg1.png", "bg1", 600, 600);
  af_loadanimation(f, renderer, "scores.png", "scores", 200, 600);
  af_loadanimation(f, renderer, "bat.png", "bat", 82, 29);
  af_loadanimation(f, renderer, "ball.png", "ball", 17, 17);
  af_loadanimation(f, renderer, "bat_shrink.png", "bat-shrink", 82, 29);
  af_loadanimation(f, renderer, "bonus.png", "bonus-d", 43, 25);
  af_loadanimation(f, renderer, "bonus-s.png", "bonus-s", 43, 25);
  af_loadanimation(f, renderer, "bonus-e.png", "bonus-e", 43, 25);
  af_loadanimation(f, renderer, "bonus-c.png", "bonus-c", 43, 25);
  af_loadanimation(f, renderer, "bonus-p.png", "bonus-p", 43, 25);
  af_loadanimation(f, renderer, "bat_small.png", "bat-s", 51, 27);
  af_loadanimation(f, renderer, "ball-deadly.png", "ball-deadly", 17, 17);
  af_loadanimation(f, renderer, "barkanoid-intro.png", "intro", 400, 75);
  af_loadanimation(f, renderer, "life.png", "life", 38, 16);
  // And some sound
  af_loadsample(f, "barkanoid-getready.wav", "getready");
  af_loadsample(f, "barkanoid-brick.wav", "brick");
  af_loadsample(f, "barkanoid-brick-high.wav", "brick-high");
  af_loadsample(f, "barkanoid-bat.wav", "bat");
  af_loadsample(f, "barkanoid-dead.wav", "dead");
}

void gameover(App* app, Gamestate* gamestate)
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
  arena_freebonuses(arena);
  af_setanimation(arena->factory, &(ball->sprite), "ball", 1, NULL, NULL, NULL);
  ball->state = bsSticky;
  ball->cx = player->x + (player->w / 2);
  ball->cy = player->y - (ball->radius * 2) + 2;
  *gamestate = gsGetReady;
  text_drawtext(app, "Get Ready!", 202, 302, (SDL_Color){0,0,0,255}, 0);
  text_drawtext(app, "Get Ready!", 200, 300, (SDL_Color){255,255,255,255}, 0);
  return 0;
}

int getready(Gamestate* gamestate, Gamestate nextstate)
{
  //SDL_Delay(3000);
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

	SDL_SetCursor(SDL_DISABLE);

  app.font = TTF_OpenFont("Nordine.ttf", 32);
	app.window = SDL_CreateWindow("Barkanoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, flags);
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
	app.music = Mix_LoadMUS("barkanoidii.mp3");

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(app.renderer, 800, 600);

  ResourceFactory f = { .anims = NULL, .animationcount = 0, .samples = NULL, .samplecount = 0 };

  // Load animations and samples
  loadresources(&f, app.renderer);

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
                  .lives = STARTLIVES,
                  .level = startlevel
                };

  arena_loadbricks(&arena, &f);

  int hi = loadhighscore();

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
      "only one craft and ace pilot",
      "trapped on the unforgiving surface",
      "Take off every Twig!",
      " "
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

    Uint32 delay = 0;

    // Clear the screen
	  SDL_RenderClear(app.renderer);


	  if((gamestate != gsTitle) && (gamestate != gsStory))
	  {
      // Draw the background
      a_drawstaticframe(af_getanimation(&f, "bg1"), app.renderer, 0, 0, 0);
      a_drawstaticframe(af_getanimation(&f, "scores"), app.renderer, 600, 0, 0);

      text_drawtext(&app, "BARKANOID", 612, 22, (SDL_Color){0, 0, 0, 255}, 0);
      text_drawtext(&app, "BARKANOID", 610, 20, (SDL_Color){255, 255, 255, 255}, 0);

      char highs[10] = "";

      sprintf(highs, "%08d", hi);

      text_drawtext(&app, highs, 612, 82, (SDL_Color){0, 0, 0, 255}, 0);
      text_drawtext(&app, highs, 610, 80, (SDL_Color){255, 255, 255, 255}, 0);

      char scores[10] = "";

      sprintf(scores, "%08d", arena.score);

      text_drawtext(&app, scores, 612, 142, (SDL_Color){0, 0, 0, 255}, 0);
      text_drawtext(&app, scores, 610, 140, (SDL_Color){255, 255, 255, 255}, 0);

      // bonuses will appear above bricks due to the order here
      arena_drawbricks(&arena, app.renderer);
    }

    if(gamestate == gsTitle)
    {
      if(Mix_PlayingMusic() == 0)
          Mix_PlayMusic(app.music, 0);

      if(!titlefinished){
        // Returns true when text has completed fade in and out
        titlefinished = text_drawflashtext(&app, &fathorse, 200, 160, 2);
      }
      intro_drawstars(app.renderer, stars);
      if(titlefinished){
        a_drawsprite(&intro, app.renderer, 200, 220);
        text_drawflashtext(&app, &pressstart, 260, 300, 2);
      }
      intro_movestars(stars);
    }

    if(gamestate == gsStory)
    {
      titlefinished = true;
      arena.level = 1;
      intro_drawstars(app.renderer, stars);
      a_drawsprite(&intro, app.renderer, 200, 220);
      text_drawflashstory(&app, &story1, &txt1, 300);
      text_drawflashstory(&app, &story2, &txt2, 340);
      if(text_drawflashstory(&app, &story3, &txt3, 380))
        gamestate = gsNewLevel;
      intro_movestars(stars);
    }
    // Again, we want another frame to draw the level
    // before we say "Get Ready!" so we need an else
    // to avoid moving directly from gsStory to gsNewLevel
    // in one loop iteration.
    else if(gamestate == gsNewLevel)
    {
      if(Mix_PlayingMusic() != 0)
        Mix_HaltMusic();
      reset(&app, &ball, &player, &arena, &gamestate);
    }

    if(gamestate == gsRunning)
    {
      // Move the ball, check for collisions with bat, arena, and bricks
      // In the event of losing the ball, reset the level
      if(1 == ball_moveball(&ball, &arena, &player))
      {
        // What we really want to do here is decrease the lives
        // remove any bonuses, and THEN show the "Get Ready!" text
        // Which means the delay needs to come after the next
        // renderpresent
        af_playsample(&f, "dead");
        while(Mix_Playing(-1));
        arena.lives--;
        if(arena.lives >= 0)
        {
          // Lives already reset, but ideally we want to delay
          // the delay() call until after the next render
          reset(&app, &ball, &player, &arena, &gamestate);
          delay = 3000;
        }
        else
        {
          gameover(&app, &gamestate);
          savehighscore(((int*)&arena.score));
          arena_resetbricks(&arena);
        }
      }

      arena_drawbonuses(&arena, app.renderer);

      if(arena.remaining == 0)
      {
        // Load new level
        arena_freebricks(&arena);
        arena.level++;
        arena_loadbricks(&arena, &f);
        gamestate = gsNewLevel;
        continue;
      }

      // Move the bat, check we're within the arena
      bat_movebat(&player, arena.bounds);
      arena_movebonuses(&arena);
      arena_batcollidesbonus(&arena, &player, &ball);
    } // This one is an else because we need one loop between
      // change of gamestate for the Get Ready text to render.
    if(gamestate == gsGetReady)
    {
      af_playsample(&f, "getready");
      delay = 3000;
      //getready(&gamestate, gsRunning);
      gamestate = gsRunning;
    }

    if(gamestate == gsDying)
    {
      //getready(&gamestate, gsTitle);
      gamestate = gsTitle;
      delay = 3000;
    }

    //a_drawsprite(&bonus, app.renderer, 202, 450);
	  if((gamestate != gsTitle) && (gamestate != gsStory) && (gamestate != gsDying))
	  {

      arena_drawlives(&arena, &app);
      // Draw the ball
      if((ball.cy - ball.radius) < (arena.bounds.bottom - 15))
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

    if(delay > 0)
      SDL_Delay(delay);

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
            ball.state = ball.state == bsSticky ? bsNormal : ball.state == bsStuck ? bsLoose : ball.state;
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
            {
              gamestate = gsNewLevel;
              arena.lives = STARTLIVES;
            }
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
