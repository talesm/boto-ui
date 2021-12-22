#ifndef BOTO_BOXSTYLE_HPP_
#define BOTO_BOXSTYLE_HPP_

#include <SDL.h>
#include "EdgeSize.hpp"
#include "Theme.hpp"

namespace boto {

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
struct ElementPaintStyle
{
  SDL_Color background;
  BorderColorStyle border;

  constexpr ElementPaintStyle withBackground(SDL_Color background) const
  {
    return {background, border};
  }
  constexpr ElementPaintStyle withBorder(const BorderColorStyle& border) const
  {
    return {background, border};
  }
};

// Border style
struct ElementStyle
{
  EdgeSize border;
  ElementPaintStyle paint;

  constexpr ElementStyle withBorderSize(const EdgeSize& border) const
  {
    return {border, paint};
  }
  constexpr ElementStyle withPaint(const ElementPaintStyle& paint) const
  {
    return {border, paint};
  }
  constexpr ElementStyle withBackgroundColor(SDL_Color background) const
  {
    return withPaint(paint.withBackground(background));
  }
  constexpr ElementStyle withBorderColor(const BorderColorStyle& border) const
  {
    return withPaint(paint.withBorder(border));
  }
};

struct Element;

namespace style {
/// Default element style
template<>
struct FromTheme<Element, SteelBlue>
{
  constexpr static ElementStyle get()
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
} // namespace boto

#endif // BOTO_BOXSTYLE_HPP_
