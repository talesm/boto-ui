#ifndef BOTO_FONT_HPP
#define BOTO_FONT_HPP

#include <string_view>
#include <SDL.h>
#include "core/Theme.hpp"

namespace boto {

struct Font
{
  SDL_Texture* texture;
  int charW, charH;
  int cols;

  constexpr operator bool() const { return bool(texture); }
};

/// Measure the given character
constexpr SDL_Point
measure(const Font& font, int scale = 0)
{
  return {font.charW << scale, font.charH << scale};
}

constexpr SDL_Point
measure(char ch, const Font& font, int scale = 0)
{
  return {font.charW << scale, font.charH << scale};
}

/// Measure the given text
constexpr SDL_Point
measure(std::string_view text, const Font& font, int scale = 0)
{
  return {int((font.charW << scale) * text.size()), font.charH << scale};
}

#include "defaultFont.h"

inline Font
loadDefaultFont(SDL_Renderer* renderer)
{
  SDL_RWops* src = SDL_RWFromConstMem(font_bmp, font_bmp_len);
  SDL_Surface* surface = SDL_LoadBMP_RW(src, 1);
  SDL_SetColorKey(surface, 1, 0);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return {texture, 8, 8, 16};
}

template<class THEME>
struct StyleTypeT<THEME, Font>
{
  using type = Font;
};

} // namespace boto

#endif
