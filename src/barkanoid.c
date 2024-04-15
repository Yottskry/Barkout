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
#include <time.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define STARTLIVES 3
#define OPT1 "Ball angle is determined by segment of bat hit"
#define OPT2 "Bat speed and direction influences ball spin and angle"

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
  af_loadanimation(f, renderer, "white.png", "white", 44, 29);
  af_loadanimation(f, renderer, "bg1.png", "bg1", 600, 600);
  af_loadanimation(f, renderer, "bg1_mg.png", "bg1-mg", 600, 600);
  af_loadanimation(f, renderer, "bg1_fg.png", "bg1-fg", 660, 600);
  af_loadanimation(f, renderer, "bg2.png", "bg2", 600, 600);
  af_loadanimation(f, renderer, "bg2_mg.png", "bg2-mg", 600, 600);
  af_loadanimation(f, renderer, "bg2_fg.png", "bg2-fg", 660, 600);
  af_loadanimation(f, renderer, "bg3.png", "bg3", 600, 600);
  af_loadanimation(f, renderer, "bg3_mg.png", "bg3-mg", 600, 600);
  af_loadanimation(f, renderer, "bg3_fg.png", "bg3-fg", 660, 600);
  af_loadanimation(f, renderer, "scores.png", "scores", 200, 600);
  af_loadanimation(f, renderer, "bat.png", "bat", 82, 29);
  af_loadanimation(f, renderer, "ball.png", "ball", 17, 17);
  af_loadanimation(f, renderer, "bat_shrink.png", "bat-shrink", 82, 29);
  af_loadanimation(f, renderer, "bat_laser.png", "bat-laser", 82, 29);
  af_loadanimation(f, renderer, "bat_laserify.png", "bat-laserify", 82, 29);
  af_loadanimation(f, renderer, "bat_grow.png", "bat-grow", 122, 29);
  af_loadanimation(f, renderer, "bonus.png", "bonus-d", 43, 25);
  af_loadanimation(f, renderer, "bonus-s.png", "bonus-s", 43, 25);
  af_loadanimation(f, renderer, "bonus-e.png", "bonus-e", 43, 25);
  af_loadanimation(f, renderer, "bonus-c.png", "bonus-c", 43, 25);
  af_loadanimation(f, renderer, "bonus-p.png", "bonus-p", 43, 25);
  af_loadanimation(f, renderer, "bonus-l.png", "bonus-l", 43, 25);
  af_loadanimation(f, renderer, "bonus-w.png", "bonus-w", 43, 25);
  af_loadanimation(f, renderer, "bat_small.png", "bat-s", 51, 27);
  af_loadanimation(f, renderer, "bat_long.png", "bat-l", 122, 27);
  af_loadanimation(f, renderer, "ball-deadly.png", "ball-deadly", 17, 17);
  //af_loadanimation(f, renderer, "barkanoid-intro.png", "intro", 400, 75);
  af_loadanimation(f, renderer, "barkout-intro.png", "intro", 400, 75);
  af_loadanimation(f, renderer, "life.png", "life", 38, 16);
  af_loadanimation(f, renderer, "warp.png", "warp", 40, 80);
  af_loadanimation(f, renderer, "border.png", "border", 600, 600);
  af_loadanimation(f, renderer, "cat.png", "cat", 40, 40);
  af_loadanimation(f, renderer, "cat-die.png", "cat-die", 40, 40);
  af_loadanimation(f, renderer, "cat-spawn.png", "cat-spawn", 40, 40);
  af_loadanimation(f, renderer, "wormhole2.png", "wormhole", 80, 50);
  af_loadanimation(f, renderer, "white_top.png", "white-top", 44, 29);
  af_loadanimation(f, renderer, "green_bottom.png", "green-bottom", 44, 29);
  af_loadanimation(f, renderer, "grey_broken.png", "grey-broken", 44, 29);
  af_loadanimation(f, renderer, "grey_repair.png", "grey-repair", 44, 29);
  af_loadanimation(f, renderer, "boss.png", "boss", 124, 104);
  af_loadanimation(f, renderer, "FatHorseGames.png", "logo", 100, 100);


  // And some sound
  af_loadsample(f, "barkanoid-getready.wav", "getready");
  af_loadsample(f, "barkanoid-brick.wav", "brick");
  af_loadsample(f, "barkanoid-brick-high.wav", "brick-high");
  af_loadsample(f, "barkanoid-brick-laser.wav", "brick-laser");
  af_loadsample(f, "barkanoid-bat.wav", "bat");
  af_loadsample(f, "barkanoid-dead.wav", "dead");
  af_loadsample(f, "barkanoid-1up.wav", "1up");
  af_loadsample(f, "barkanoid-warp.wav", "warp");
  af_loadsample(f, "barkanoid-hitcat.wav", "cat-hit");
  af_loadsample(f, "barkanoid-laser.wav", "laser");
  af_loadsample(f, "barkanoid-wormhole-in.wav", "wormhole-in");
  af_loadsample(f, "barkanoid-wormhole-out.wav", "wormhole-out");
  af_loadsample(f, "barkanoid-victory.wav", "victory");
  af_loadsample(f, "barkanoid-explosion.wav", "explosion");
}

int gameover(App* app, Arena* arena, Gamestate* gamestate, int* hi)
{
  *gamestate = gsDying;
  text_drawtext(app, "Game Over!", 202, 302, (SDL_Color){0,0,0,255}, 0, fnTitle);
  text_drawtext(app, "Game Over!", 200, 300, (SDL_Color){255,255,255,255}, 0, fnTitle);
  if(arena->score > *hi)
  {
    savehighscore(((int*)&arena->score));
    *hi = arena->score;
  }
  return *hi;
}

// Draw "Get Ready!" text and wait for three seconds
int reset(App* app, Ball* ball, Bat* player, Arena* arena, Gamestate* gamestate)
{
  player->x = arena->width / 2;
  bat_reset(player, arena->factory);
  bonus_freebonuses(&arena->bonuses, &arena->bonuscount);
  arena_freebullets(arena);
  arena->counter = SDL_GetTicks();
  af_setanimation(arena->factory, &(ball->sprite), "ball", 1, NULL, NULL, NULL);
  ball->state = bsSticky;
  ball->cx = player->x + (player->w / 2);
  ball->cy = player->y - (ball->radius * 2) + 2;
  ball->speed = 6;
  ball->warpdest = NULL;
  *gamestate = gsGetReady;
  text_drawtext(app, "Get Ready!", 202, 302, (SDL_Color){0,0,0,255}, 0, fnTitle);
  text_drawtext(app, "Get Ready!", 200, 300, (SDL_Color){255,255,255,255}, 0, fnTitle);
  return 0;
}

int getready(Gamestate* gamestate, Gamestate nextstate)
{
  //SDL_Delay(3000);
  *gamestate = nextstate;
  return 0;
}

void menu_quitclick(void* data)
{
  App* app = (App*)data;
  app->gamestate = gsQuit;
}

void menu_startclick(void* data)
{
  App* app = (App*)data;
  app->gamestate = gsStory;
}

void menu_creditsclick(void* data)
{
  App* app = (App*)data;
  app->gamestate = gsCredits;
}

void menu_howtoplayclick(void* data)
{
  App* app = (App*)data;
  app->gamestate = gsHelp;
}

void menu_fullscreentoggle(void* data, void* item)
{
  App* app = (App*)data;
  MenuItem* menuitem = (MenuItem*)item;

  if(menuitem->selectedvalue == 1)
    SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN);
  else
    SDL_SetWindowFullscreen(app->window, 0);
}

void drawbackground(App* app, Arena* arena, Bat* player, ResourceFactory* factory)
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

void drawarenatext(App* app, Arena* arena, int hi)
{
  text_drawtext(app, "BARKOUT", 612, 22, (SDL_Color){0, 0, 0, 255}, 0, fnTitle);
  text_drawtext(app, "BARKOUT", 610, 20, (SDL_Color){255, 255, 255, 255}, 0, fnTitle);

  char highs[10] = "";

  sprintf(highs, "%08d", hi);

  text_drawtext(app, "Hi Score", 612, 82, (SDL_Color){0,0,0,255}, 0, fnTitle);
  text_drawtext(app, "Hi Score", 610, 80, (SDL_Color){255,255,255,255}, 0, fnTitle);

  text_drawtext(app, highs, 612, 122, (SDL_Color){0, 0, 0, 255}, 0, fnTitle);
  text_drawtext(app, highs, 610, 120, (SDL_Color){255, 255, 255, 255}, 0, fnTitle);

  char scores[10] = "";

  sprintf(scores, "%08d", arena->score);

  text_drawtext(app, "Score", 612, 202, (SDL_Color){0,0,0,255}, 0, fnTitle);
  text_drawtext(app, "Score", 610, 200, (SDL_Color){255,255,255,255}, 0, fnTitle);

  text_drawtext(app, scores, 612, 242, (SDL_Color){0, 0, 0, 255}, 0, fnTitle);
  text_drawtext(app, scores, 610, 240, (SDL_Color){255, 255, 255, 255}, 0, fnTitle);

  char level[4] = "";

  sprintf(level, "%02d", arena->level);

  text_drawtext(app, "Round", 612, 322, (SDL_Color){0,0,0,255}, 0, fnTitle);
  text_drawtext(app, "Round", 610, 320, (SDL_Color){255,255,255,255}, 0, fnTitle);

  text_drawtext(app, level, 742, 322, (SDL_Color){0, 0, 0, 255}, 0, fnTitle);
  text_drawtext(app, level, 740, 320, (SDL_Color){255, 255, 255, 255}, 0, fnTitle);
}

void drawhowtoplace(ResourceFactory* factory, App* app)
{
  int left = 40;
  int top = 30;

  SDL_Color white = {255,255,255,255};

  text_drawtext(app, "How to Play", left, top, white, 0, fnStory);

  char* howtotext = "Use your craft, Maus, to direct "
                    "the energy ball and destroy the "
                    "infrastructure of the evil Cat Empire!";
  top = 75;
  text_drawwrappedtext(app, howtotext, left, top, white, 0, 700, fnStory);

  char* bonustext = "Some of the structure may reveal "
                    "powerful bonuses when destroyed. "
                    "Use them wisely!";

  top = 185;
  text_drawwrappedtext(app, bonustext, left, top, white, 0, 700, fnStory);

  top = 300;
  //text_drawtext(app, "Use them wisely!", left, top, white, 0);
  //top += 40;
  //text_drawtext(app, "Bonuses", left, top, white, 0);
  //top += 50;
  a_drawstaticframe(af_getanimation(factory, "bonus-c"), app->renderer, left, top, 0, 255);
  text_drawwrappedtext(app, "The ball sticks to the Maus", left + 80, top - 5, white, 0, 600, fnBody);
  top += 35;
  a_drawstaticframe(af_getanimation(factory, "bonus-l"), app->renderer, left, top, 0, 255);
  text_drawwrappedtext(app, "Enable the Maus's laser guns", left + 80, top - 5, white, 0, 600, fnBody);
  top += 35;
  a_drawstaticframe(af_getanimation(factory, "bonus-d"), app->renderer, left, top, 0, 255);
  text_drawwrappedtext(app, "Enhance the energy ball", left + 80, top - 5, white, 0, 600, fnBody);
  top += 35;
  a_drawstaticframe(af_getanimation(factory, "bonus-e"), app->renderer, left, top, 0, 255);
  text_drawwrappedtext(app, "Extend the Maus!", left + 80, top - 5, white, 0, 600, fnBody);
  top += 35;
  a_drawstaticframe(af_getanimation(factory, "bonus-s"), app->renderer, left, top, 0, 255);
  text_drawwrappedtext(app, "Shrink the Maus. Avoid this one.", left + 80, top - 5, white, 0, 600, fnBody);
  top += 35;
  a_drawstaticframe(af_getanimation(factory, "bonus-p"), app->renderer, left, top, 0, 255);
  text_drawwrappedtext(app, "Extra player!", left + 80, top - 5, white, 0, 600, fnBody);
  top += 35;
  a_drawstaticframe(af_getanimation(factory, "bonus-w"), app->renderer, left, top, 0, 255);
  text_drawwrappedtext(app, "Warp to the next round", left + 80, top - 5, white, 0, 600, fnBody);
}

void drawcredits(App* app)
{
  int left = 150;

  SDL_Color white = {255,255,255,255};

  text_drawtext(app, "Credits", left, 110, white, 0, fnTitle);
  text_drawtext(app, "_____________________________________", left, 120, white, 0, fnTitle);
  text_drawtext(app, "Programming", left, 160, white, 0, fnTitle);
  text_drawtext(app, "Graphics", left, 200, white, 0, fnTitle);
  text_drawtext(app, "Sound FX", left, 240, white, 0, fnTitle);
  text_drawtext(app, "Music", left, 280, white, 0, fnTitle);

  text_drawtext(app, "Go team!", left, 360, white, 0, fnTitle);

  for(int i = 0; i < 4; i++)
    text_drawtext(app, "Fat Harry", left + 260, 160 + (i*40), white, 0, fnTitle);
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

  config_setfullscreen(false);

	if((flags & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN)
    config_setfullscreen(true);

	Config* config = config_load();

  text_loadfonts(&app);

	app.window = SDL_CreateWindow("Barkout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, flags);
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

	app.music = af_loadmusic("barkanoidiii.mp3");

  app.gamestate = gsTitle;

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
  //player.controlmethod = cmBarkanoid;
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
                  .score = 0,
                  .lives = STARTLIVES,
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

  Menu menu = { .itemcount = 0, .items = NULL, .selectedindex = 0, .optionx = 500, .x = 100, .y = 240, .app=&app };
  MenuItem* item = menu_additem(&menu, "Start game", NULL, menu_startclick, NULL);
  item = menu_additem(&menu, "Control method", (int*)&(config->controlmethod), NULL, NULL);
  menu_additemoption(item, "Barkout", OPT2, (int)cmBarkanoid);
  menu_additemoption(item, "Classic", OPT1, (int)cmClassic);
  item = menu_additem(&menu, "Full screen", (int*)&(config->fullscreen), NULL, menu_fullscreentoggle);
  menu_additemoption(item, "Yes", "Stretch to full screen", 1);
  menu_additemoption(item, "No", "Play windowed", 0);
  menu_additem(&menu, "How to Play", NULL, menu_howtoplayclick, NULL);
  menu_additem(&menu, "Credits", NULL, menu_creditsclick, NULL);
  menu_additem(&menu, "Quit", NULL, menu_quitclick, NULL);

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
      arena_loadbinary(&f, &arena, customlevelfile);
    else
      arena_loadbinary(&f, &arena, apath);
  }
  else
    arena_loadlevels(&arena, &f);

  int hi = loadhighscore();

  Star stars[STARS];
  intro_init(stars);
  //Gamestate gamestate = gsTitle;

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

  Cat cats[3];
  for(int i = 0; i < 3; i++)
    cat_init(&cats[i], &f);

  cats[0].state = csAlive;

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
              arena_addbonus(&arena, 200, 300, boLaser);
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
              menu_previousoption(&menu);
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
              menu_nextoption(&menu);
            }
          break;
          case SDLK_p: app.gamestate = app.gamestate == gsRunning ? gsPaused : gsRunning; break;
          case SDLK_SPACE:
            if((app.gamestate == gsRunning) && (player.state == plLaser))
            {
              af_playsample(&f, "laser");
              arena_addbullet(&arena, &player);
            }

          case SDLK_RETURN:
            if((app.gamestate == gsTitle) || (app.gamestate == gsCredits) || (app.gamestate == gsHelp))
              app.gamestate = gsMenu;
            else if(app.gamestate == gsMenu)
              menu_execute(&menu);
            else if(app.gamestate == gsStory)
            {
              app.gamestate = gsNewLevel;
              arena.lives = STARTLIVES;
              arena.level = startlevel;
              arena.score = 0;
              arena.alpha = 255;
              arena_loadbricks(&arena, arena.level);
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
            printf("Diagnostics: \n");
            printf("ball speed: %d\n", ball.speed);
            printf("ball bearing: %f\n", ball.bearing);
            printf("remaining bricks: %d\n", arena.remaining);
            printf("current level: %d\n", arena.level);
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
        titlefinished = text_drawflashtext(&app, &fathorse, 200, 100, 2);
      }
      intro_drawstars(app.renderer, stars);
      if(titlefinished){
        a_drawsprite(&intro, app.renderer, 200, 160);
        text_drawflashtext(&app, &pressstart, 260, 220, 2);
        //a_drawstaticframe(af_getanimation(&f, "logo"), app.renderer, 700, 500, 0, 255);
      }

      intro_movestars(stars);
    }

    if(app.gamestate == gsMenu)
    {
      titlefinished = true;

      intro_drawstars(app.renderer, stars);
      a_drawsprite(&intro, app.renderer, 200, 160);

      menu_drawmenu(&menu, &app);

      intro_movestars(stars);
      // problem is that on our next loop, if we've changed
      // to gsNewLevel we draw one single frame of the previous
      // level layout
      a_drawstaticframe(af_getanimation(&f, "logo"), app.renderer, 700, 500, 0, 255);
    }

    if((app.gamestate == gsCredits) || (app.gamestate == gsHelp))
    {
      titlefinished = true;
      intro_drawstars(app.renderer, stars);

      if(app.gamestate == gsCredits)
        drawcredits(&app);
      else
        drawhowtoplace(&f, &app);

      intro_movestars(stars);
    }

    if(app.gamestate == gsStory)
    {
      titlefinished = true;
      arena.level = startlevel;
      intro_drawstars(app.renderer, stars);
      a_drawsprite(&intro, app.renderer, 200, 220);
      text_drawflashstory(&app, &story1, &txt1, 300);
      text_drawflashstory(&app, &story2, &txt2, 340);
      if(text_drawflashstory(&app, &story3, &txt3, 380))
        app.gamestate = gsNewLevel;
      intro_movestars(stars);
      // problem is that on our next loop, if we've changed
      // to gsNewLevel we draw one single frame of the previous
      // level layout
    }
    // Again, we want another frame to draw the level
    // before we say "Get Ready!" so we need an else
    // to avoid moving directly from gsStory to gsNewLevel
    // in one loop iteration.
    else if(app.gamestate == gsNewLevel)
    {
      if(Mix_PlayingMusic() != 0)
        Mix_HaltMusic();
      arena_loadbricks(&arena, arena.level);
      drawbackground(&app, &arena, NULL, &f);
      drawarenatext(&app, &arena, hi);
      arena_drawbricks(&arena, app.renderer);
      arena_drawbricks(&arena, app.renderer);
      // Reset immediately changes the state to gsGetReady
      // So this block only executes once
      cats[0].state = csDead;
      cats[1].state = csDead;
      cats[2].state = csDead;
      //baddiecounter = SDL_GetTicks();
      reset(&app, &ball, &player, &arena, &app.gamestate);
    }

    if(app.gamestate == gsPaused)
    {
      drawbackground(&app, &arena, &player, &f);
      drawarenatext(&app, &arena, hi);
      arena_drawbricks(&arena, app.renderer);
      bonus_drawbonuses(arena.bonuses, arena.bonuscount, app.renderer);
    }

    if(app.gamestate == gsRunning)
    {
      drawbackground(&app, &arena, &player, &f);
      drawarenatext(&app, &arena, hi);
      arena_drawbricks(&arena, app.renderer);

      // draw the warp area on the right
      if(player.warpenabled == true)
        a_drawsprite(&warp, app.renderer, 560, arena.bounds.bottom - 80);

      // Move the ball, check for collisions with bat, arena, and bricks
      // In the event of losing the ball, reset the level
      // Check if it was already set (by keypress)
      if(currentlywarping == 0)
        if(arena.remaining > 0)
          islostball = ball_moveball(&ball, &arena, &player) || islostball;

      if(1 == islostball)
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
          reset(&app, &ball, &player, &arena, &app.gamestate);
          delay = 3000;
        }
        else
        {
          gameover(&app, &arena, &app.gamestate, &hi);
          arena_resetbricks(&arena);
        }
      }

      bonus_drawbonuses(arena.bonuses, arena.bonuscount, app.renderer);

      //int alivecount = 0;
      for(int i = 0; i < BADDIECOUNT; i++)
      {
        // Test collisions with bat / ball before testing state
        // if(cat_collidesbat()...

        if(cats[i].state != csDead)
        {
          //alivecount++;
          cat_move(&cats[i], arena.bricks, arena.brickcount, &arena.bounds);
          cat_draw(&cats[i], app.renderer);
        }
      }

      cat_spawn(cats, &f, arena.spawnx, arena.spawny);
      cat_collidesball(cats, &ball, &f);
      cat_collidesbat(cats, &((Bounds){ .left = player.x, .top = player.y, .width = player.w, .height = player.h }), &f);

      if(arena.remaining == 0)
      {
        // We rely on the onlevelend event at the point remaining is decremented in order to set up the explosions
        if(arena.level == arena.numlevels)
        {
          bool allfinished = arena_drawexplosions(&arena, app.renderer);
          arena.alpha -= arena.alpha < 3 ? arena.alpha : 3;
          text_drawtext(&app, "VICTORY!", 0, 275, (SDL_Color){255,255,255,255}, TEXT_ARENA_CENTRED, fnTitle);
          if(allfinished)
          {
            gameover(&app, &arena, &app.gamestate, &hi);
            arena_resetbricks(&arena);
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
        arena_movebullets(&arena);
      }
      else
      {
        if(!Mix_Playing(-1))
          af_playsample(&f, "warp");

        // Don't add the scores for the remaining bricks in one go,
        // but add them incrementally for a nice "ticking up" effect.
        if(arena.remaining > 0)
        {
          arena.score += BRICKSCORE;
          arena.remaining--;
        }

        if((player.x > arena.bounds.right + 20) && (arena.remaining == 0))
        {
          //arena.score += BRICKSCORE * arena.remaining;
          arena.remaining = 0;
          player.targetspeed = 0;
        }
      }
      arena_checkbulletcollisions(&arena);
      arena_batcollidesbonus(&arena, &player, &ball);
      arena_movebricks(&arena);
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

	  //if((app.gamestate != gsTitle) && (app.gamestate != gsMenu) && (app.gamestate != gsStory) && (app.gamestate != gsDying))
	  if((app.gamestate == gsRunning) || (app.gamestate == gsPaused) || (app.gamestate == gsNewLevel) || (app.gamestate == gsGetReady))
	  {

      arena_drawlives(&arena, &app);
      // Draw the ball
      if((ball.cy - ball.radius) < (arena.bounds.bottom - 15))
      {

        ball_drawball(&ball, app.renderer);
      }

      // Draw the bat
      bat_drawbat(&player, app.renderer, arena.bounds);

      arena_drawbullets(&arena, app.renderer);
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

  menu_free(&menu);
  //af_freeanimation(&f, "ball");
  bonus_freebonuses(&arena.bonuses, &arena.bonuscount);
  arena_freelevels(&arena);
  arena_freebullets(&arena);

  af_freesamples(&f);
  af_freeanimations(&f);

  SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);

	Mix_FreeMusic(app.music);
	Mix_CloseAudio();

	text_freefonts(&app);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
