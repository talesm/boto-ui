#ifndef BOTO_THEMES_STEELBLUE_TEXTSTYLE_HPP_
#define BOTO_THEMES_STEELBLUE_TEXTSTYLE_HPP_

#include "Font.hpp"
#include "Theme.hpp"
#include "core/DisplayList.hpp"

namespace boto {

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

constexpr TextStyle
adjustDefaultFont(TextStyle style, const Font& font)
{
  if (!style.font) {
    style.font = font;
  }
  return style;
}

inline SDL_Point
presentCharacter(DisplayList& dList,
                 char ch,
                 const SDL_Point& p,
                 Status status,
                 const TextStyle& style)
{
  auto& font = style.font;
  auto adv = measure(ch, font, style.scale);
  SDL_Rect dstRect{p.x, p.y, adv.x, adv.y};
  SDL_Rect srcRect{(ch % font.cols) * font.charW,
                   (ch / font.cols) * font.charH,
                   font.charW,
                   font.charH};
  dList.push(dstRect, style.color, SDL_BLENDMODE_BLEND, font.texture, srcRect);
  return adv;
}

inline SDL_Point
presentText(DisplayList& dList,
            std::string_view str,
            SDL_Point p,
            Status status,
            const TextStyle& style)
{
  SDL_Point adv = {0};
  for (auto ch : str) {
    auto chAdv = presentCharacter(dList, ch, {p.x + adv.x, p.y}, status, style);
    p.x += chAdv.x;
    if (chAdv.y > p.y) {
      p.y = chAdv.y;
    }
  }
  return adv;
}

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
} // namespace boto

#endif // BOTO_TEXTSTYLE_HPP_
