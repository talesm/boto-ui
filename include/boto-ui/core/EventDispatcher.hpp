#ifndef BOTO_CORE_EVENTDISPATCHER_HPP
#define BOTO_CORE_EVENTDISPATCHER_HPP

#include <string>
#include <string_view>
#include <vector>
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

class EventDispatcher
{
  SDL_Point pointerPos;
  bool hadHover;
  std::vector<SDL_Rect> elementRects;

  struct EventTargetUnStack
  {
    void operator()(EventDispatcher* dispatcher) { dispatcher->endCheck(); }
  };

public:
  // Event triggers
  void movePointer(const SDL_Point& pos) { pointerPos = pos; }

  /// Reset flags (call once per turn)
  void reset() { hadHover = false; }

  using Cookie = CookieBase<EventDispatcher, EventTargetUnStack>;

  struct EventTarget : CookieBase<EventDispatcher, EventTargetUnStack>
  {
    SDL_Rect rect;
    uint16_t status;

    EventTarget() = default;
    EventTarget(EventDispatcher* c, const SDL_Rect r, uint16_t status)
      : CookieBase(c)
      , rect(r)
      , status(status)
    {}
  };

  EventTarget check(RequestEvent ev, SDL_Rect rect)
  {
    if (!elementRects.empty()) {
      SDL_IntersectRect(&elementRects.back(), &rect, &rect);
    }
    elementRects.push_back(rect);

    uint16_t status = 0;
    if (ev == RequestEvent::NONE) {
      goto END;
    }

    // Check hovering
    if (!hadHover && SDL_PointInRect(&pointerPos, &rect)) {
      status |= STATUS_HOVERED;
      hadHover = true;
    }
  END:
    return {this, rect, status};
  }

private:
  void endCheck() { elementRects.pop_back(); }
};

using EventTarget = EventDispatcher::EventTarget;

} // namespace boto

#endif // BOTO_CORE_EVENTDISPATCHER_HPP
