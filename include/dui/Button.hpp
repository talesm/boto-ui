#ifndef DUI_BUTTON_HPP
#define DUI_BUTTON_HPP

#include <string_view>
#include "ButtonStyle.hpp"
#include "Group.hpp"
#include "Panel.hpp"

namespace dui {

constexpr const ElementPaintStyle&
decideButtonColors(const ButtonStyle& style, bool pushed, bool grabbing)
{
  if (grabbing == pushed) {
    return (grabbing) ? style.grabbed : style.normal;
  } else {
    return (grabbing) ? style.pressedGrabbed : style.pressed;
  }
}

/**
 * @brief Common button behavior
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param pushed if the button is pushed or not
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state (just released)
 * @return false otherwise
 */
inline bool
buttonBase(Target target,
           std::string_view id,
           std::string_view str,
           bool pushed,
           const SDL_Point& p = {0},
           const ButtonStyle& style = themeFor<ButtonBase>())
{
  if (str.empty()) {
    str = id;
  }
  auto adv = elementSize(style.padding + style.border,
                         measure(str, style.font, style.scale));
  SDL_Rect r{p.x, p.y, adv.x, adv.y};
  auto action = target.checkMouse(id, r);

  element(target,
          str,
          r,
          {style.padding,
           style.border,
           style.font,
           style.scale,
           decideButtonColors(style, pushed, action == MouseAction::HOLD)});
  return action == MouseAction::ACTION;
}

/**
 * @brief A push button
 * @ingroup elements
 *
 * It can be used to make an action every time it is pushed
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state (just released)
 * @return false otherwise
 */
inline bool
button(Target target,
       std::string_view id,
       std::string_view str,
       const SDL_Point& p = {0},
       const ButtonStyle& style = themeFor<Button>())
{
  return buttonBase(target, id, str, false, p, style);
}
/// @copydoc button()
/// @ingroup elements
inline bool
button(Target target,
       std::string_view id,
       const SDL_Point& p = {0},
       const ButtonStyle& style = themeFor<Button>())
{
  return button(target, id, id, p, style);
}

/**
 * @brief A button that toggle a boolean variable
 * @ingroup elements
 *
 * Every time it is pushed the value is negated. The button reflects this state
 * by being pushed or pulled if the variable is true of false, respectively.
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param value a pointer to a boolean with the state
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state
 * @return false otherwise
 */
inline bool
toggleButton(Target target,
             std::string_view id,
             std::string_view str,
             bool* value,
             const SDL_Point& p = {0},
             const ButtonStyle& style = themeFor<ToggleButton>())
{
  if (buttonBase(target, id, str, *value, p, style)) {
    *value = !*value;
    return true;
  }
  return false;
}
/// @copydoc toggleButton
/// @ingroup elements
inline bool
toggleButton(Target target,
             std::string_view id,
             bool* value,
             const SDL_Point& p = {0},
             const ButtonStyle& style = themeFor<ToggleButton>())
{
  return toggleButton(target, id, id, value, p, style);
}

/**
 * @brief A button part of multiple choice question
 * @ingroup elements
 *
 * If this button is actionned the value is changed to the given option
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param value a pointer to the control variable
 * @param option the option this button represents if value is equivalent to
 * this, then the button will appear pressed. If the user pushed, then the value
 * is set to this option.
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state
 * @return false otherwise
 */
template<class T, class U>
inline bool
choiceButton(Target target,
             std::string_view id,
             std::string_view str,
             T* value,
             U option,
             const SDL_Point& p = {0},
             const ButtonStyle& style = themeFor<ChoiceButton>())
{
  bool selected = *value == option;
  if (buttonBase(target, id, str, selected, p, style) && !selected) {
    *value = option;
    return true;
  }
  return false;
}
/// @copydoc choiceButton
/// @ingroup elements
template<class T, class U>
inline bool
choiceButton(Target target,
             std::string_view id,
             T* value,
             U option,
             const SDL_Point& p = {0},
             const ButtonStyle& style = themeFor<ChoiceButton>())
{
  return choiceButton(target, id, id, value, option, p, style);
}

} // namespace dui

#endif // DUI_BUTTON_HPP
