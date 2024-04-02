#ifndef _APP_H_
#define _APP_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdbool.h>

#define SCREENW 800
#define SCREENH 600
#define ARENAW 520

typedef enum { gsTitle, gsMenu, gsStory, gsCredits, gsHelp, gsNewLevel, gsGetReady, gsRunning, gsDying, gsPaused, gsQuit } Gamestate;

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	Mix_Music* music;
	Gamestate gamestate;
} App;

#endif // _APP_H_
