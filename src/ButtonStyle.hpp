#ifndef DUI_BUTTONSTYLE_HPP
#define DUI_BUTTONSTYLE_HPP

#include <SDL.h>
#include "ElementStyle.hpp"
#include "Theme.hpp"

namespace dui {

// Style for button
struct ButtonStyle
{
  EdgeSize padding;
  EdgeSize border;
  Font font;
  int scale;
  ElementPaintStyle normal;
  ElementPaintStyle grabbed;
  ElementPaintStyle pressed;
  ElementPaintStyle pressedGrabbed;
};

struct ButtonBase;
struct Button;
struct ToggleButton;
struct ChoiceButton;

namespace style {

template<class Theme>
struct FromTheme<ButtonBase, Theme>
{
  constexpr static ButtonStyle get()
  {
    auto element = themeFor<Element, Theme>();
    ElementPaintStyle buttonBox = {
      element.paint.text,
      {176, 195, 222, 255},
      {
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {0, 0, 0, 255},
        {0, 0, 0, 255},
      },
    };
    ElementPaintStyle buttonBoxGrabbed =
      buttonBox.withBackground({147, 173, 210, 255});
    return {
      EdgeSize::all(3),
      EdgeSize::all(2),
      element.font,
      element.scale,
      buttonBox,
      buttonBoxGrabbed,
      buttonBox.withBorder(buttonBox.border.invert()),
      buttonBoxGrabbed.withBorder(buttonBox.border.invert()),
    };
  }
};

template<class Theme>
struct FromTheme<Button, Theme> : FromTheme<ButtonBase, Theme>
{};
template<class Theme>
struct FromTheme<ToggleButton, Theme> : FromTheme<ButtonBase, Theme>
{};
template<class Theme>
struct FromTheme<ChoiceButton, Theme> : FromTheme<ButtonBase, Theme>
{};
} // namespace style
} // namespace dui

#endif // DUI_BUTTONSTYLE_HPP
