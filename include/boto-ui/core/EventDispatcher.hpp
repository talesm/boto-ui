#ifndef BOTO_CORE_EVENTDISPATCHER_HPP
#define BOTO_CORE_EVENTDISPATCHER_HPP

#include <string>
#include <string_view>
#include <vector>
#include <SDL_keycode.h>
#include <SDL_rect.h>
#include "util/CookieBase.hpp"

namespace boto {

/**
 * @brief The current element state
 *
 */
enum Status : uint16_t
{
  STATUS_NONE = 0,
  STATUS_HOVERED = 1,
  STATUS_GRABBED = 2,
  STATUS_FOCUSED = 4,
  STATUS_INPUTING = 8,
};
/**
 * @brief The event the element is receiving
 *
 */
enum class Event : uint16_t
{
  NONE,
  FOCUS_GAINED,
  FOCUS_LOST,
  ACTION,
  CANCEL,
  TEXT_INPUT,
  BACKSPACE,
};

/**
 * @brief The events an element will accept
 *
 */
enum class RequestEvent
{
  NONE,
  HOVER,
};

/**
 * @brief The state of an event target
 *
 */
struct EventTargetState
{
  SDL_Rect rect;   ///< @brief The event target area (absolute)
  uint16_t status; ///< @brief the event target status
};

/**
 * @brief Component responsible to handle and dispatch events for the UI
 *
 */
class EventDispatcher
{
  SDL_Point pointerPos;
  bool hadHover;

  std::vector<EventTargetState> elementStack;

  struct EventTargetUnStack
  {
    void operator()(EventDispatcher* dispatcher)
    {
      dispatcher->elementStack.pop_back();
    }
  };

public:
  // Event triggers
  void movePointer(const SDL_Point& pos) { pointerPos = pos; }

  /// Reset flags (call once per turn)
  void reset() { hadHover = false; }

  /**
   * @brief An element able to receive events
   *
   */
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

    const EventTargetState& state() const { return get()->elementStack[index]; }
    EventTargetState& state() { return get()->elementStack[index]; }

    uint16_t status() const { return state().status; }

    const SDL_Rect& rect() const { return state().rect; }
  };

  /**
   * @brief Check events for the specified element
   *
   * @param ev events you accept. Events after it will be ignored
   * @param rect the area occupied by the element (absolute)
   * @return EventTarget
   */
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
    elementStack.emplace_back(EventTargetState{rect, status});
    return {this, index};
  }
};

using EventTarget = EventDispatcher::EventTarget;

} // namespace boto

#endif // BOTO_CORE_EVENTDISPATCHER_HPP
