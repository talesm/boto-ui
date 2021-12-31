#ifndef BOTO_THEMES_STEELBLUE_ELEMENTSTYLE_HPP_
#define BOTO_THEMES_STEELBLUE_ELEMENTSTYLE_HPP_

#include <SDL.h>
#include "EdgeSize.hpp"
#include "core/DisplayList.hpp"
#include "core/Status.hpp"
#include "core/Theme.hpp"
#include "elements/presenters/ElementPresenter.hpp"

namespace boto {

// Border style
struct ElementStyle
{
  EdgeSize border;
  SDL_Color background;
  SDL_Color left;
  SDL_Color top;
  SDL_Color right;
  SDL_Color bottom;

  constexpr ElementStyle withBorderSize(const EdgeSize& border) const
  {
    return {border, background, right, bottom, left, top};
  }
  constexpr ElementStyle withBackground(SDL_Color background) const
  {
    return {border, background, left, top, right, bottom};
  }
  constexpr ElementStyle withLeft(SDL_Color left) const
  {
    return {border, background, left, top, right, bottom};
  }
  constexpr ElementStyle withTop(SDL_Color top) const
  {
    return {border, background, left, top, right, bottom};
  }
  constexpr ElementStyle withRight(SDL_Color right) const
  {
    return {border, background, left, top, right, bottom};
  }
  constexpr ElementStyle withBottom(SDL_Color bottom) const
  {
    return {border, background, left, top, right, bottom};
  }
  constexpr ElementStyle invertBorders() const
  {
    return {border, background, right, bottom, left, top};
  }
  constexpr ElementStyle withBorder(const SDL_Color& color) const
  {
    return {border, background, color, color, color, color};
  }
};

inline void
presentElement(DisplayList& dList, const SDL_Rect& r, const ElementStyle& style)
{
  auto c = style.background;
  auto e = style.right;
  auto n = style.top;
  auto w = style.left;
  auto s = style.bottom;
  auto esz = style.border.right;
  auto nsz = style.border.top;
  auto wsz = style.border.left;
  auto ssz = style.border.bottom;
  auto clip = dList.clip(r);
  dList.push({r.x + 1, r.y, r.w - 2, nsz}, n);
  dList.push({r.x, r.y + 1, wsz, r.h - 2}, w);
  dList.push({r.x + 1, r.y + r.h - ssz, r.w - 2, ssz}, s);
  dList.push({r.x + r.w - esz, r.y + 1, esz, r.h - 2}, e);
  dList.push({r.x + esz, r.y + nsz, r.w - esz - wsz, r.h - nsz - ssz}, c);
}

/// Default element style
template<>
struct StyleFor<SteelBlue, Element>
{
  constexpr static ElementStyle get(Theme&)
  {
    return {
      EdgeSize::all(1),
      {219, 228, 240, 240},
      {45, 72, 106, 255},
      {45, 72, 106, 255},
      {45, 72, 106, 255},
      {45, 72, 106, 255},
    };
  }
};
} // namespace boto

#endif // BOTO_THEMES_STEELBLUE_ELEMENTSTYLE_HPP_
