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
struct ElementPaintStyle
{
  SDL_Color text;
  SDL_Color background;
  BorderColorStyle border;

  constexpr ElementPaintStyle withText(SDL_Color text) const
  {
    return {text, background, border};
  }
  constexpr ElementPaintStyle withBackground(SDL_Color background) const
  {
    return {text, background, border};
  }
  constexpr ElementPaintStyle withBorder(const BorderColorStyle& border) const
  {
    return {text, background, border};
  }

  constexpr operator BoxPaintStyle() const { return {background, border}; }
};

struct ElementStyle
{
  EdgeSize padding;
  EdgeSize border;
  Font font;
  int scale;
  ElementPaintStyle paint;

  constexpr ElementStyle withPadding(EdgeSize padding) const
  {
    return {padding, border, font, scale, paint};
  }
  constexpr ElementStyle withBorder(EdgeSize border) const
  {
    return {padding, border, font, scale, paint};
  }
  constexpr ElementStyle withPaint(const ElementPaintStyle& paint) const
  {
    return {padding, border, font, scale, paint};
  }
  constexpr ElementStyle withFont(const Font& font) const
  {
    return {padding, border, font, scale, paint};
  }
  constexpr ElementStyle withScale(int scale) const
  {
    return {padding, border, font, scale, paint};
  }

  constexpr ElementStyle withText(SDL_Color text) const
  {
    return withPaint(paint.withText(text));
  }
  constexpr ElementStyle withBackgroundColor(SDL_Color c) const
  {
    return withPaint(paint.withBackground(c));
  }
  constexpr ElementStyle withBorderColor(
    const BorderColorStyle& borderColor) const
  {
    return withPaint(paint.withBorder(borderColor));
  }
  constexpr operator BoxStyle() const { return {border, paint}; }
  constexpr operator TextStyle() const { return {font, paint.text, scale}; }
};

struct Element;

namespace style {

template<>
struct FromTheme<Element, SteelBlue>
{
  constexpr static ElementStyle get()
  {
    auto text = themeFor<Text, SteelBlue>();
    return {
      EdgeSize::all(2),
      EdgeSize::all(0),
      text.font,
      text.scale,
      {text.color},
    };
  }
};
} // namespace style

inline SDL_Point
computeSize(std::string_view str,
            const ElementStyle& style,
            const SDL_Point& sz)
{
  if (sz.x != 0 && sz.y != 0) {
    return sz;
  }
  auto clientSz = measure(str, style.font, style.scale);
  auto elementSz = elementSize(style.padding + style.border, clientSz);
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
  auto sz = computeSize(str, style, {r.w, r.h});
  auto g = group(target, {}, {r.x, r.y, sz.x, sz.y}, Layout::NONE);
  text(g, str, {offset.left, offset.top}, style);
  box(g, {0, 0, sz.x, sz.y}, style);
  g.end();
}

} // namespace dui

#endif // DUI_BASIC_WIDGETS_HPP_
