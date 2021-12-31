#ifndef BOTO_CONTROLSTYLE_HPP_
#define BOTO_CONTROLSTYLE_HPP_

#include <SDL.h>
#include "ElementStyle.hpp"
#include "TextStyle.hpp"
#include "core/Theme.hpp"

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
  constexpr ControlStyle withBackground(SDL_Color c) const
  {
    return withDecoration(decoration.withBackground(c));
  }
  constexpr ControlStyle withBorder(SDL_Color borderColor) const
  {
    return withDecoration(decoration.withBorder(borderColor));
  }
  constexpr ControlStyle withLeft(SDL_Color color) const
  {
    return withDecoration(decoration.withLeft(color));
  }
  constexpr ControlStyle withTop(SDL_Color color) const
  {
    return withDecoration(decoration.withTop(color));
  }
  constexpr ControlStyle withRight(SDL_Color color) const
  {
    return withDecoration(decoration.withRight(color));
  }
  constexpr ControlStyle withBottom(SDL_Color color) const
  {
    return withDecoration(decoration.withBottom(color));
  }
  constexpr ControlStyle invertBorders() const
  {
    return withDecoration(decoration.invertBorders());
  }
  constexpr ControlStyle withBorderSize(EdgeSize border) const
  {
    return withDecoration(decoration.withBorderSize(border));
  }
  constexpr operator ElementStyle() const { return decoration; }
  constexpr operator TextStyle() const { return text; }
};

struct Control;

template<>
struct StyleFor<SteelBlue, Control>
{
  static ControlStyle get(Theme& theme)
  {
    return {
      theme.of<Element>(),
      EdgeSize::all(1),
      theme.of<Text>(),
    };
  }
};

} // namespace boto

#endif // BOTO_CONTROLSTYLE_HPP_
