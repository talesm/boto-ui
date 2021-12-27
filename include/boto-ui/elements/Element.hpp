#ifndef BOTO_ELEMENT_HPP_
#define BOTO_ELEMENT_HPP_

#include "Theme.hpp"
#include "core/Target.hpp"
#include "presenters/ElementPresenter.hpp"

namespace boto {

/**
 * @brief A stylizable box
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param r the box local position and size
 * @param style
 */
template<class STYLE>
inline const EventTargetState&
element(Target target, const SDL_Rect& r, STYLE style = themeFor<Element>())
{
  auto& el = target.check({}, r, RequestEvent::HOVER);
  presentElement(target.getDisplayList(), el.rect, el.status, style);
  return el;
}

} // namespace boto

#endif // BOTO_ELEMENT_HPP_
