#include "resourcefactory.h"
#include "arena.h"
#include "app.h"
#include "text.h"
#include "intro.h"
#include "cat.h"
#include "scores.h"
#include "config.h"
#include "menu.h"
#include "levels.h"
#include "vector.h"
#include "resources.h"
#include <time.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define STARTLIVES 3
#define OPT1 "Ball angle is determined by segment of bat hit"
#define OPT2 "Bat speed and direction influences ball spin and angle"
#define DON1 "This game is a labour of love. All programming, graphics, music, and sound effects were produced by me."
#define DON2 "If you enjoy playing it, please consider making a small donation by buying me a coffee through the link below."
#define DON3 "https://buymeacoffee.com/retrojunkies"

static void printDiagnostics(Ball* ball, Arena* arena)
{
  printf("Diagnostics: \n");
  printf("ball speed: %d\n", ball->speed);
  printf("ball bearing: %f\n", ball->bearing);
  printf("remaining bricks: %d\n", arena->remaining);
  printf("current level: %d\n", arena->level);
  printf("-------------------------------------------------\n\n");

  for(int i = 0; i < arena->brickcount; i++)
  {
    Brick* brick = arena->bricks[i];
    printf("Brick %d (%d)\n", i, brick->type);
    printf("Counter:  %d\n", brick->counter);
    printf("IsDead:   %d\n", brick->isdead);
    printf("HitCount: %d\n", brick->hitcount);
    printf("* * * * * * * * * * *\n");
  }
}

static int gameOver(App* app, Bat* player, Gamestate* gamestate, int* hi)
{
  *gamestate = gsDying;
  text_drawText(app, "Game Over!", 200, 300, (SDL_Color){255,255,255,255}, TEXT_SHADOW, fnTitle);
  if(player->score > *hi)
  {
    saveHighScore(((int*)&player->score));
    *hi = player->score;
  }
  return *hi;
}

// Draw "Get Ready!" text and wait for three seconds
static int reset(App* app, Ball* ball, Bat* player, Arena* arena, Gamestate* gamestate)
{
  player->x = arena->width / 2;
  bat_reset(player, arena->factory);
  bonus_freebonuses(&arena->bonuses, &arena->bonuscount);
  arena_freeBullets(arena);
  arena->counter = SDL_GetTicks();
  af_setanimation(arena->factory, &(ball->sprite), "ball", 1, NULL, NULL, NULL);
  ball->state = bsSticky;
  ball->cx = player->x + (player->w / 2);
  ball->cy = player->y - (ball->radius);
  ball->speed = 6;
  ball->warpdest = NULL;
  *gamestate = gsGetReady;
  text_drawText(app, "Get Ready!", 200, 300, (SDL_Color){255,255,255,255}, TEXT_SHADOW, fnTitle);
  return 0;
}

void menu_quitClick(void* data)
{
  App* app = (App*)data;
  app->gamestate = gsQuit;
}

void menu_startClick(void* data)
{
  App* app = (App*)data;
  app->gamestate = gsStory;
}

void menu_creditsClick(void* data)
{
  App* app = (App*)data;
  app->gamestate = gsCredits;
}

void menu_howToPlayClick(void* data)
{
  App* app = (App*)data;
  app->gamestate = gsHelp;
}

void menu_fullScreenToggle(void* data, void* item)
{
  App* app = (App*)data;
  MenuItem* menuitem = (MenuItem*)item;

  if(menuitem->selectedvalue == 1)
    SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN);
  else
    SDL_SetWindowFullscreen(app->window, 0);
}

void menu_donateClick(void* data)
{
  App* app = (App*)data;
  app->gamestate = gsDonate;
}

static void drawLives(App* app, Bat* player, ResourceFactory* factory)
{
  for(int i = 0; i < player->lives; i++)
    a_drawstaticframe(af_getanimation(factory, "life"), app->renderer, 40+(40*i), 560, 0, 255);
}

static void drawBackground(App* app, Arena* arena, Bat* player, ResourceFactory* factory)
{
  int ofs = 0;
  int ofs2 = 0;

  if(player != NULL)
  {
    ofs = (player->x - ((arena->bounds.right + arena->bounds.left) / 2)) / -30;
    ofs2 = (player->x - ((arena->bounds.right + arena->bounds.left) / 2)) / -10;
  }

  // Draw the background
  a_drawstaticframe(arena->bg, app->renderer, 0, 0, 0, arena->alpha);
  a_drawstaticframe(arena->mg, app->renderer, ofs, 0, 0, arena->alpha);
  a_drawstaticframe(arena->fg, app->renderer, ofs2 - 30, 0, 0, arena->alpha);
  a_drawstaticframe(af_getanimation(factory, "scores"), app->renderer, 600, 0, 0, 255);
  a_drawstaticframe(af_getanimation(factory, "border"), app->renderer, 0, 0, 0, 255);
}

static void drawArenaText(App* app, Arena* arena, Bat* player, int hi)
{
  char highs[10] = "";
  char scores[10] = "";
  char level[4] = "";

  sprintf(highs, "%08d", hi);
  sprintf(scores, "%08d", player->score);
  sprintf(level, "%02d", arena->level);

  text_drawText(app, "BARKOUT", 610, 20, (SDL_Color){255, 255, 255, 255}, TEXT_SHADOW, fnTitle);
  text_drawText(app, "Hi Score", 610, 80, (SDL_Color){255,255,255,255}, TEXT_SHADOW, fnTitle);
  text_drawText(app, highs, 610, 120, (SDL_Color){255, 255, 255, 255}, TEXT_SHADOW, fnTitle);
  text_drawText(app, "Score", 610, 200, (SDL_Color){255,255,255,255}, TEXT_SHADOW, fnTitle);
  text_drawText(app, scores, 610, 240, (SDL_Color){255, 255, 255, 255}, TEXT_SHADOW, fnTitle);
  text_drawText(app, "Round", 610, 320, (SDL_Color){255,255,255,255}, TEXT_SHADOW, fnTitle);
  text_drawText(app, level, 740, 320, (SDL_Color){255, 255, 255, 255}, TEXT_SHADOW, fnTitle);
}

static void drawHowToPlay(App* app, Sprite* sprites)
{
  int left = 40;
  int top = 30;

  SDL_Color white = {255,255,255,255};

  text_drawText(app, "How to Play", left, top, white, 0, fnStory);

  char* howtotext = "Use your craft, Maus, to direct "
                    "the energy ball and destroy the "
                    "infrastructure of the evil Cat Empire!";
  top = 75;
  text_drawWrappedText(app, howtotext, left, top, white, 0, 700, fnStory);

  char* bonustext = "Some of the structure may reveal "
                    "powerful bonuses when destroyed. "
                    "Use them wisely!";

  top = 185;
  text_drawWrappedText(app, bonustext, left, top, white, 0, 700, fnStory);

  top = 300;
  int snum = 0;

  a_drawsprite(&(sprites[snum++]), app->renderer, left, top);
  text_drawWrappedText(app, "The ball sticks to the Maus", left + 80, top - 5, white, 0, 600, fnBody);
  top += 35;

  a_drawsprite(&(sprites[snum++]), app->renderer, left, top);
  text_drawWrappedText(app, "Enable the Maus's laser guns", left + 80, top - 5, white, 0, 600, fnBody);
  top += 35;

  a_drawsprite(&(sprites[snum++]), app->renderer, left, top);
  text_drawWrappedText(app, "Enhance the energy ball", left + 80, top - 5, white, 0, 600, fnBody);
  top += 35;

  a_drawsprite(&(sprites[snum++]), app->renderer, left, top);
  text_drawWrappedText(app, "Extend the Maus!", left + 80, top - 5, white, 0, 600, fnBody);
  top += 35;

  a_drawsprite(&(sprites[snum++]), app->renderer, left, top);
  text_drawWrappedText(app, "Shrink the Maus. Avoid this one.", left + 80, top - 5, white, 0, 600, fnBody);
  top += 35;

  a_drawsprite(&(sprites[snum++]), app->renderer, left, top);
  text_drawWrappedText(app, "Extra player!", left + 80, top - 5, white, 0, 600, fnBody);
  top += 35;

  a_drawsprite(&(sprites[snum++]), app->renderer, left, top);
  text_drawWrappedText(app, "Warp to the next round", left + 80, top - 5, white, 0, 600, fnBody);
}

static void drawCredits(App* app)
{
  int left = 150;

  SDL_Color white = {255,255,255,255};

  text_drawText(app, "Credits", left, 110, white, 0, fnTitle);
  text_drawText(app, "_____________________________________", left, 120, white, 0, fnTitle);
  text_drawText(app, "Programming", left, 160, white, 0, fnTitle);
  text_drawText(app, "Graphics", left, 200, white, 0, fnTitle);
  text_drawText(app, "Sound FX", left, 240, white, 0, fnTitle);
  text_drawText(app, "Music", left, 280, white, 0, fnTitle);

  text_drawText(app, "Go team!", left, 360, white, 0, fnTitle);

  for(int i = 0; i < 4; i++)
    text_drawText(app, "Fat Harry", left + 260, 160 + (i*40), white, 0, fnTitle);
}

static void drawDonate(App* app)
{
  int left = 100;

  SDL_Color white = {255,255,255,255};

  text_drawWrappedText(app, "Donate! (please)", left, 100, white, 0, 600, fnBody);
  text_drawWrappedText(app, DON1, left, 160, white, 0, 500, fnBody);
  text_drawWrappedText(app, DON2, left, 280, white, 0, 500, fnBody);
  text_drawWrappedText(app, DON3, left, 400, white, 0, 500, fnBody);
  text_drawWrappedText(app, "Thanks.", left, 480, white, 0, 500, fnBody);
}

int main(int argc, char** argv)
{
  srand(time(0));
  Uint32 flags = SDL_WINDOW_OPENGL;
  int startlevel = 1;
  bool binarylevels = true;
  char customlevelfile[255] = "";
  bool customlevels = false;
  // Test parameters here before initialising anything
  // as we may only be compiling a level file
  for(int i = 0; i < argc; i++)
	{
    // Play fullscreen
    if(strcmp(argv[i], "-f") == 0)
    {
      flags |= SDL_WINDOW_FULLSCREEN;
    }
    // Select starting level
    if(strcmp(argv[i], "-l") == 0)
    {
      if(i+1 < argc)
        sscanf(argv[i+1], "%d", &startlevel);
    }
    // Compile a level file
    if(strcmp(argv[i], "-c") == 0)
    {
      char folder[255];
      if(i+1 < argc)
        sscanf(argv[i+1], "%s", folder);
      levels_compile(folder, ".");
      // Compile level file and quit
      return 0;
    }
    // Load uncompressed level files
    if(strcmp(argv[i], "-u") == 0)
    {
      binarylevels = false;
    }
    // Load custom compressed level file
    if(strcmp(argv[i], "-f") == 0)
    {
      binarylevels = true;
      customlevels = true;
      if(i+1 < argc)
        sscanf(argv[i+1], "%s", customlevelfile);
    }

	}

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

	Config* config = config_load();

	if((flags & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN)
    config_setfullscreen(true);

  if(config->fullscreen)
    flags = flags | SDL_WINDOW_FULLSCREEN;

  text_loadFonts(&app);

	app.window = SDL_CreateWindow("Barkout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, flags);
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

	app.music = af_loadmusic("barkanoidiii.mp3");

  app.gamestate = gsTitle;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(app.renderer, 800, 600);

  ResourceFactory f = { .anims = NULL, .animationcount = 0, .samples = NULL, .samplecount = 0 };

  // Load animations and samples
  loadResources(&f, app.renderer);

  Bat player = { .x = 100, .y = 520, .w = psNormal, .h = 25, .maxspeed = 8, .speed = 0, .targetspeed = 0, .lives = STARTLIVES, .score = 0 };
  player.sprite.anim = af_getanimation(&f, "bat");
  player.sprite.currentframe = 0;
  player.sprite.lastticks = 0;
  player.sprite.loop = 1;
  player.sprite.state = asLooping;
  player.warpenabled = false;

  // Set up the ball
  Ball ball = { .cx = player.x + 40, .cy = 310, .speed = 6, .bearing = 60, .radius = 7, .state = bsSticky };
  ball.sprite.anim = af_getanimation(&f, "ball");
  ball.sprite.currentframe = 0;
  ball.sprite.lastticks = 0;
  ball.sprite.loop = 1;
  ball.sprite.state = asLooping;
  for(int i = 0; i < MAXTRAILPARTICLES; i++)
  {
    ball.sparkles[i].alpha = 0;
    ball.sparkles[i].x = 0;
    ball.sparkles[i].y = 0;
    ball.sparkles[i].gdiff = 0;
  }

  Sprite intro = { .currentframe = 0, .lastticks = 0, .loop = 0, .state = asPlayToEnd };
  intro.anim = af_getanimation(&f, "intro");
  intro.data = NULL;
  intro.sender = NULL;
  intro.onanimfinished = NULL;

  Sprite warp = { .currentframe = 0, .lastticks = 0, .loop = 1, .state = asLooping };
  warp.anim = af_getanimation(&f, "warp");
  warp.data = NULL;
  warp.sender = NULL;
  warp.onanimfinished = NULL;

  // Set up the level
  Arena arena = { .bounds = { .top = 40, .bottom = 550, .left = 40, .right = 560 },
                  .width = ARENAW,
                  .bonuscounter = 0,
                  .bonuscount = 0,
                  .factory = &f,
                  .bonuses = NULL,
                  .level = startlevel,
                  .counter = 0,
                  .bulletcount = 0,
                  .bullets = NULL,
                  .bg = NULL,
                  .numlevels = 0,
                  .levels = NULL,
                  .alpha = 255,
                  .multiplier = 1
                };

  int spritecount = 7;
  Sprite bonussprites[spritecount];
  // "dsecplw";
  // Yes, 7. No \0.
  char bonustypes[7] = "cldespw";
  char bonusstring[8] = "bonus-x";
  for(int i=0; i < spritecount; i++)
  {
    //Sprite* s = malloc(sizeof(Sprite));
    // Change the last letter of the bonus sprite name
    bonusstring[6] = bonustypes[i];
    bonussprites[i].currentframe = 0;
    bonussprites[i].lastticks = 0;
    bonussprites[i].data = NULL;
    bonussprites[i].sender = NULL;
    bonussprites[i].onanimfinished = NULL;
    bonussprites[i].loop = 1;
    bonussprites[i].state = asLooping;
    bonussprites[i].anim = af_getanimation(&f, bonusstring);
  }

  Menu menu = { .itemcount = 0, .items = NULL, .selectedindex = 0, .optionx = 500, .x = 100, .y = 240, .app=&app };
  MenuItem* item = menu_addItem(&menu, "Start game", NULL, menu_startClick, NULL);
  item = menu_addItem(&menu, "Control method", (int*)&(config->controlmethod), NULL, NULL);
  menu_addItemOption(item, "Barkout", OPT2, (int)cmBarkanoid);
  menu_addItemOption(item, "Classic", OPT1, (int)cmClassic);
  item = menu_addItem(&menu, "Full screen", (int*)&(config->fullscreen), NULL, menu_fullScreenToggle);
  menu_addItemOption(item, "Yes", "Stretch to full screen", 1);
  menu_addItemOption(item, "No", "Play windowed", 0);
  menu_addItem(&menu, "How to Play", NULL, menu_howToPlayClick, NULL);
  menu_addItem(&menu, "Credits", NULL, menu_creditsClick, NULL);
  menu_addItem(&menu, "Donate!", NULL, menu_donateClick, NULL);
  menu_addItem(&menu, "Quit", NULL, menu_quitClick, NULL);

	SDL_ShowCursor(SDL_DISABLE);

  if(binarylevels)
  {
    char* levelfile = "levels.dat";
    char apath[255] = "./Levels/";

    #ifdef INSTALLDIR
    if(config_getinstalled())
      strcpy(apath, INSTALLDIR "/Levels/");
    #endif
    strcat(apath, levelfile);
    if(customlevels)
      arena_loadBinary(&f, &arena, customlevelfile);
    else
      arena_loadBinary(&f, &arena, apath);
  }
  else
    arena_loadLevels(&arena, &f);

  int hi = loadHighScore();

  Star stars[STARS];
  intro_init(stars);

  FlashText pressstart = { .text = "Press 1P Start", .alpha = 0, .targetalpha = 255, .duration = 0, .font = fnTitle };

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
      "only pilot Willow \"Twiglet\" Twiglington",
      "trapped on the unforgiving surface",
      "Take off every Twig!",
      " "
    }
  };

  FlashStory story3 = {
    .current = 0,
    .count = 4,
    .texts = {
      "and her spacecraft The Maus survived",
      "of a hitherto unknown planet...",
      " ",
      " "
    }
  };

  FlashText txt1 = { .alpha = 0, .targetalpha = 255, .duration = 0, .font = fnStory };
  FlashText txt2 = { .alpha = 0, .targetalpha = 255, .duration = 0, .font = fnStory };
  FlashText txt3 = { .alpha = 0, .targetalpha = 255, .duration = 0, .font = fnStory };
  FlashText fathorse = { .alpha = 0, .targetalpha = 255, .duration = 0, .text = "Fat Horse Games presents", .font = fnTitle };

  bool titlefinished = false;
  int currentlywarping = 0;

  while(app.gamestate != gsQuit)
  {
    // Compare this to the end of the loop to set the frame rate
    Uint32 startticks = SDL_GetTicks();
    Uint32 delay = 0;

    int islostball = 0;

    // If we check the keys here, then in many cases the state
    // will only change here, so we'll draw the correct frame each time
    SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}

			/* // We'll come back to this...
			if (e.type == SDL_MOUSEMOTION)
			{
        if (e.motion.x > player.x + player.w)
        {
          if(currentlywarping == 0)
            player.targetspeed = player.maxspeed;
        }
        else if (e.motion.xrel < player.x)
        {
          if(currentlywarping == 0)
            player.targetspeed = -1 * player.maxspeed;
        }
        else
        {
          // Mouse is stationary
          player.targetspeed = 0;
        }
			}
      */

			if (e.type == SDL_KEYUP)
      {
        switch(e.key.keysym.sym)
        {
          case SDLK_z:
          case SDLK_LEFT:
            if(currentlywarping == 0)
              player.targetspeed = player.speed > 0 ? player.targetspeed : 0;
          break;
          case SDLK_x:
          case SDLK_RIGHT:
            if(currentlywarping == 0)
              player.targetspeed = player.speed < 0 ? player.targetspeed : 0;
          break;
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
          if((app.gamestate != gsTitle) && (app.gamestate != gsStory))
            app.gamestate = gsTitle;
          else
            break;
        }

        switch(e.key.keysym.sym)
        {
          case SDLK_1:
            if(app.gamestate == gsRunning)
              arena_addBonus(&arena, 200, 300, boLaser);
          break;

          case SDLK_k:
            // Kill self - if stuck in a loop, for example
            if(app.gamestate == gsRunning)
                islostball = 1;
          break;

          case SDLK_z:
          case SDLK_LEFT:
            if(app.gamestate == gsRunning)
            {
              if(currentlywarping == 0)
                player.targetspeed = -1 * player.maxspeed;
            }
            else if(app.gamestate==gsMenu)
            {
              menu_previousOption(&menu);
            }
          break;
          case SDLK_x:
          case SDLK_RIGHT:
            if(app.gamestate == gsRunning)
            {
              if(currentlywarping == 0)
                player.targetspeed = player.maxspeed;
            }
            else if(app.gamestate==gsMenu)
            {
              menu_nextOption(&menu);
            }
          break;
          case SDLK_p: app.gamestate = app.gamestate == gsRunning ? gsPaused : gsRunning; break;
          case SDLK_SPACE:
            if((app.gamestate == gsRunning) && (player.state == plLaser))
            {
              af_playsample(&f, "laser");
              arena_addBullet(&arena, &player);
            }
          // fall through
          case SDLK_RETURN:
            if((app.gamestate == gsTitle) || (app.gamestate == gsCredits) || (app.gamestate == gsHelp) || (app.gamestate == gsDonate))
              app.gamestate = gsMenu;
            else if(app.gamestate == gsMenu)
              menu_execute(&menu);
            else if(app.gamestate == gsStory)
            {
              app.gamestate = gsNewLevel;
              player.lives = STARTLIVES;
              arena.level = startlevel;
              player.score = 0;
              arena.alpha = 255;
              arena_loadBricks(&arena, arena.level);
            }
          break;
          case SDLK_DOWN:
            if(app.gamestate==gsMenu)
            {
              menu_next(&menu);
            }
          break;
          case SDLK_UP:
            if(app.gamestate==gsMenu)
            {
              menu_previous(&menu);
            }
          break;
          case SDLK_F1:
            printDiagnostics(&ball, &arena);
          break;
          case SDLK_n:
            if(app.gamestate == gsRunning)
            {
              // Skip to next level
              arena.remaining = 0;
            }
          break;
          case SDLK_l:
            if(app.gamestate == gsRunning)
              player.state = plLaser;
          break;
        }
      }
		}

		// Clear the screen
		SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
	  SDL_RenderClear(app.renderer);

    if(app.gamestate == gsTitle)
    {
      if(Mix_PlayingMusic() == 0)
          Mix_PlayMusic(app.music, 0);

      if(!titlefinished){
        // Returns true when text has completed fade in and out
        titlefinished = text_drawFlashText(&app, &fathorse, 200, 100, 2);
      }
      intro_drawstars(app.renderer, stars);
      if(titlefinished){
        a_drawsprite(&intro, app.renderer, 200, 160);
        text_drawFlashText(&app, &pressstart, 260, 220, 2);
        //a_drawstaticframe(af_getanimation(&f, "logo"), app.renderer, 700, 500, 0, 255);
      }

      intro_movestars(stars);
    }

    if(app.gamestate == gsMenu)
    {
      titlefinished = true;

      intro_drawstars(app.renderer, stars);
      a_drawsprite(&intro, app.renderer, 200, 160);

      menu_drawMenu(&menu, &app);

      intro_movestars(stars);
      // problem is that on our next loop, if we've changed
      // to gsNewLevel we draw one single frame of the previous
      // level layout
      a_drawstaticframe(af_getanimation(&f, "logo"), app.renderer, 700, 500, 0, 255);
    }

    if((app.gamestate == gsCredits) || (app.gamestate == gsHelp) || (app.gamestate == gsDonate))
    {
      titlefinished = true;
      intro_drawstars(app.renderer, stars);

      if(app.gamestate == gsCredits)
        drawCredits(&app);
      else if(app.gamestate == gsHelp)
        drawHowToPlay(&app, bonussprites);
      else
        drawDonate(&app);

      intro_movestars(stars);
    }

    if(app.gamestate == gsStory)
    {
      titlefinished = true;
      arena.level = startlevel;
      intro_drawstars(app.renderer, stars);
      a_drawsprite(&intro, app.renderer, 200, 220);
      text_drawFlashStory(&app, &story1, &txt1, 300);
      text_drawFlashStory(&app, &story2, &txt2, 340);
      if(text_drawFlashStory(&app, &story3, &txt3, 380))
        app.gamestate = gsNewLevel;
      intro_movestars(stars);
    }
    // Again, we want another frame to draw the level
    // before we say "Get Ready!" so we need an else
    // to avoid moving directly from gsStory to gsNewLevel
    // in one loop iteration.
    else if(app.gamestate == gsNewLevel)
    {
      if(Mix_PlayingMusic() != 0)
        Mix_HaltMusic();
      arena_loadBricks(&arena, arena.level);
      drawBackground(&app, &arena, NULL, &f);
      drawArenaText(&app, &arena, &player, hi);
      arena_drawBricks(&arena, app.renderer);
      arena_drawBricks(&arena, app.renderer);
      // Reset immediately changes the state to gsGetReady
      // So this block only executes once

      Vector* cats = arena.levels[arena.level - 1].cats;

      for(int cnum = 0; cnum < cats->size; cnum++)
        ((Cat*)(cats->elements[cnum]))->state = csDead;

      reset(&app, &ball, &player, &arena, &app.gamestate);
    }

    if(app.gamestate == gsPaused)
    {
      drawBackground(&app, &arena, &player, &f);
      drawArenaText(&app, &arena, &player, hi);
      arena_drawBricks(&arena, app.renderer);
      bonus_drawbonuses(arena.bonuses, arena.bonuscount, app.renderer);
    }

    if(app.gamestate == gsRunning)
    {
      drawBackground(&app, &arena, &player, &f);
      drawArenaText(&app, &arena, &player, hi);
      arena_moveBricks(&arena, &ball);
      arena_drawBricks(&arena, app.renderer);

      // draw the warp area on the right
      if(player.warpenabled == true)
        a_drawsprite(&warp, app.renderer, 560, arena.bounds.bottom - 80);

      // Move the ball, check for collisions with bat, arena, and bricks
      // In the event of losing the ball, reset the level
      // Check if it was already set (by keypress)
      if(currentlywarping == 0)
        if(arena.remaining > 0)
          islostball = ball_moveBall(&ball, &arena, &player) || islostball;

      if(1 == islostball)
      {
        // What we really want to do here is decrease the lives
        // remove any bonuses, and THEN show the "Get Ready!" text
        // Which means the delay needs to come after the next
        // renderpresent
        af_playsample(&f, "dead");
        while(Mix_Playing(-1));
        player.lives--;
        if(player.lives >= 0)
        {
          // Lives already reset, but ideally we want to delay
          // the delay() call until after the next render
          reset(&app, &ball, &player, &arena, &app.gamestate);
          delay = 3000;
        }
        else
        {
          gameOver(&app, &player, &app.gamestate, &hi);
          arena_resetBricks(&arena);
        }
      }

      bonus_drawbonuses(arena.bonuses, arena.bonuscount, app.renderer);


      Vector* cats = arena.levels[arena.level - 1].cats;
      Cat** kittens = (Cat**)(cats->elements);

      cat_move(kittens, cats->size, arena.bricks, arena.brickcount, &arena.bounds);
      cat_draw(kittens, cats->size, app.renderer);
      cat_spawn(kittens, cats->size, &f);
      cat_collidesball(kittens, cats->size, &ball, &f);
      cat_collidesbat(kittens, cats->size, &((Bounds){ .left = player.x, .top = player.y, .width = player.w, .height = player.h }), &f);

      if(arena.remaining == 0)
      {
        // We rely on the onlevelend event at the point remaining is decremented in order to set up the explosions
        if(arena.level == arena.numlevels)
        {
          bool allfinished = arena_drawExplosions(&arena, app.renderer);
          arena.alpha -= arena.alpha < 3 ? arena.alpha : 3;
          text_drawText(&app, "VICTORY!", 0, 275, (SDL_Color){255,255,255,255}, TEXT_ARENA_CENTRED | TEXT_SHADOW, fnTitle);
          if(allfinished)
          {
            gameOver(&app, &player, &app.gamestate, &hi);
            arena_resetBricks(&arena);
          }
        }
        else
        {
          arena.level++;
          app.gamestate = gsNewLevel;
        }
        // See the note above SDL_RenderPresent (below)
        //continue;
      }

      // Move the bat, check we're within the arena
      currentlywarping = bat_movebat(&player, arena.bounds);
      if(currentlywarping == 0)
      {
        bonus_movebonuses(&arena.bonuses, &arena.bonuscount, arena.bounds);
        arena_moveBullets(&arena);
      }
      else
      {
        if(!Mix_Playing(-1))
          af_playsample(&f, "warp");

        // Don't add the scores for the remaining bricks in one go,
        // but add them incrementally for a nice "ticking up" effect.
        if(arena.remaining > 0)
        {
          player.score += BRICKSCORE;
          arena.remaining--;
        }

        if((player.x > arena.bounds.right + 20) && (arena.remaining == 0))
        {
          arena.remaining = 0;
          player.targetspeed = 0;
        }
      }
      arena_checkBulletCollisions(&arena, &player);
      arena_batCollidesBonus(&arena, &player, &ball);

    } // This one is an else because we need one loop between
      // change of app.gamestate for the Get Ready text to render.

    if(app.gamestate == gsGetReady)
    {
      currentlywarping = 0;
      af_playsample(&f, "getready");
      delay = 3000;
      app.gamestate = gsRunning;
      arena.counter = startticks;
    }

    if(app.gamestate == gsDying)
    {
      app.gamestate = gsTitle;
      delay = 3000;
    }

	  if((app.gamestate == gsRunning) || (app.gamestate == gsPaused) || (app.gamestate == gsNewLevel) || (app.gamestate == gsGetReady))
	  {

      drawLives(&app, &player, arena.factory);
      // Draw the ball
      if((ball.cy - ball.radius) < (arena.bounds.bottom - 15))
      {

        ball_drawBall(&ball, app.renderer);
      }

      // Draw the bat
      bat_drawbat(&player, app.renderer, arena.bounds);

      arena_drawBullets(&arena, app.renderer);
    }

    // Display everything on the screen
    // There appears to be a bug in SDL whereby calling SDL_RenderClear or perhaps
    // the SDL drawing functions without calling SDL_RenderPresent causes some memory leaks.
    SDL_RenderPresent(app.renderer);

    Uint32 endticks = SDL_GetTicks();
		// Fix refresh rate as 60fps
    // This means we need to spend 16.6667 miliseconds per frame,
    // so if less time than that has expired, delay for the remainder.
    if(endticks - startticks < 16.6667)
      SDL_Delay(floor(16.6667 - (endticks - startticks)));

    if(delay > 0)
      SDL_Delay(delay);
  }

  // Exiting the program, so free all allocated memory

  config_save();

  menu_free(&menu);
  bonus_freebonuses(&arena.bonuses, &arena.bonuscount);
  arena_freeLevels(&arena);
  arena_freeBullets(&arena);

  af_freesamples(&f);
  af_freeanimations(&f);

  SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);

	Mix_FreeMusic(app.music);
	Mix_CloseAudio();

	text_freeFonts(&app);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
