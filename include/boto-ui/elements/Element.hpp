#ifndef BOTO_ELEMENT_HPP_
#define BOTO_ELEMENT_HPP_

#include "core/Target.hpp"
#include "presenters/ElementPresenter.hpp"

namespace boto {
template<class ELEMENT = Element>
const auto&
elementStyle(State& state, StatusFlags status)
{
  return elementStyle<ELEMENT>(state.theme, status);
}
template<class ELEMENT = Element>
const auto&
elementStyle(Target& target, StatusFlags status)
{
  return elementStyle<ELEMENT>(*target.state, status);
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
element(Target target, const SDL_Rect& r, STYLE style)
{
  auto el = target.element({}, r, RequestEvent::HOVER);
  presentElement(target.getDisplayList(), el.rect, style);
}
template<class ELEMENT = Element>
inline EventTargetState
element(Target target, std::string_view id, const SDL_Rect& r, RequestEvent req)
{
  auto el = target.element(id, r, req);
  presentElement(
    target.getDisplayList(), el.rect, elementStyle<ELEMENT>(target, el.status));
  return el;
}
template<class ELEMENT = Element>
inline EventTargetState
element(Target target,
        const SDL_Rect& r,
        RequestEvent req = RequestEvent::HOVER)
{
  return element<ELEMENT>(target, {}, r, req);
}

} // namespace boto

#endif // BOTO_ELEMENT_HPP_
