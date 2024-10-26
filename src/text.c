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


#include "text.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void text_drawText(App* app, const char* text, int x, int y, SDL_Color color, int flags, FontType fonttype)
{
  SDL_Color cWhite = {255,255,255,255};
  SDL_Surface* txt = TTF_RenderUTF8_Solid(app->font[fonttype], text, cWhite);

  assert(txt != NULL);

  SDL_Texture* tex = SDL_CreateTextureFromSurface(app->renderer, txt);

  assert(tex != NULL);

  int xpos = x;

  if((flags & TEXT_CENTRED) == TEXT_CENTRED)
    xpos = (int)((SCREENW - txt->w) / 2);
  else if ((flags & TEXT_ARENA_CENTRED) == TEXT_ARENA_CENTRED)
    xpos = (int)((ARENAW - txt->w) / 2) + 40;

  if((flags & TEXT_SHADOW) == TEXT_SHADOW)
  {
    SDL_SetTextureColorMod(tex, 0, 0, 0);
    SDL_SetTextureAlphaMod(tex, color.a);
    SDL_Rect src = {0, 0, txt->w, txt->h};
    SDL_Rect dst = {xpos+2, y+2, txt->w, txt->h};
    SDL_RenderCopy(app->renderer, tex, &src, &dst);
  }

  SDL_SetTextureColorMod(tex, color.r, color.g, color.b);
  SDL_SetTextureAlphaMod(tex, color.a);
  SDL_Rect src = {0, 0, txt->w, txt->h};
  SDL_Rect dst = {xpos, y, txt->w, txt->h};
  SDL_RenderCopy(app->renderer, tex, &src, &dst);
  SDL_FreeSurface(txt);
  SDL_DestroyTexture(tex);
}

void text_drawWrappedText(App* app, const char* text, int x, int y, SDL_Color color, int flags, int len, FontType fonttype)
{
  SDL_Color cWhite = {255,255,255,255};
  SDL_Surface* txt = TTF_RenderUTF8_Solid_Wrapped(app->font[fonttype], text, cWhite, len);

  assert(txt != NULL);

  SDL_Texture* tex = SDL_CreateTextureFromSurface(app->renderer, txt);

  assert(tex != NULL);

  int xpos = x;

  if((flags & TEXT_CENTRED) == TEXT_CENTRED)
    xpos = (int)((SCREENW - txt->w) / 2);

  SDL_SetTextureColorMod(tex, color.r, color.g, color.b);
  SDL_SetTextureAlphaMod(tex, color.a);
  SDL_Rect src = {0, 0, txt->w, txt->h};
  SDL_Rect dst = {xpos, y, txt->w, txt->h};
  SDL_RenderCopy(app->renderer, tex, &src, &dst);
  SDL_FreeSurface(txt);
  SDL_DestroyTexture(tex);
}

bool text_drawFlashText(App* app, FlashText* text, int x, int y, int speed)
{
  text_drawText(app, text->text, x, y, (SDL_Color){255, 255, 255, text->alpha}, TEXT_CENTRED, text->font);
  if(text->alpha > text->targetalpha)
  {
    // We don't want to overstep the target
    if(abs(text->alpha - text->targetalpha) < speed)
      text->alpha -= abs(text->alpha - text->targetalpha);
    else
      text->alpha -= speed;
  }
  else if(text->alpha < text->targetalpha)
  {
    if(abs(text->alpha - text->targetalpha) < speed)
      text->alpha += abs(text->alpha - text->targetalpha);
    else
      text->alpha += speed;
  }

  if(text->alpha == text->targetalpha)
  {
    text->targetalpha = text->targetalpha == 255 ? 0 : 255;
  }
  return (text->alpha == 0);
}

bool text_drawFlashStory(App* app, FlashStory* story, FlashText* text, int y)
{
  if(story->current == story->count)
    return true;

  text->text = story->texts[story->current];

  if(text_drawFlashText(app, text, 0, y, 1))
  {
    story->current++;
    text->alpha = 0;
    text->targetalpha = 255;
  }

  return false;
}

// private - not in header
static TTF_Font* text_loadfont(const char* filename, int ptsize)
{
  char apath[255] = "./Fonts/";
  #ifdef INSTALLDIR
  if(config_getinstalled())
    strcpy(apath, INSTALLDIR "/Fonts/");
  #endif // INSTALLDIR

  strcat(apath, filename);

  TTF_Font* tmp = TTF_OpenFont(apath, ptsize);

  TEST_LOADED(tmp, filename)

  return tmp;
}

void text_loadFonts(App* app)
{

  app->font[0] = text_loadfont("Nordine.ttf", 32);
  assert(app->font[0] != NULL);
  app->font[1] = text_loadfont("Oxanium-Light.ttf", 26);
  assert(app->font[1] != NULL);
  app->font[2] = text_loadfont("Vectroid.otf", 32);
  assert(app->font[2] != NULL);
  app->font[3] = text_loadfont("Nordine.ttf", 30);
  assert(app->font[3] != NULL);
  app->font[4] = text_loadfont("Oxanium-Light.ttf", 18);
  assert(app->font[4] != NULL);
}

void text_freeFonts(App* app)
{
  for(int i = 0; i < FONTCOUNT; i++)
    TTF_CloseFont(app->font[i]);
}
