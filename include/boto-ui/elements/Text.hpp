#ifndef BOTO_ELEMENTS_TEXT_HPP_
#define BOTO_ELEMENTS_TEXT_HPP_

#include <SDL.h>
#include "presenters/TextPresenter.hpp"

namespace boto {

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
character(Target target, char ch, const SDL_Point& p, const TextStyle& style)
{
  auto st = adjustDefaultFont(style, target.getFont());
  auto sz = measure(ch, st.font, 0);
  auto& el = target.check({}, {p.x, p.y, sz.x, sz.y}, RequestEvent::NONE);
  presentCharacter(
    target.getDisplayList(), ch, {el.rect.x, el.rect.y}, Status::NONE, st);
}
inline void
character(Target target, char ch, const SDL_Point& p)
{
  return character(target, ch, p, target.styleFor<Text>());
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
     const TextStyle& style)
{
  auto st = adjustDefaultFont(style, target.getFont());
  auto sz = measure(str, st.font, 0);
  auto& el = target.check({}, {p.x, p.y, sz.x, sz.y}, RequestEvent::NONE);
  presentText(
    target.getDisplayList(), str, {el.rect.x, el.rect.y}, Status::NONE, st);
}
inline void
text(Target target, std::string_view str, const SDL_Point& p)
{
  return text(target, str, p, target.styleFor<Text>());
}
} // namespace boto

#endif // BOTO_ELEMENTS_TEXT_HPP_
