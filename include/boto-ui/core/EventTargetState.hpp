#ifndef BOTO_CORE_EVENTTARGETSTATE_HPP
#define BOTO_CORE_EVENTTARGETSTATE_HPP

#include <string_view>
#include <SDL_rect.h>
#include "core/Event.hpp"
#include "core/Status.hpp"

namespace boto {

/**
 * @brief The state of an event target
 *
 */
struct EventTargetState
{
  size_t idLength;    ///< @brief just an aux. Ignore
  SDL_Rect rect;      ///< @brief The event target area (absolute)
  StatusFlags status; ///< @brief the event target status
  Event event;        ///< @brief the event target event
};
}

#endif // BOTO_CORE_EVENTTARGETSTATE_HPP
