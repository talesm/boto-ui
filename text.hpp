#ifndef DUI_TEXT_HPP_
#define DUI_TEXT_HPP_

#include <SDL.h>
#include "Group.hpp"
#include "theme.hpp"

namespace dui {

// Text style
struct TextStyle
{
  SDL_Color color;
  // TODO: Font
};

struct Text;

namespace style {

/// Default text style
template<>
struct FromTheme<Text, SteelBlue>
{
  constexpr static TextStyle get() { return {45, 72, 106, 255}; }
};
}

/// Measure the given character
SDL_Point
measure(char ch)
{
  return {8, 8};
}

/// Measure the given text
SDL_Point
measure(std::string_view text)
{
  return {int(8 * text.size()), 8};
}

/**
 * @brief Adds a character element
 *
 * @param target the parent group or frame
 * @param ch the character
 * @param p the position
 * @param c the color (style::TEXT by default)
 */
inline void
character(Group& target,
          char ch,
          const SDL_Point& p,
          const TextStyle& style = themeFor<Text>())
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto& font = state.getFont();
  SDL_assert(font.texture != nullptr);

  auto caret = target.getCaret();
  target.advance({p.x + font.charW, p.y + font.charH});
  SDL_Rect dstRect{p.x + caret.x, p.y + caret.y, font.charW, font.charH};
  SDL_Rect srcRect{(ch % font.cols) * font.charW,
                   (ch / font.cols) * font.charH,
                   font.charW,
                   font.charH};
  state.display(Shape::Texture(dstRect, font.texture, srcRect, style.color));
}

/**
 * @brief Adds a text element
 *
 * @param target the parent group or frame
 * @param str the text
 * @param p the position
 * @param c the color (style::TEXT by default)
 */
inline void
text(Group& target,
     std::string_view str,
     const SDL_Point& p,
     const TextStyle& style = themeFor<Text>())
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto& font = state.getFont();
  SDL_assert(font.texture != nullptr);

  auto caret = target.getCaret();
  target.advance({p.x + font.charW * int(str.size()), p.y + font.charH});
  SDL_Rect dstRect{p.x + caret.x, p.y + caret.y, font.charW, font.charH};
  for (auto ch : str) {
    SDL_Rect srcRect{(ch % font.cols) * font.charW,
                     (ch / font.cols) * font.charH,
                     font.charW,
                     font.charH};
    state.display(Shape::Texture(dstRect, font.texture, srcRect, style.color));
    dstRect.x += 8;
  }
}
} // namespace dui

#endif // DUI_TEXT_HPP_
