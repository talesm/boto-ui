#ifndef BOTO_ELEMENT_HPP_
#define BOTO_ELEMENT_HPP_

#include "Theme.hpp"
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
element(Target target, const SDL_Rect& r, STYLE style = themeFor<Element>())
{
  auto caret = target.getCaret();
  presentElement(target.getDisplayList(),
                 {r.x + caret.x, r.y + caret.y, r.w, r.h},
                 Status::NONE,
                 style);
  target.advance({r.x + r.w, r.y + r.h});
}

} // namespace boto

#endif // BOTO_ELEMENT_HPP_
