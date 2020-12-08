#ifndef DUI_TEXTSTYLE_HPP_
#define DUI_TEXTSTYLE_HPP_

#include <SDL.h>
#include "Font.hpp"
#include "theme.hpp"

namespace dui {

// Text style
struct TextStyle
{
  Font font;
  SDL_Color color;
  int scale; // 0: 1x, 1: 2x, 2: 4x, 3: 8x, and so on

  constexpr TextStyle withFont(const Font& font) const
  {
    return {font, color, scale};
  }

  constexpr TextStyle withColor(SDL_Color color) const
  {
    return {font, color, scale};
  }

  constexpr TextStyle withScale(int scale) const
  {
    return {font, color, scale};
  }
};

struct Text;

namespace style {

/// Default text style
template<class Theme>
struct FromTheme<Text, Theme>
{
  constexpr static TextStyle get()
  {
    return {{nullptr, 8, 8, 16}, {45, 72, 106, 255}, 0};
  }
};
}
} // namespace dui

#endif // DUI_TEXTSTYLE_HPP_
