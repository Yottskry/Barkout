#include "resourcefactory.h"
#include "arena.h"
#include "app.h"
#include "text.h"
#include "intro.h"
#include "cat.h"
#include "scores.h"
#include "config.h"
#include "menu.h"
#include <time.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
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
  af_loadanimation(f, renderer, "barkanoid-intro.png", "intro", 400, 75);
  af_loadanimation(f, renderer, "life.png", "life", 38, 16);
  af_loadanimation(f, renderer, "warp.png", "warp", 40, 80);
  af_loadanimation(f, renderer, "border.png", "border", 600, 600);
  af_loadanimation(f, renderer, "cat.png", "cat", 40, 40);
  af_loadanimation(f, renderer, "cat-die.png", "cat-die", 40, 40);
  af_loadanimation(f, renderer, "cat-spawn.png", "cat-spawn", 40, 40);
  af_loadanimation(f, renderer, "wormhole2.png", "wormhole", 40, 25);

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
  arena_freebullets(arena);
  arena->counter = SDL_GetTicks();
  af_setanimation(arena->factory, &(ball->sprite), "ball", 1, NULL, NULL, NULL);
  ball->state = bsSticky;
  ball->cx = player->x + (player->w / 2);
  ball->cy = player->y - (ball->radius * 2) + 2;
  ball->speed = 6;
  ball->warpdest = NULL;
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

void menu_creditsclicks(void* data)
{
  App* app = (App*)data;
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
  a_drawstaticframe(arena->bg, app->renderer, 0, 0, 0);
  a_drawstaticframe(arena->mg, app->renderer, ofs, 0, 0);
  a_drawstaticframe(arena->fg, app->renderer, ofs2 - 30, 0, 0);
  a_drawstaticframe(af_getanimation(factory, "scores"), app->renderer, 600, 0, 0);
  a_drawstaticframe(af_getanimation(factory, "border"), app->renderer, 0, 0, 0);
}

void drawarenatext(App* app, Arena* arena, int hi)
{
  text_drawtext(app, "BARKANOID", 612, 22, (SDL_Color){0, 0, 0, 255}, 0);
  text_drawtext(app, "BARKANOID", 610, 20, (SDL_Color){255, 255, 255, 255}, 0);

  char highs[10] = "";

  sprintf(highs, "%08d", hi);

  text_drawtext(app, "Hi Score", 612, 82, (SDL_Color){0,0,0,255}, 0);
  text_drawtext(app, "Hi Score", 610, 80, (SDL_Color){255,255,255,255}, 0);

  text_drawtext(app, highs, 612, 122, (SDL_Color){0, 0, 0, 255}, 0);
  text_drawtext(app, highs, 610, 120, (SDL_Color){255, 255, 255, 255}, 0);

  char scores[10] = "";

  sprintf(scores, "%08d", arena->score);

  text_drawtext(app, "Score", 612, 202, (SDL_Color){0,0,0,255}, 0);
  text_drawtext(app, "Score", 610, 200, (SDL_Color){255,255,255,255}, 0);

  text_drawtext(app, scores, 612, 242, (SDL_Color){0, 0, 0, 255}, 0);
  text_drawtext(app, scores, 610, 240, (SDL_Color){255, 255, 255, 255}, 0);

  char level[4] = "";

  sprintf(level, "%02d", arena->level);

  text_drawtext(app, "Round", 612, 322, (SDL_Color){0,0,0,255}, 0);
  text_drawtext(app, "Round", 610, 320, (SDL_Color){255,255,255,255}, 0);

  text_drawtext(app, level, 742, 322, (SDL_Color){0, 0, 0, 255}, 0);
  text_drawtext(app, level, 740, 320, (SDL_Color){255, 255, 255, 255}, 0);
}

int main(int argc, char** argv)
{
  srand(time(0));

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

	int startlevel = 10;
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

  printf("Start level %d\n", startlevel);

	SDL_SetCursor(SDL_DISABLE);

  app.font = TTF_OpenFont("Nordine.ttf", 32);
	app.window = SDL_CreateWindow("Barkanoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, flags);
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
	app.music = Mix_LoadMUS("./Sounds/barkanoidii.mp3");
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
                  .width = 520,
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
                  .bg = NULL
                };

  Config* config = config_load();

  Menu menu = { .itemcount = 0, .items = NULL, .selectedindex = 0, .optionx = 500, .x = 100, .y = 320, .app=&app };
  MenuItem* item = menu_additem(&menu, "Start game", NULL, menu_startclick);
  item = menu_additem(&menu, "Control method", (int*)&(config->controlmethod), NULL);
  menu_additemoption(item, "Barkanoid", OPT2, (int)cmBarkanoid);
  menu_additemoption(item, "Classic", OPT1, (int)cmClassic);
  menu_additem(&menu, "Credits", NULL, NULL);
  menu_additem(&menu, "Quit", NULL, menu_quitclick);

  arena_loadlevels(&arena, &f);

  int hi = loadhighscore();

  Star stars[STARS];
  intro_init(stars);
  //Gamestate gamestate = gsTitle;

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

  Cat cats[3];
  for(int i = 0; i < 3; i++)
    cat_init(&cats[i], &f);

  cats[0].state = csAlive;

  FlashText txt1 = { .alpha = 0, .targetalpha = 255, .duration = 0 };
  FlashText txt2 = { .alpha = 0, .targetalpha = 255, .duration = 0 };
  FlashText txt3 = { .alpha = 0, .targetalpha = 255, .duration = 0 };
  FlashText fathorse = { .alpha = 0, .targetalpha = 255, .duration = 0, .text = "Fat Horse Games presents" };

  bool titlefinished = false;
  int currentlywarping = 0;

  while(app.gamestate != gsQuit)
  {
    // Compare this to the end of the loop to set the frame rate
    Uint32 startticks = SDL_GetTicks();
    Uint32 delay = 0;

    // If we check the keys here, then in many cases the state
    // will only change here, so we'll draw the correct frame each time
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
              arena_addbonus(&arena, 200, 200, boCatch);
          break;

          case SDLK_z:
          case SDLK_LEFT:
            if(currentlywarping == 0)
              player.targetspeed = -1 * player.maxspeed;
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
            if(app.gamestate == gsTitle)
              app.gamestate = gsMenu;
            else if(app.gamestate == gsMenu)
              menu_execute(&menu);
            else if(app.gamestate == gsStory)
            {
              app.gamestate = gsNewLevel;
              arena.lives = STARTLIVES;
              arena.level = startlevel;
              arena.score = 0;
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
	  SDL_RenderClear(app.renderer);

    if(app.gamestate == gsTitle)
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

    if(app.gamestate == gsMenu)
    {
      titlefinished = true;

      intro_drawstars(app.renderer, stars);
      a_drawsprite(&intro, app.renderer, 200, 220);

      menu_drawmenu(&menu, &app);

      intro_movestars(stars);
      // problem is that on our next loop, if we've changed
      // to gsNewLevel we draw one single frame of the previous
      // level layout
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
      arena_drawbonuses(&arena, app.renderer);
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
      int islostball = 0;
      if(currentlywarping == 0)
        islostball = ball_moveball(&ball, &arena, &player);

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
          gameover(&app, &app.gamestate);
          savehighscore(((int*)&arena.score));
          arena_resetbricks(&arena);
        }
      }

      arena_drawbonuses(&arena, app.renderer);

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
        arena.level++;
        app.gamestate = gsNewLevel;
        // See the note above SDL_RenderPresent (below)
        //continue;
      }

      // Move the bat, check we're within the arena
      currentlywarping = bat_movebat(&player, arena.bounds);
      if(currentlywarping == 0)
      {
        arena_movebonuses(&arena);
        arena_movebullets(&arena);
      }
      else
      {
        if(!Mix_Playing(-1))
          af_playsample(&f, "warp");
        if(player.x > arena.bounds.right + 20)
        {
          arena.remaining = 0;
          player.targetspeed = 0;
        }
      }
      arena_checkbulletcollisions(&arena);
      arena_batcollidesbonus(&arena, &player, &ball);
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

	  if((app.gamestate != gsTitle) && (app.gamestate != gsMenu) && (app.gamestate != gsStory) && (app.gamestate != gsDying))
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
  arena_freebonuses(&arena);
  arena_freelevels(&arena);
  arena_freebullets(&arena);

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
