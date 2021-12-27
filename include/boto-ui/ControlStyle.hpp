#ifndef BOTO_CONTROLSTYLE_HPP_
#define BOTO_CONTROLSTYLE_HPP_

#include <SDL.h>
#include "Theme.hpp"

namespace boto {

struct ControlStyle
{
  ElementStyle decoration;
  EdgeSize padding;
  TextStyle text;

  constexpr ControlStyle withDecoration(const ElementStyle& decoration) const
  {
    return {decoration, padding, text};
  }
  constexpr ControlStyle withPadding(EdgeSize padding) const
  {
    return {decoration, padding, text};
  }
  constexpr ControlStyle withText(const TextStyle& text) const
  {
    return {decoration, padding, text};
  }
  constexpr ControlStyle withBorder(EdgeSize border) const
  {
    return withDecoration(decoration.withBorderSize(border));
  }
  constexpr ControlStyle withFont(const Font& font) const
  {
    return withText(text.withFont(font));
  }
  constexpr ControlStyle withScale(int scale) const
  {
    return withText(text.withScale(scale));
  }

  constexpr ControlStyle withText(SDL_Color color) const
  {
    return withText(text.withColor(color));
  }
  constexpr ControlStyle withBackgroundColor(SDL_Color c) const
  {
    return withDecoration(decoration.withBackgroundColor(c));
  }
  constexpr ControlStyle withBorderColor(
    const BorderColorStyle& borderColor) const
  {
    return withDecoration(decoration.withBorderColor(borderColor));
  }
  constexpr ControlStyle withBorderSize(EdgeSize border) const
  {
    return withDecoration(decoration.withBorderSize(border));
  }
  constexpr operator ElementStyle() const { return decoration; }
  constexpr operator TextStyle() const { return text; }
};

struct Control;

namespace style {

template<class Theme>
struct FromTheme<Control, Theme>
{
  constexpr static ControlStyle get()
  {
    return {
      themeFor<Element, Theme>(),
      EdgeSize::all(1),
      themeFor<Text, Theme>(),
    };
  }
};
} // namespace style

} // namespace boto

#endif // BOTO_CONTROLSTYLE_HPP_
