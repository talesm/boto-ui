#ifndef DUI_TEXT_HPP_
#define DUI_TEXT_HPP_

#include <SDL.h>
#include "Group.hpp"

namespace dui {

namespace style {
/// Default text style
constexpr SDL_Color TEXT{45, 72, 106, 255};
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
character(Group& target, char ch, const SDL_Point& p, SDL_Color c = style::TEXT)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto caret = target.getCaret();
  target.advance({p.x + 8, p.y + 8});
  SDL_Rect dstRect{p.x + caret.x, p.y + caret.y, 8, 8};
  SDL_Rect srcRect{(ch % 16) * 8, (ch / 16) * 8, 8, 8};
  state.display(Shape::Texture(dstRect, state.getFont(), srcRect, c));
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
     SDL_Color c = style::TEXT)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto caret = target.getCaret();
  target.advance({p.x + 8 * int(str.size()), p.y + 8});
  SDL_Rect dstRect{p.x + caret.x, p.y + caret.y, 8, 8};
  for (auto ch : str) {
    SDL_Rect srcRect{(ch % 16) * 8, (ch / 16) * 8, 8, 8};
    state.display(Shape::Texture(dstRect, state.getFont(), srcRect, c));
    dstRect.x += 8;
  }
}
} // namespace dui

#endif // DUI_TEXT_HPP_
