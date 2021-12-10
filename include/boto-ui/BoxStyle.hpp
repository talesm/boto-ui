#ifndef DUI_BOXSTYLE_HPP_
#define DUI_BOXSTYLE_HPP_

#include <SDL.h>
#include "EdgeSize.hpp"
#include "Theme.hpp"

namespace dui {

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
/// Default box style
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
} // namespace style
} // namespace dui

#endif // DUI_BOXSTYLE_HPP_
