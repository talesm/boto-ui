#ifndef BOTO_DARKTHEME_HPP_
#define BOTO_DARKTHEME_HPP_

namespace boto {
namespace style {
// Dark theme
struct DarkTheme;

} // namespace style
} // namespace boto

#define BOTO_THEME boto::style::DarkTheme
#include "BoxStyle.hpp"
#include "ButtonStyle.hpp"
#include "ElementStyle.hpp"
#include "InputBoxStyle.hpp"
#include "TextStyle.hpp"
#include "Theme.hpp"

namespace boto {

struct Box;
struct Text;
struct Element;
struct ButtonBase;
struct InputBoxBase;

namespace style {

template<>
struct FromTheme<Box, DarkTheme>
{
  constexpr static BoxStyle get()
  {
    return {
      EdgeSize::all(1),
      {
        {0, 0, 0, 127},
        BorderColorStyle::all({223, 223, 223, 255}),
      },
    };
  }
};

template<>
struct FromTheme<Text, DarkTheme>
{
  constexpr static TextStyle get()
  {
    return {{nullptr, 8, 8, 16}, {191, 191, 63, 255}, 0};
  }
};

template<>
struct FromTheme<ButtonBase, DarkTheme>
{
  constexpr static ButtonStyle get()
  {
    auto element = themeFor<Element, DarkTheme>();
    ElementPaintStyle buttonBox = {
      element.paint.text,
      {64, 64, 0, 255},
      {
        {127, 127, 0, 255},
        {127, 127, 0, 255},
        {31, 31, 0, 255},
        {31, 31, 0, 255},
      },
    };
    ElementPaintStyle buttonBoxGrabbed =
      buttonBox.withText({255, 255, 0, 255})
        .withBorder(BorderColorStyle::all(element.paint.text));
    return {
      EdgeSize::all(3),
      EdgeSize::all(2),
      element.font,
      element.scale,
      buttonBox,
      buttonBoxGrabbed,
      buttonBox.withBorder(buttonBox.border.invert()),
      buttonBoxGrabbed,
    };
  }
};

template<>
struct FromTheme<InputBoxBase, DarkTheme>
{
  constexpr static InputBoxStyle get()
  {
    auto element = themeFor<Element, DarkTheme>();
    return {
      EdgeSize::all(2),
      EdgeSize::all(1),
      element.font,
      element.scale,
      {
        element.paint.text,
        {64, 64, 0, 255},
        BorderColorStyle::all({127, 127, 0, 255}),
      },
      {
        element.paint.text,
        {64, 64, 0, 255},
        BorderColorStyle::all(element.paint.text),
      },
    };
  }
};

} // namespace style

} // namespace boto

#endif // BOTO_DARKTHEME_HPP_
