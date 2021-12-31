#ifndef BOTO_ELEMENTS_TEXTPRESENTER_HPP_
#define BOTO_ELEMENTS_TEXTPRESENTER_HPP_

#include <string_view>
#include "ElementPresenter.hpp"
#include "Font.hpp"
#include "core/Theme.hpp"

namespace boto {

struct TextColor;

template<class THEME>
struct StyleFor<THEME, TextColor>
{
  constexpr static SDL_Color get(ThemeT<THEME>& t)
  {
    return t.template of<DefaultColor>();
  }
};
struct TextScale;

template<class THEME>
struct StyleFor<THEME, TextScale>
{
  constexpr static int get(ThemeT<THEME>& theme) { return 0; }
};

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

template<class THEME>
struct StyleFor<THEME, Text>
{
  static TextStyle get(ThemeT<THEME>& theme)
  {
    return {
      theme.template of<Font>(),
      theme.template of<TextColor>(),
      theme.template of<TextScale>(),
    };
  }
};

inline SDL_Point
measureText(std::string_view str, const TextStyle& style)
{
  return measure(str, style.font, style.scale);
}
inline SDL_Point
measureText(char ch, const TextStyle& style)
{
  return measure(ch, style.font, style.scale);
}
inline SDL_Point
presentText(DisplayList& dList,
            char ch,
            const SDL_Point& p,
            const TextStyle& style)
{
  auto& font = style.font;
  auto adv = measure(font, style.scale);
  SDL_Rect dstRect{p.x, p.y, adv.x, adv.y};
  SDL_Rect srcRect{
    (ch % font.cols) * font.charW,
    (ch / font.cols) * font.charH,
    font.charW,
    font.charH,
  };
  presentElement(
    dList, dstRect, PartialTextureStyle{font.texture, style.color, srcRect});
  return adv;
}

inline SDL_Point
presentText(DisplayList& dList,
            std::string_view str,
            const SDL_Point& p,
            const TextStyle& style)
{
  auto& font = style.font;
  SDL_Point adv{0, measure(font, style.scale).y};
  for (auto ch : str) {
    adv.x += presentText(dList, ch, {p.x + adv.x, p.y}, style).x;
  }
  return {adv.x, adv.y};
}

} // namespace boto

#endif // BOTO_ELEMENTS_TEXTPRESENTER_HPP_
