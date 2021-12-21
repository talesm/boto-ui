#ifndef BOTO_ELEMENTS_BUTTONHANDLER_HPP
#define BOTO_ELEMENTS_BUTTONHANDLER_HPP

#include <SDL_assert.h>
#include "core/EventTargetState.hpp"

namespace boto {

/**
 * @brief Handle a common push button
 *
 * @param state the button state. Must be at least GRAB level
 * @return true if an ACTION occurred
 * @return false otherwise
 */
inline bool
handlePushButton(const EventTargetState& state)
{
  return state.event == Event::ACTION;
}

/**
 * @brief Handle a common check button (like a checkbox)
 *
 * @param state the button state. Must be at least GRAB level
 * @param checked the checked status
 * @return true if an ACTION occurred (and checked was inverted)
 * @return false otherwise
 */
inline bool
handleCheckButton(const EventTargetState& state, bool* checked)
{
  if (handlePushButton(state)) {
    SDL_assert_paranoid(checked != nullptr);
    *checked = !*checked;
    return true;
  }
  return false;
}

}

#endif // BOTO_ELEMENTS_BUTTONHANDLER_HPP
