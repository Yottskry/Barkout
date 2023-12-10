#ifndef _APP_H_
#define _APP_H_

#include <SDL.h>
#include <SDL_ttf.h>

#define SCREENW 800
#define SCREENH 600

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
} App;

#endif // _APP_H_
