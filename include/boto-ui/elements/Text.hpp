#ifndef BOTO_ELEMENTS_TEXT_HPP_
#define BOTO_ELEMENTS_TEXT_HPP_

#include <SDL.h>
#include "Group.hpp"
#include "Theme.hpp"

namespace boto {

/// Measure the given character
constexpr SDL_Point
measure(char ch, const Font& font, int scale)
{
  return {font.charW << scale, font.charH << scale};
}

/// Measure the given text
constexpr SDL_Point
measure(std::string_view text, const Font& font, int scale)
{
  return {int((font.charW << scale) * text.size()), font.charH << scale};
}

/**
 * @brief Adds a character element
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param ch the character
 * @param p the position
 * @param style
 */
inline void
character(Target target,
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
  SDL_Rect dstRect{p.x + caret.x,
                   p.y + caret.y,
                   font.charW << style.scale,
                   font.charH << style.scale};
  target.advance({p.x + dstRect.w, p.y + dstRect.h});
  SDL_Rect srcRect{(ch % font.cols) * font.charW,
                   (ch / font.cols) * font.charH,
                   font.charW,
                   font.charH};
  state.display(dstRect, font.texture, srcRect, style.color);
}

/**
 * @brief Adds a text element
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param str the text
 * @param p the position
 * @param style
 */
inline void
text(Target target,
     std::string_view str,
     const SDL_Point& p,
     const TextStyle& style = themeFor<Text>())
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto font = style.font.texture ? style.font : state.getFont();
  SDL_assert(font.texture != nullptr);

  auto caret = target.getCaret();
  SDL_Rect dstRect{p.x + caret.x,
                   p.y + caret.y,
                   font.charW << style.scale,
                   font.charH << style.scale};
  target.advance({p.x + dstRect.w * int(str.size()), p.y + dstRect.h});
  for (auto ch : str) {
    SDL_Rect srcRect{(ch % font.cols) * font.charW,
                     (ch / font.cols) * font.charH,
                     font.charW,
                     font.charH};
    state.display(dstRect, font.texture, srcRect, style.color);
    dstRect.x += dstRect.w;
  }
}
} // namespace boto

#endif // BOTO_ELEMENTS_TEXT_HPP_
