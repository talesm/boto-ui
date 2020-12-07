#ifndef DUI_BOX_HPP_
#define DUI_BOX_HPP_

#include <SDL.h>
#include "Group.hpp"
#include "theme.hpp"

namespace dui {

/**
 * @brief adds a box element to target
 *
 * @param target the parent group or frame
 * @param rect the box local position and size
 * @param c the box color
 */
inline void
box(Group& target, SDL_Rect rect, SDL_Color c)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto caret = target.getCaret();
  target.advance({rect.x + rect.w, rect.y + rect.h});
  rect.x += caret.x;
  rect.y += caret.y;
  state.display(Shape::Box(rect, c));
}

struct BorderColorStyle
{
  SDL_Color left;
  SDL_Color top;
  SDL_Color right;
  SDL_Color bottom;

  constexpr BorderColorStyle withLeft(SDL_Color c) const
  {
    return {c, top, right, bottom};
  }
  constexpr BorderColorStyle withTop(SDL_Color c) const
  {
    return {left, c, right, bottom};
  }
  constexpr BorderColorStyle withRight(SDL_Color c) const
  {
    return {left, top, c, bottom};
  }
  constexpr BorderColorStyle withBotton(SDL_Color c) const
  {
    return {left, top, right, c};
  }
  constexpr BorderColorStyle invert() const
  {
    return {right, bottom, left, top};
  }

  static constexpr BorderColorStyle all(SDL_Color c) { return {c, c, c, c}; }
};

// Border style
struct BoxStyle
{
  SDL_Color background;
  BorderColorStyle borderColor;
  EdgeSize borderSize;

  constexpr BoxStyle withBackground(SDL_Color background) const
  {
    return {background, borderColor, borderSize};
  }
  constexpr BoxStyle withBorderColor(const BorderColorStyle& borderColor) const
  {
    return {background, borderColor, borderSize};
  }
  constexpr BoxStyle withBorderSize(const EdgeSize& borderSize) const
  {
    return {background, borderColor, borderSize};
  }
};

struct Box;

namespace style {
/// Default panel style
template<>
struct FromTheme<Box, SteelBlue>
{
  constexpr static BoxStyle get()
  {
    return {
      {219, 228, 240, 240},
      BorderColorStyle::all({45, 72, 106, 255}),
      EdgeSize::all(1),
    };
  }
};
}

// A box with colored border
inline void
borderedBox(Group& target,
            const SDL_Rect& r,
            const BoxStyle& style = themeFor<Box>())
{
  auto c = style.background;
  auto e = style.borderColor.right;
  auto n = style.borderColor.top;
  auto w = style.borderColor.left;
  auto s = style.borderColor.bottom;
  auto esz = style.borderSize.right;
  auto nsz = style.borderSize.top;
  auto wsz = style.borderSize.left;
  auto ssz = style.borderSize.bottom;
  auto g = group(target, {}, {0}, Layout::NONE);
  box(g, {r.x + 1, r.y, r.w - 2, nsz}, {n.r, n.g, n.b, n.a});
  box(g, {r.x, r.y + 1, wsz, r.h - 2}, {w.r, w.g, w.b, w.a});
  box(g, {r.x + 1, r.y + r.h - ssz, r.w - 2, ssz}, {s.r, s.g, s.b, s.a});
  box(g, {r.x + r.w - esz, r.y + 1, esz, r.h - 2}, {e.r, e.g, e.b, e.a});
  box(g,
      {r.x + esz, r.y + nsz, r.w - esz - wsz, r.h - nsz - ssz},
      {c.r, c.g, c.b, c.a});
  g.end();
}

/**
 * @brief adds an texturedBox element to target
 *
 * @param target the parent group or frame
 * @param texture the texture
 * @param rect the box local position and size
 */
inline void
texturedBox(Group& target, SDL_Texture* texture, SDL_Rect rect)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto caret = target.getCaret();
  target.advance({rect.x + rect.w, rect.y + rect.h});
  rect.x += caret.x;
  rect.y += caret.y;
  state.display(Shape::Texture(rect, texture));
}

} // namespace dui

#endif // DUI_BOX_HPP_
