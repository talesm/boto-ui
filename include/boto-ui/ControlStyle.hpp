#ifndef BOTO_CONTROLSTYLE_HPP_
#define BOTO_CONTROLSTYLE_HPP_

#include <SDL.h>
#include "BoxStyle.hpp"
#include "TextStyle.hpp"
#include "Theme.hpp"

namespace boto {

// Style for control state
struct ControlPaintStyle
{
  SDL_Color text;
  SDL_Color background;
  BorderColorStyle border;

  constexpr ControlPaintStyle withText(SDL_Color text) const
  {
    return {text, background, border};
  }
  constexpr ControlPaintStyle withBackground(SDL_Color background) const
  {
    return {text, background, border};
  }
  constexpr ControlPaintStyle withBorder(const BorderColorStyle& border) const
  {
    return {text, background, border};
  }

  constexpr operator ElementPaintStyle() const { return {background, border}; }
};

struct ControlStyle
{
  EdgeSize padding;
  EdgeSize border;
  Font font;
  int scale;
  ControlPaintStyle paint;

  constexpr ControlStyle withPadding(EdgeSize padding) const
  {
    return {padding, border, font, scale, paint};
  }
  constexpr ControlStyle withBorder(EdgeSize border) const
  {
    return {padding, border, font, scale, paint};
  }
  constexpr ControlStyle withPaint(const ControlPaintStyle& paint) const
  {
    return {padding, border, font, scale, paint};
  }
  constexpr ControlStyle withFont(const Font& font) const
  {
    return {padding, border, font, scale, paint};
  }
  constexpr ControlStyle withScale(int scale) const
  {
    return {padding, border, font, scale, paint};
  }

  constexpr ControlStyle withText(SDL_Color text) const
  {
    return withPaint(paint.withText(text));
  }
  constexpr ControlStyle withBackgroundColor(SDL_Color c) const
  {
    return withPaint(paint.withBackground(c));
  }
  constexpr ControlStyle withBorderColor(
    const BorderColorStyle& borderColor) const
  {
    return withPaint(paint.withBorder(borderColor));
  }
  constexpr operator ElementStyle() const { return {border, paint}; }
  constexpr operator TextStyle() const { return {font, paint.text, scale}; }
};

struct Control;

namespace style {

template<class Theme>
struct FromTheme<Control, Theme>
{
  constexpr static ControlStyle get()
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

#endif // BOTO_CONTROLSTYLE_HPP_
