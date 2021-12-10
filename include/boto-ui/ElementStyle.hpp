#ifndef DUI_ELEMENTSTYLE_HPP_
#define DUI_ELEMENTSTYLE_HPP_

#include <SDL.h>
#include "BoxStyle.hpp"
#include "TextStyle.hpp"
#include "Theme.hpp"

namespace boto {

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

template<class Theme>
struct FromTheme<Element, Theme>
{
  constexpr static ElementStyle get()
  {
    auto text = themeFor<Text, Theme>();
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

} // namespace boto

#endif // DUI_ELEMENTSTYLE_HPP_
