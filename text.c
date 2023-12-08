#include "text.h"
#include <SDL.h>
#include <SDL_ttf.h>

void text_drawtext(App* app, const char* text, int x, int y, SDL_Color color)
{
  SDL_Color cWhite = {255,255,255,255};
  SDL_Surface* txt = TTF_RenderText_Solid(app->font, text, cWhite);
  SDL_Texture* tex = SDL_CreateTextureFromSurface(app->renderer, txt);
  SDL_SetTextureColorMod(tex, color.r, color.g, color.b);
  SDL_SetTextureAlphaMod(tex, color.a);
  SDL_Rect src = {0, 0, txt->w, txt->h};
  SDL_Rect dst = {x, y, txt->w, txt->h};
  SDL_RenderCopy(app->renderer, tex, &src, &dst);
  SDL_FreeSurface(txt);
  SDL_DestroyTexture(tex);
}

void text_drawflashtext(App* app, FlashText* text, int x, int y)
{
  text_drawtext(app, text->text, x, y, (SDL_Color){255, 255, 255, text->alpha});
  if(text->alpha > text->targetalpha)
  {
    // We don't want to overstep the target
    if(abs(text->alpha - text->targetalpha) < 2)
      text->alpha -= abs(text->alpha - text->targetalpha);
    else
      text->alpha -= 2;
  }
  else if(text->alpha < text->targetalpha)
  {
    if(abs(text->alpha - text->targetalpha) < 2)
      text->alpha += abs(text->alpha - text->targetalpha);
    else
      text->alpha += 2;
  }

  if(text->alpha == text->targetalpha)
  {
    text->targetalpha = text->targetalpha == 255 ? 0 : 255;
  }
}
