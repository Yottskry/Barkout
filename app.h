#ifndef _APP_H_
#define _APP_H_

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
} App;

#endif // _APP_H_
