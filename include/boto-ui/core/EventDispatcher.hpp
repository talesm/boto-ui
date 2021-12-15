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

struct TargetState
{
  SDL_Rect rect;
  uint16_t status;
};

class EventDispatcher
{
  SDL_Point pointerPos;
  bool hadHover;

  std::vector<TargetState> elementStack;

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

  class EventTarget : public CookieBase<EventDispatcher, EventTargetUnStack>
  {
    size_t index;

    EventTarget(EventDispatcher* c, size_t index)
      : CookieBase(c)
      , index(index)
    {}

    friend class EventDispatcher;

  public:
    EventTarget() = default;

    const TargetState& state() const { return get()->elementStack[index]; }
    TargetState& state() { return get()->elementStack[index]; }

    uint16_t status() const { return state().status; }

    const SDL_Rect& rect() const { return state().rect; }
  };

  EventTarget check(RequestEvent ev, SDL_Rect rect)
  {
    if (!elementStack.empty()) {
      SDL_IntersectRect(&elementStack.back().rect, &rect, &rect);
    }

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
    auto index = elementStack.size();
    elementStack.emplace_back(TargetState{rect, status});
    return {this, index};
  }

private:
  void endCheck() { elementStack.pop_back(); }
};

using EventTarget = EventDispatcher::EventTarget;

} // namespace boto

#endif // BOTO_CORE_EVENTDISPATCHER_HPP
