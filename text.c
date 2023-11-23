#include "text.h"
#include <SDL.h>
#include <SDL_ttf.h>

int text_drawtext(App* app, const char* text, int x, int y, SDL_Color color)
{
  SDL_Color cWhite = {255,255,255};
  SDL_Surface* txt = TTF_RenderText_Solid(app->font, text, cWhite);
  SDL_Texture* tex = SDL_CreateTextureFromSurface(app->renderer, txt);
  SDL_SetTextureColorMod(tex, color.r, color.g, color.b);
  SDL_Rect src = {0, 0, txt->w, txt->h};
  SDL_Rect dst = {x, y, txt->w, txt->h};
  SDL_RenderCopy(app->renderer, tex, &src, &dst);
  SDL_FreeSurface(txt);
  SDL_DestroyTexture(tex);
  return 0;
}
