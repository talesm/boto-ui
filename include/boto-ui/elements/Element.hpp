#ifndef BOTO_ELEMENT_HPP_
#define BOTO_ELEMENT_HPP_

#include "core/Target.hpp"
#include "presenters/ElementPresenter.hpp"

namespace boto {

template<class ELEMENT = Element>
void
present(Target& target, const SDL_Rect& r, StatusFlags status)
{
  return present(
    target.displayList(), r, elementStyle<ELEMENT>(target.theme(), status));
}

/**
 * @brief A stylizable box
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param r the box local position and size
 * @param style
 */
template<class STYLE>
inline void
element(Target target,
        std::string_view id,
        const SDL_Rect& r,
        RequestEvent req,
        STYLE style)
{
  auto el = target.element(id, r, req);
  present(target.displayList(), el.rect, style);
}
template<class STYLE>
inline void
element(Target target, std::string_view id, const SDL_Rect& r, STYLE style)
{
  element(target, id, r, RequestEvent::HOVER, style);
}
template<class STYLE>
inline void
element(Target target, const SDL_Rect& r, STYLE style)
{
  element(target, {}, r, RequestEvent::HOVER, style);
}
template<class ELEMENT = Element>
inline void
element(Target target, std::string_view id, const SDL_Rect& r, RequestEvent req)
{
  auto el = target.element(id, r, req);
  present<ELEMENT>(target, el.rect, el.status);
}
template<class ELEMENT = Element>
inline void
element(Target target, std::string_view id, RequestEvent req)
{
  auto& style = elementStyle<ELEMENT>(target.theme());
  auto sz = measure(style);
  return element(target, id, {0, 0, sz.x, sz.y}, req);
}

} // namespace boto

#endif // BOTO_ELEMENT_HPP_
