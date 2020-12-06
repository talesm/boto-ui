#ifndef DUI_BASIC_WIDGETS_HPP_
#define DUI_BASIC_WIDGETS_HPP_

#include <string_view>
#include "EdgeSize.hpp"
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
  constexpr BorderColorStyle withInvertedBorders() const
  {
    return {right, bottom, left, top};
  }

  static constexpr BorderColorStyle all(SDL_Color c) { return {c, c, c, c}; }
};

// Border style
struct BorderedBoxStyle
{
  SDL_Color background;
  BorderColorStyle borderColor;

  constexpr BorderedBoxStyle withBackground(SDL_Color c) const
  {
    return {c, borderColor};
  }
  constexpr BorderedBoxStyle withBorderColor(
    const BorderColorStyle& borderColor) const
  {
    return {background, borderColor};
  }
};

// A box with colored border
inline void
borderedBox(Group& target, const SDL_Rect& r, const BorderedBoxStyle& style)
{
  auto c = style.background;
  auto e = style.borderColor.right;
  auto n = style.borderColor.top;
  auto w = style.borderColor.left;
  auto s = style.borderColor.bottom;
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
  auto caret = target.getCaret();
  target.advance({p.x + 8, p.y + 8});
  SDL_Rect dstRect{p.x + caret.x, p.y + caret.y, 8, 8};
  SDL_Rect srcRect{(ch % 16) * 8, (ch / 16) * 8, 8, 8};
  state.display(Shape::Texture(dstRect, state.getFont(), srcRect, c));
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
     const SDL_Point& p,
     SDL_Color c = style::TEXT)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto caret = target.getCaret();
  target.advance({p.x + 8 * int(str.size()), p.y + 8});
  SDL_Rect dstRect{p.x + caret.x, p.y + caret.y, 8, 8};
  for (auto ch : str) {
    SDL_Rect srcRect{(ch % 16) * 8, (ch / 16) * 8, 8, 8};
    state.display(Shape::Texture(dstRect, state.getFont(), srcRect, c));
    dstRect.x += 8;
  }
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

#endif // DUI_BASIC_WIDGETS_HPP_
