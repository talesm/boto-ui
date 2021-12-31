#ifndef BOTO_ELEMENT_HPP_
#define BOTO_ELEMENT_HPP_

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
inline void
element(Target target, const SDL_Rect& r, STYLE style)
{
  auto el = target.element({}, r, RequestEvent::HOVER);
  presentElement(target.getDisplayList(), el.rect, style);
}
inline void
element(Target target, const SDL_Rect& r)
{
  element(target, r, target.styleFor<Element>());
}

} // namespace boto

#endif // BOTO_ELEMENT_HPP_
