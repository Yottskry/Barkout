#include "text.h"
#include <SDL.h>
#include <SDL_ttf.h>

void text_drawtext(App* app, const char* text, int x, int y, SDL_Color color, int flags)
{
  SDL_Color cWhite = {255,255,255,255};
  SDL_Surface* txt = TTF_RenderUTF8_Solid(app->font, text, cWhite);

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

bool text_drawflashtext(App* app, FlashText* text, int x, int y, int speed)
{
  text_drawtext(app, text->text, x, y, (SDL_Color){255, 255, 255, text->alpha}, TEXT_CENTRED);
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

bool text_drawflashstory(App* app, FlashStory* story, FlashText* text, int y)
{
  if(story->current == story->count)
    return true;

  text->text = story->texts[story->current];

  if(text_drawflashtext(app, text, 0, y, 1))
  {
    story->current++;
    text->alpha = 0;
    text->targetalpha = 255;
  }

  return false;
}
