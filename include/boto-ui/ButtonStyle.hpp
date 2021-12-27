#ifndef BOTO_BUTTONSTYLE_HPP
#define BOTO_BUTTONSTYLE_HPP

#include <SDL.h>
#include "ControlStyle.hpp"
#include "Theme.hpp"

namespace boto {

// Style for button
struct ButtonStyle
{
  ControlStyle normal;
  ControlStyle grabbed;
  ControlStyle pressed;
  ControlStyle pressedGrabbed;
};

struct ButtonBase;
struct Button;
struct ToggleButton;
struct ChoiceButton;

constexpr const ControlStyle&
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
    auto buttonBox = themeFor<Control, Theme>()
                       .withBorder(EdgeSize::all(2))
                       .withBackgroundColor({176, 195, 222, 255})
                       .withBorderColor({
                         {255, 255, 255, 255},
                         {255, 255, 255, 255},
                         {0, 0, 0, 255},
                         {0, 0, 0, 255},
                       });
    auto buttonBoxGrabbed = buttonBox.withBackgroundColor({147, 173, 210, 255});
    return {
      buttonBox,
      buttonBoxGrabbed,
      buttonBox.withBorderColor(buttonBox.decoration.paint.border.invert()),
      buttonBoxGrabbed.withBorderColor(
        buttonBox.decoration.paint.border.invert()),
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
