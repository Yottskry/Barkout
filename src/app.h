/*
*
* Barkout - Copyright 2024 Stephen Branley.
*
* This file is part of Barkout.
*
* Barkout is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License,
* or (at your option) any later version.
*
* Barkout is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*
*/


#ifndef _APP_H_
#define _APP_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREENW 800
#define SCREENH 600
#define ARENAW 520
#define FONTCOUNT 4

#define TEST_ALLOC(V) if(V == 0) \
        { \
          printf("Memory allocation failed\n"); \
          exit(1); \
        }
#define TEST_LOADED(V, F) if(V == 0) \
        { \
          printf("File load failed: %s \n", F); \
          exit(1); \
        }

typedef enum { gsTitle, gsMenu, gsStory, gsCredits, gsHelp, gsDonate, gsLostLife, gsNewLevel, gsGetReady, gsRunning, gsDying, gsPaused, gsQuit } Gamestate;

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font[FONTCOUNT];
	Mix_Music* music;
	Gamestate gamestate;
} App;

#endif // _APP_H_
