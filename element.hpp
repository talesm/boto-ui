#ifndef DUI_BASIC_WIDGETS_HPP_
#define DUI_BASIC_WIDGETS_HPP_

#include <string_view>
#include "EdgeSize.hpp"
#include "Group.hpp"
#include "box.hpp"
#include "text.hpp"
#include "theme.hpp"

namespace dui {

// Style for element state
struct ElementColorStyle
{
  SDL_Color text;
  SDL_Color background;
  BorderColorStyle border;

  constexpr ElementColorStyle withText(SDL_Color text) const
  {
    return {text, background, border};
  }
  constexpr ElementColorStyle withBackground(SDL_Color background) const
  {
    return {text, background, border};
  }
  constexpr ElementColorStyle withBorder(const BorderColorStyle& border) const
  {
    return {text, background, border};
  }
};

struct ElementStyle
{
  EdgeSize padding;
  EdgeSize border;
  ElementColorStyle paint;

  constexpr ElementStyle withPadding(EdgeSize padding) const
  {
    return {padding, border, paint};
  }
  constexpr ElementStyle withBorder(EdgeSize border) const
  {
    return {padding, border, paint};
  }
  constexpr ElementStyle withPaint(const ElementColorStyle& paint) const
  {
    return {padding, border, paint};
  }

  constexpr ElementStyle withTextColor(SDL_Color c) const
  {
    return {padding, border, paint.withText(c)};
  }
  constexpr ElementStyle withBackgroundColor(SDL_Color c) const
  {
    return {padding, border, paint.withBackground(c)};
  }
  constexpr ElementStyle withBorderColor(
    const BorderColorStyle& borderColor) const
  {
    return {padding, border, paint.withBorder(borderColor)};
  }
};

struct Element;

namespace style {

template<>
struct FromTheme<Element, SteelBlue>
{
  constexpr static ElementStyle get()
  {
    return {
      EdgeSize::all(2),
      EdgeSize::all(0),
      ElementColorStyle{TEXT},
    };
  }
};
} // namespace style

inline SDL_Point
computeSize(std::string_view str, const EdgeSize& edgeSize, const SDL_Point& sz)
{
  if (sz.x != 0 && sz.y != 0) {
    return sz;
  }
  auto clientSz = measure(str);
  auto elementSz = elementSize(edgeSize, clientSz);
  if (sz.x != 0) {
    elementSz.x = sz.x;
  } else if (sz.y != 0) {
    elementSz.y = sz.y;
  }
  return elementSz;
}

inline void
element(Group& target,
        std::string_view str,
        const SDL_Rect& r = {0},
        const ElementStyle& style = themeFor<Element>())
{
  auto offset = style.border + style.padding;
  auto sz = computeSize(str, offset, {r.w, r.h});
  auto g = group(target, {}, {r.x, r.y, sz.x, sz.y}, Layout::NONE);
  text(g, str, {offset.left, offset.top}, style.paint.text);
  borderedBox(g,
              {0, 0, sz.x, sz.y},
              {style.paint.background, style.paint.border, style.border});
  g.end();
}

} // namespace dui

#endif // DUI_BASIC_WIDGETS_HPP_
