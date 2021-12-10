#ifndef BOTO_BOX_HPP_
#define BOTO_BOX_HPP_

#include <SDL.h>
#include "BoxStyle.hpp"
#include "EdgeSize.hpp"
#include "Group.hpp"
#include "Theme.hpp"

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
box(Target target, const SDL_Rect& r, const BoxStyle& style = themeFor<Box>())
{
  auto c = style.paint.background;
  auto e = style.paint.border.right;
  auto n = style.paint.border.top;
  auto w = style.paint.border.left;
  auto s = style.paint.border.bottom;
  auto esz = style.border.right;
  auto nsz = style.border.top;
  auto wsz = style.border.left;
  auto ssz = style.border.bottom;
  auto g = group(target, {}, {0}, Layout::NONE);
  colorBox(g, {r.x + 1, r.y, r.w - 2, nsz}, {n.r, n.g, n.b, n.a});
  colorBox(g, {r.x, r.y + 1, wsz, r.h - 2}, {w.r, w.g, w.b, w.a});
  colorBox(g, {r.x + 1, r.y + r.h - ssz, r.w - 2, ssz}, {s.r, s.g, s.b, s.a});
  colorBox(g, {r.x + r.w - esz, r.y + 1, esz, r.h - 2}, {e.r, e.g, e.b, e.a});
  colorBox(g,
           {r.x + esz, r.y + nsz, r.w - esz - wsz, r.h - nsz - ssz},
           {c.r, c.g, c.b, c.a});
}

} // namespace boto

#endif // BOTO_BOX_HPP_
