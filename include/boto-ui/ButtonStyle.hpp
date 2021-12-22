#ifndef BOTO_BUTTONSTYLE_HPP
#define BOTO_BUTTONSTYLE_HPP

#include <SDL.h>
#include "ControlStyle.hpp"
#include "Theme.hpp"

namespace boto {

// Style for button
struct ButtonStyle
{
  EdgeSize padding;
  EdgeSize border;
  Font font;
  int scale;
  ControlPaintStyle normal;
  ControlPaintStyle grabbed;
  ControlPaintStyle pressed;
  ControlPaintStyle pressedGrabbed;
};

struct ButtonBase;
struct Button;
struct ToggleButton;
struct ChoiceButton;

constexpr const ControlPaintStyle&
decideButtonColors(const ButtonStyle& style, bool pushed, bool grabbing)
{
  if (grabbing == pushed) {
    return (grabbing) ? style.grabbed : style.normal;
  } else {
    return (grabbing) ? style.pressedGrabbed : style.pressed;
  }
}

namespace style {

template<class Theme>
struct FromTheme<ButtonBase, Theme>
{
  constexpr static ButtonStyle get()
  {
    auto control = themeFor<Control, Theme>();
    ControlPaintStyle buttonBox = {
      control.paint.text,
      {176, 195, 222, 255},
      {
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {0, 0, 0, 255},
        {0, 0, 0, 255},
      },
    };
    ControlPaintStyle buttonBoxGrabbed =
      buttonBox.withBackground({147, 173, 210, 255});
    return {
      EdgeSize::all(3),
      EdgeSize::all(1),
      control.font,
      control.scale,
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
} // namespace boto

#endif // BOTO_BUTTONSTYLE_HPP
