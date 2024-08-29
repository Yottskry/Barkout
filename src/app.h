#ifndef _APP_H_
#define _APP_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

#define SCREENW 800
#define SCREENH 600
#define ARENAW 520
#define FONTCOUNT 3

#define TEST_ALLOC(V) if(V == 0) \
        { \
          printf("Memory allocation failed\n"); \
          exit(1); \
        }
#define TEST_LOADED(V) if(V == 0) \
        { \
          printf("File load failed\n"); \
          exit(1); \
        }

typedef enum { gsTitle, gsMenu, gsStory, gsCredits, gsHelp, gsDonate, gsNewLevel, gsGetReady, gsRunning, gsDying, gsPaused, gsQuit } Gamestate;

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font[FONTCOUNT];
	Mix_Music* music;
	Gamestate gamestate;
} App;

#endif // _APP_H_
