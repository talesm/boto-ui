#ifndef BOTO_CORE_EVENTDISPATCHER_HPP
#define BOTO_CORE_EVENTDISPATCHER_HPP

#include <string>
#include <string_view>
#include <vector>
#include <SDL_assert.h>
#include <SDL_keycode.h>
#include <SDL_rect.h>
#include "Event.hpp"
#include "TargetStatus.hpp"
#include "util/CookieBase.hpp"

namespace boto {

/**
 * @brief The state of an event target
 *
 */
struct EventTargetState
{
  SDL_Rect rect;      ///< @brief The event target area (absolute)
  StatusFlags status; ///< @brief the event target status
  Event event;        ///< @brief the event target event
};

class EventTarget;

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
  friend class EventTarget;

public:
  /// @name Event triggers
  /// @{
  void movePointer(const SDL_Point& pos) { pointerPos = pos; }
  void pressPointer(unsigned button)
  {
    SDL_assert(button < 32);
    pointerPressed |= 1 << button;
  }
  void releasePointer(unsigned button)
  {
    SDL_assert(button < 32);
    pointerReleased |= 1 << button;
  }
  /// @}

  /// Reset flags (call once per turn)
  void reset()
  {
    hadHover = false;
    pointerPressed = pointerReleased = 0;
    if (idNextFocus == idFocus) {
      idNextFocus.clear();
    }
    if (idLosingFocus == idFocus) {
      idFocus.clear();
    } else {
      idLosingFocus.clear();
    }
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
   * @brief Check events for the specified element
   *
   * @param req events you accept. Events after it will be ignored
   * @param rect the area occupied by the element (absolute)
   * @param id the unique id representing the event target
   * @return EventTarget
   */
  EventTarget check(RequestEvent req,
                    const SDL_Rect& rect,
                    std::string_view id = {});

private:
  SDL_Point pointerPos;
  Uint32 pointerPressed;
  Uint32 pointerReleased;

  std::string idCurrent;
  bool hadHover;
  std::string idGrabbed;
  std::string idFocus;
  std::string idNextFocus;
  std::string idLosingFocus;

  std::vector<EventTargetState> elementStack;

  void popTarget()
  {
    if (elementStack.back().status & Status::HOVERED) {
      hadHover = true;
    }
    elementStack.pop_back();
  }

  StatusFlags checkHover(RequestEvent req, const SDL_Rect& rect, Event& event);

  StatusFlags checkGrabOver(RequestEvent req, Event& event);
  StatusFlags checkGrabOut(RequestEvent req, Event& event);

  StatusFlags checkFocus(RequestEvent req, Event& event);
  StatusFlags gainFocus(RequestEvent req, Event& event);
  StatusFlags loseFocus(RequestEvent req, Event& event);
};

/**
 * @brief An element able to receive events
 *
 */
class EventTarget
  : public CookieBase<EventDispatcher, EventDispatcher::EventTargetUnStack>
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

  StatusFlags status() const { return state().status; }
  Event event() const { return state().event; }

  const SDL_Rect& rect() const { return state().rect; }

  void discard(StatusFlags flags) { state().status.reset(flags); }
};

inline EventTarget
EventDispatcher::check(RequestEvent req,
                       const SDL_Rect& rect,
                       std::string_view id)
{
  idCurrent = id;
  if (req == RequestEvent::NONE) {
    elementStack.emplace_back(EventTargetState{rect});
    return {this, elementStack.size() - 1};
  }

  Event event = Event::NONE;
  elementStack.emplace_back(EventTargetState{
    rect,
    checkHover(req, rect, event),
    event,
  });
  return {this, elementStack.size() - 1};
}

inline StatusFlags
EventDispatcher::checkHover(RequestEvent req,
                            const SDL_Rect& rect,
                            Event& event)
{
  if (hadHover ||
      !(elementStack.empty() ||
        elementStack.back().status.test(Status::HOVERED)) ||
      !SDL_PointInRect(&pointerPos, &rect)) {
    if (req == RequestEvent::HOVER) {
      return Status::NONE;
    }
    return checkGrabOut(req, event);
  }
  if (req == RequestEvent::HOVER) {
    return Status::HOVERED;
  }
  return Status::HOVERED | checkGrabOver(req, event);
}

inline StatusFlags
EventDispatcher::checkGrabOver(RequestEvent req, Event& event)
{
  if (pointerReleased != 0) {
    if (idGrabbed == idCurrent) {
      event = Event::ACTION;
      idGrabbed.clear();
    }
    return checkFocus(req, event);
  }
  if (pointerPressed != 1) {
    if (idGrabbed != idCurrent) {
      return req == RequestEvent::GRAB ? Status::NONE : gainFocus(req, event);
    }
    if (pointerPressed == 0) {
      return Status::GRABBED | checkFocus(req, event);
    }
    event = Event::CANCEL;
    idGrabbed.clear();
    return checkFocus(req, event);
  }
  event = Event::GRAB;
  idGrabbed = idCurrent;
  if (req == RequestEvent::GRAB) {
    return Status::GRABBED;
  }
  return Status::GRABBED | gainFocus(req, event);
}

inline StatusFlags
EventDispatcher::checkGrabOut(RequestEvent req, Event& event)
{
  if (idGrabbed != idCurrent) {
    return pointerPressed == 0 ? checkFocus(req, event) : loseFocus(req, event);
  }
  if (pointerReleased == 0 && pointerPressed == 0) {
    return Status::GRABBED | checkFocus(req, event);
  }
  event = Event::CANCEL;
  idGrabbed.clear();
  if (req == RequestEvent::GRAB || idFocus != idCurrent) {
    return Status::NONE;
  }
  if (pointerPressed != 0) {
    return loseFocus(req, event);
  }
  return checkFocus(req, event);
}

inline StatusFlags
EventDispatcher::checkFocus(RequestEvent req, Event& event)
{
  if (idFocus == idCurrent) {
    if (idLosingFocus != idCurrent) {
      idNextFocus = idCurrent;
    }
    return Status::FOCUSED;
  }
  if (idLosingFocus == idCurrent) {
    event = Event::FOCUS_LOST;
    return Status::NONE;
  }
  if (idNextFocus == idCurrent) {
    idFocus = idCurrent;
    event = Event::FOCUS_GAINED;
    return Status::FOCUSED;
  }
  return Status::NONE;
}

inline StatusFlags
EventDispatcher::gainFocus(RequestEvent req, Event& event)
{
  if (idFocus == idCurrent || idNextFocus == idCurrent) {
    return checkFocus(req, event);
  }
  if (!idNextFocus.empty()) {
    return Status::NONE;
  }
  idNextFocus = idCurrent;
  if (event != Event::NONE || !idLosingFocus.empty() ||
      (!idFocus.empty() && idFocus == idGrabbed)) {
    return Status::NONE;
  }
  idLosingFocus = idFocus;
  idFocus = idCurrent;
  event = Event::FOCUS_GAINED;
  return Status::FOCUSED;
}
inline StatusFlags
EventDispatcher::loseFocus(RequestEvent req, Event& event)
{
  if (idFocus != idCurrent) {
    return checkFocus(req, event);
  }
  if (event == Event::NONE) {
    idFocus.clear();
    event = Event::FOCUS_LOST;
    return Status::NONE;
  }
  idLosingFocus = idCurrent;
  auto status = checkFocus(req, event);
  return status;
}

} // namespace boto

#endif // BOTO_CORE_EVENTDISPATCHER_HPP
