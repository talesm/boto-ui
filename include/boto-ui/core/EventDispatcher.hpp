#ifndef BOTO_CORE_EVENTDISPATCHER_HPP
#define BOTO_CORE_EVENTDISPATCHER_HPP

#include <string>
#include <string_view>
#include <SDL_keycode.h>
#include <SDL_rect.h>
#include "util/CookieBase.hpp"

namespace boto {

enum ElementStatus : uint16_t
{
  STATUS_NONE = 0,
  STATUS_HOVERED = 1,
  STATUS_GRABBED = 2,
  STATUS_FOCUSED = 4,
  STATUS_INPUTING = 8,
};
enum class ElementEvent : uint16_t
{
  NONE,
  FOCUS_GAINED,
  FOCUS_LOST,
  ACTION,
  CANCEL,
  TEXT_INPUT,
  BACKSPACE,
};

enum class RequestEvent
{
  NONE,
  HOVER,
};

struct EventTarget
{
  uint16_t status;
  bool isPointerOver() const { return status & STATUS_HOVERED; }
};

class EventDispatcher
{
  SDL_Point pointerPos;

public:
  // Event triggers
  void movePointer(const SDL_Point& pos) { pointerPos = pos; }

  EventTarget check(RequestEvent ev, const SDL_Rect& rect)
  {
    uint16_t status = 0;
    if (ev == RequestEvent::NONE) {
      goto END;
    }

    if (SDL_PointInRect(&pointerPos, &rect)) {
      status |= STATUS_HOVERED;
    }
  END:
    return {status};
  }
};

} // namespace boto

#endif // BOTO_CORE_EVENTDISPATCHER_HPP
