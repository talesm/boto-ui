#ifndef DUI_BASIC_WIDGETS_HPP_
#define DUI_BASIC_WIDGETS_HPP_

#include <string_view>
#include "Group.hpp"

namespace dui {

namespace style {
/// Default text style
constexpr SDL_Color TEXT{45, 72, 106, 255};
}

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
  target.advance({rect.x + rect.w, rect.y + rect.h});
  auto caret = target.getCaret();
  rect.x += caret.x;
  rect.y += caret.y;
  state.display(Shape::Box(rect, c));
}

// Border style
struct BorderedBoxStyle
{
  SDL_Color center;
  SDL_Color left;
  SDL_Color top;
  SDL_Color right;
  SDL_Color bottom;

  constexpr BorderedBoxStyle withCenter(SDL_Color c) const
  {
    return {c, left, top, right, bottom};
  }
  constexpr BorderedBoxStyle withLeft(SDL_Color c) const
  {
    return {center, c, top, right, bottom};
  }
  constexpr BorderedBoxStyle withTop(SDL_Color c) const
  {
    return {center, left, c, right, bottom};
  }
  constexpr BorderedBoxStyle withRight(SDL_Color c) const
  {
    return {center, left, top, c, bottom};
  }
  constexpr BorderedBoxStyle withBotton(SDL_Color c) const
  {
    return {center, left, top, right, c};
  }
  constexpr BorderedBoxStyle withInvertedBorders() const
  {
    return {center, right, bottom, left, top};
  }
};

// A box with colored border
inline void
borderedBox(Group& target, const SDL_Rect& r, const BorderedBoxStyle& style)
{
  auto c = style.center;
  auto e = style.right;
  auto n = style.top;
  auto w = style.left;
  auto s = style.bottom;
  auto g = group(target, {}, {0}, Layout::NONE);
  box(g, {r.x + 1, r.y, r.w - 2, 1}, {n.r, n.g, n.b, n.a});
  box(g, {r.x, r.y + 1, 1, r.h - 2}, {w.r, w.g, w.b, w.a});
  box(g, {r.x + 1, r.y + r.h - 2 + 1, r.w - 2, 1}, {s.r, s.g, s.b, s.a});
  box(g, {r.x + r.w - 2 + 1, r.y + 1, 1, r.h - 2}, {e.r, e.g, e.b, e.a});
  box(g, {r.x + 1, r.y + 1, r.w - 2, r.h - 2}, {c.r, c.g, c.b, c.a});
  g.end();
}

/// Measure the given character
SDL_Point
measure(char ch)
{
  return {8, 8};
}

/// Measure the given text
SDL_Point
measure(std::string_view text)
{
  return {int(8 * text.size()), 8};
}

/**
 * @brief Adds a character element
 *
 * @param target the parent group or frame
 * @param ch the character
 * @param p the position
 * @param c the color (style::TEXT by default)
 */
inline void
character(Group& target, char ch, const SDL_Point& p, SDL_Color c = style::TEXT)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  target.advance({p.x + 8, p.y + 8});
  auto caret = target.getCaret();
  state.display(Shape::Character({caret.x + p.x, caret.y + p.y}, c, ch));
}

/**
 * @brief Adds a text element
 *
 * @param target the parent group or frame
 * @param str the text
 * @param p the position
 * @param c the color (style::TEXT by default)
 */
inline void
text(Group& target,
     std::string_view str,
     SDL_Point p,
     SDL_Color c = style::TEXT)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  target.advance({p.x + 8 * int(str.size()), p.y + 8});
  auto caret = target.getCaret();
  for (auto ch : str) {
    state.display(Shape::Character({caret.x + p.x, caret.y + p.y}, c, ch));
    p.x += 8;
  }
}
} // namespace dui

#endif // DUI_BASIC_WIDGETS_HPP_
