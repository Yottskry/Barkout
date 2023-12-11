#ifndef _APP_H_
#define _APP_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdbool.h>

#define SCREENW 800
#define SCREENH 600

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	Mix_Music* music;
} App;

#endif // _APP_H_
