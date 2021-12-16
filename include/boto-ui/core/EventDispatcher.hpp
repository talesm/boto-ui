#ifndef BOTO_CORE_EVENTDISPATCHER_HPP
#define BOTO_CORE_EVENTDISPATCHER_HPP

#include <string>
#include <string_view>
#include <vector>
#include <SDL_assert.h>
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
  GRAB,
  ACTION,
  CANCEL,
};

/**
 * @brief The events an element will accept
 *
 */
enum class RequestEvent
{
  NONE,
  HOVER,
  ACTION,
};

/**
 * @brief The state of an event target
 *
 */
struct EventTargetState
{
  SDL_Rect rect;   ///< @brief The event target area (absolute)
  uint16_t status; ///< @brief the event target status
  Event event;     ///< @brief the event target event
};

/**
 * @brief Component responsible to handle and dispatch events for the UI
 *
 */
class EventDispatcher
{
private:
  struct EventTargetUnStack
  {
    void operator()(EventDispatcher* dispatcher) { dispatcher->popTarget(); }
  };

public:
  // Event triggers
  void movePointer(const SDL_Point& pos) { pointerPos = pos; }
  void pressPointer(unsigned button)
  {
    SDL_assert(button < 32);
    pointerPressed |= 1 << button;
  }
  void releasePointer(unsigned button)
  {
    SDL_assert(button < 32);
    pointerReleased = ~1 << button;
  }

  /// Reset flags (call once per turn)
  void reset()
  {
    hadHover = false;
    pointerPressed = pointerReleased = 0;
  }

  // Accessors
  const SDL_Point pointerPosition() const { return pointerPos; }
  bool isPointerPressed(unsigned button) const
  {
    SDL_assert(button < 32);
    // Equals is used because multiple buttons was buggy
    return !pointerReleased && pointerPressed == unsigned(1 << button);
  }

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
    Event event() const { return state().event; }

    const SDL_Rect& rect() const { return state().rect; }
  };

  /**
   * @brief Check events for the specified element
   *
   * @param req events you accept. Events after it will be ignored
   * @param rect the area occupied by the element (absolute)
   * @param id the unique id representing the event target
   * @return EventTarget
   */
  EventTarget check(RequestEvent req, SDL_Rect rect, std::string_view id = {})
  {
    if (!elementStack.empty()) {
      SDL_IntersectRect(&elementStack.back().rect, &rect, &rect);
    }

    Event event = Event::NONE;
    uint16_t status = req == RequestEvent::NONE
                        ? STATUS_NONE
                        : processHover(req, rect, id, event);
    auto index = elementStack.size();
    elementStack.emplace_back(EventTargetState{rect, status, event});
    return {this, index};
  }

private:
  SDL_Point pointerPos;
  Uint32 pointerPressed;
  Uint32 pointerReleased;
  bool hadHover;
  std::string idGrabbed;

  std::vector<EventTargetState> elementStack;

  void popTarget()
  {
    if (elementStack.back().status & STATUS_HOVERED) {
      hadHover = true;
    }
    elementStack.pop_back();
  }

  uint16_t processHover(RequestEvent req,
                        const SDL_Rect& rect,
                        std::string_view id,
                        Event& event)
  {
    if (hadHover || !SDL_PointInRect(&pointerPos, &rect)) {
      if (req >= RequestEvent::ACTION && idGrabbed == id &&
          (pointerReleased != 0 || pointerPressed != 0)) {
        event = Event::CANCEL;
      }
      return STATUS_NONE;
    }
    if (req == RequestEvent::HOVER) {
      return STATUS_HOVERED;
    }
    return STATUS_HOVERED | processActionHovered(req, rect, id, event);
  }

  uint16_t processActionHovered(RequestEvent req,
                                const SDL_Rect& rect,
                                std::string_view id,
                                Event& event)
  {
    if (pointerReleased != 0) {
      if (idGrabbed == id) {
        event = Event::ACTION;
      }
      return STATUS_NONE;
    }
    if (pointerPressed != 1) {
      if (idGrabbed == id) {
        if (pointerPressed == 0) {
          return STATUS_GRABBED;
        }
        event = Event::CANCEL;
        idGrabbed.clear();
      }
      return STATUS_NONE;
    }
    event = Event::GRAB;
    idGrabbed = id;
    return STATUS_GRABBED;
  }
};

using EventTarget = EventDispatcher::EventTarget;

} // namespace boto

#endif // BOTO_CORE_EVENTDISPATCHER_HPP
