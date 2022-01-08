#ifndef BOTO_BUTTON_HPP
#define BOTO_BUTTON_HPP

#include <string_view>
#include "Control.hpp"

namespace boto {

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
           const SDL_Point& p,
           const ButtonStyle& style)
{
  if (str.empty()) {
    str = id;
  }
  auto sz = measure(style.normal.padding + style.normal.decoration.border) +
            measure(str, style.normal.text.font, style.normal.text.scale);
  SDL_Rect r{p.x, p.y, sz.x, sz.y};
  auto state = target.element(id, r, RequestEvent::INPUT);

  control(
    target,
    {},
    str,
    r,
    RequestEvent::NONE,
    decideButtonColors(style, pushed, state.status.test(Status::GRABBED)));
  return state.event == Event::ACTION;
}
inline bool
buttonBase(Target target,
           std::string_view id,
           std::string_view str,
           bool pushed,
           const SDL_Point& p = {0})
{
  return buttonBase(target, id, str, pushed, p, target.styleFor<ButtonBase>());
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
       const SDL_Point& p,
       const ButtonStyle& style)
{
  return buttonBase(target, id, str, false, p, target.styleFor<Button>());
}
inline bool
button(Target target,
       std::string_view id,
       std::string_view str,
       const SDL_Point& p = {0})
{
  return button(target, id, str, p, target.styleFor<Button>());
}
/// @copydoc button()
/// @ingroup elements
inline bool
button(Target target, std::string_view id, const SDL_Point& p = {0})
{
  return button(target, id, id, p);
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
             const SDL_Point& p = {0})
{
  if (buttonBase(target, id, str, *value, p, target.styleFor<ToggleButton>())) {
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
             const SDL_Point& p = {0})
{
  return toggleButton(target, id, id, value, p);
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
             const SDL_Point& p = {0})
{
  bool selected = *value == option;
  if (buttonBase(
        target, id, str, selected, p, target.styleFor<ChoiceButton>()) &&
      !selected) {
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
             const SDL_Point& p = {0})
{
  return choiceButton(target, id, id, value, option, p);
}

} // namespace boto

#endif // BOTO_BUTTON_HPP
