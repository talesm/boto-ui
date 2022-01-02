#ifndef BOTO_ELEMENTS_TEXT_HPP_
#define BOTO_ELEMENTS_TEXT_HPP_

#include <SDL.h>
#include "presenters/TextPresenter.hpp"

namespace boto {

template<class ELEMENT = Text, class STR>
SDL_Point
presentText(Target& target, STR str, const SDL_Point& p, StatusFlags status)
{
  return presentText(target.displayList(),
                     str,
                     p,
                     elementStyle<ELEMENT>(target.theme(), status));
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
template<class ELEMENT = Text>
inline EventTargetState
text(Target target,
     std::string_view id,
     char ch,
     const SDL_Point& p,
     RequestEvent req = RequestEvent::NONE)
{
  auto el = target.element(id, {p.x, p.y, Undefined, Undefined}, req);
  presentText<ELEMENT>(target, ch, {el.rect.x, el.rect.y}, el.status);
  return el;
}
template<class ELEMENT = Text>
inline EventTargetState
text(Target target,
     char ch,
     const SDL_Point& p,
     RequestEvent req = RequestEvent::NONE)
{
  return text<ELEMENT>(target, {}, ch, p, req);
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
template<class ELEMENT = Text>
inline EventTargetState
text(Target target,
     std::string_view id,
     std::string_view str,
     const SDL_Point& p,
     RequestEvent req = RequestEvent::NONE)
{
  auto el = target.element(id, {p.x, p.y, Undefined, Undefined}, req);
  presentText<ELEMENT>(target, str, {el.rect.x, el.rect.y}, el.status);
  return el;
}
template<class ELEMENT = Text>
inline EventTargetState
text(Target target,
     std::string_view str,
     const SDL_Point& p,
     RequestEvent req = RequestEvent::NONE)
{
  return text<ELEMENT>(target, {}, str, p, req);
}
inline void
text(Target target,
     std::string_view str,
     const SDL_Point& p,
     const TextStyle& style)
{
  auto sz = measure(str, style.font, 0);
  auto el = target.element({}, {p.x, p.y, sz.x, sz.y}, RequestEvent::NONE);
  presentText(target.displayList(), str, {el.rect.x, el.rect.y}, style);
}
inline void
text(Target target, std::string_view str, const SDL_Point& p)
{
  return text(target, str, p, target.styleFor<Text>());
}
} // namespace boto

#endif // BOTO_ELEMENTS_TEXT_HPP_
