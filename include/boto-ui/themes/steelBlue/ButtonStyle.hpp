#ifndef BOTO_BUTTONSTYLE_HPP
#define BOTO_BUTTONSTYLE_HPP

#include <SDL.h>
#include "ControlStyle.hpp"
#include "core/Theme.hpp"

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

template<>
struct StyleFor<SteelBlue, ButtonBase>
{
  static ButtonStyle get(Theme& theme)
  {
    auto buttonBox = theme.of<Control>()
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

template<>
struct StyleFor<SteelBlue, Button> : StyleFor<SteelBlue, ButtonBase>
{};
template<>
struct StyleFor<SteelBlue, ToggleButton> : StyleFor<SteelBlue, ButtonBase>
{};
template<>
struct StyleFor<SteelBlue, ChoiceButton> : StyleFor<SteelBlue, ButtonBase>
{};
} // namespace boto

#endif // BOTO_BUTTONSTYLE_HPP
