#ifndef DUI_BOX_HPP_
#define DUI_BOX_HPP_

#include <SDL.h>
#include "Group.hpp"
#include "theme.hpp"

namespace dui {

/**
 * @brief adds a colorBox element to target
 *
 * @param target the parent group or frame
 * @param rect the colorBox local position and size
 * @param c the colorBox color
 */
inline void
colorBox(Group& target, SDL_Rect rect, SDL_Color c)
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

/**
 * @brief adds an texturedBox element to target
 *
 * @param target the parent group or frame
 * @param texture the texture
 * @param rect the box local position and size
 */
inline void
textureBox(Group& target, SDL_Texture* texture, SDL_Rect rect)
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

// Style for element state
struct BoxPaintStyle
{
  SDL_Color background;
  BorderColorStyle border;

  constexpr BoxPaintStyle withBackground(SDL_Color background) const
  {
    return {background, border};
  }
  constexpr BoxPaintStyle withBorder(const BorderColorStyle& border) const
  {
    return {background, border};
  }
};

// Border style
struct BoxStyle
{
  EdgeSize border;
  BoxPaintStyle paint;

  constexpr BoxStyle withBorderSize(const EdgeSize& border) const
  {
    return {border, paint};
  }
  constexpr BoxStyle withPaint(const BoxPaintStyle& paint) const
  {
    return {border, paint};
  }
  constexpr BoxStyle withBackgroundColor(SDL_Color background) const
  {
    return withPaint(paint.withBackground(background));
  }
  constexpr BoxStyle withBorderColor(const BorderColorStyle& border) const
  {
    return withPaint(paint.withBorder(border));
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
      EdgeSize::all(1),
      {
        {219, 228, 240, 240},
        BorderColorStyle::all({45, 72, 106, 255}),
      },
    };
  }
};
}

// A box with colored border
inline void
box(Group& target, const SDL_Rect& r, const BoxStyle& style = themeFor<Box>())
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
  g.end();
}

} // namespace dui

#endif // DUI_BOX_HPP_
