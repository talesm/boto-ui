#ifndef BOTO_ELEMENT_HPP_
#define BOTO_ELEMENT_HPP_

#include <SDL.h>
#include "EdgeSize.hpp"
#include "Group.hpp"
#include "Theme.hpp"
#include "presenters/ElementPresenter.hpp"

namespace boto {

/**
 * @brief adds a colorBox element to target
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param rect the colorBox local position and size
 * @param c the colorBox color
 */
inline void
colorBox(Target target, SDL_Rect rect, SDL_Color c)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto caret = target.getCaret();
  target.advance({rect.x + rect.w, rect.y + rect.h});
  rect.x += caret.x;
  rect.y += caret.y;
  state.display(rect, c);
}

/**
 * @brief adds an texturedBox element to target
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param texture the texture
 * @param rect the box local position and size
 */
inline void
textureBox(Target target, SDL_Texture* texture, SDL_Rect rect)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto caret = target.getCaret();
  target.advance({rect.x + rect.w, rect.y + rect.h});
  rect.x += caret.x;
  rect.y += caret.y;
  state.display(rect, texture, {0, 0, rect.w, rect.h});
}

/**
 * @brief A stylizable box
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param r the box local position and size
 * @param style
 */
inline void
element(Target target,
        const SDL_Rect& r,
        const ElementStyle& style = themeFor<Element>())
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
