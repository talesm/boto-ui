#ifndef BOTO_CORE_EVENTDISPATCHER_HPP
#define BOTO_CORE_EVENTDISPATCHER_HPP

#include <string>
#include <string_view>
#include <vector>
#include <SDL_assert.h>
#include <SDL_keycode.h>
#include <SDL_rect.h>
#include "TargetStatus.hpp"
#include "util/CookieBase.hpp"

namespace boto {

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
  FOCUS_GAINED,
  FOCUS_LOST,
};

/**
 * @brief The events an element will accept
 *
 */
enum class RequestEvent
{
  NONE,
  HOVER,
  GRAB,
  FOCUS,
};

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
  EventTarget check(RequestEvent req, SDL_Rect rect, std::string_view id = {});

private:
  SDL_Point pointerPos;
  Uint32 pointerPressed;
  Uint32 pointerReleased;
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

  StatusFlags checkHover(RequestEvent req,
                         const SDL_Rect& rect,
                         std::string_view id,
                         Event& event);

  StatusFlags checkGrabOver(RequestEvent req,
                            std::string_view id,
                            Event& event);
  StatusFlags checkGrabOut(RequestEvent req, std::string_view id, Event& event);

  StatusFlags checkFocus(RequestEvent req, std::string_view id, Event& event);
  StatusFlags gainFocus(RequestEvent req, std::string_view id, Event& event);
  StatusFlags loseFocus(RequestEvent req, std::string_view id, Event& event);
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

  void discard(StatusFlags flags)
  {
    //
    state().status.reset(flags);
  }
};

inline EventTarget
EventDispatcher::check(RequestEvent req, SDL_Rect rect, std::string_view id)
{
  if (!elementStack.empty()) {
    SDL_IntersectRect(&elementStack.back().rect, &rect, &rect);
  }

  Event event = Event::NONE;
  StatusFlags status =
    req == RequestEvent::NONE ? Status::NONE : checkHover(req, rect, id, event);
  auto index = elementStack.size();
  elementStack.emplace_back(EventTargetState{rect, status, event});
  return {this, index};
}

inline StatusFlags
EventDispatcher::checkHover(RequestEvent req,
                            const SDL_Rect& rect,
                            std::string_view id,
                            Event& event)
{
  if (hadHover || !SDL_PointInRect(&pointerPos, &rect)) {
    if (req == RequestEvent::HOVER) {
      return Status::NONE;
    }
    return checkGrabOut(req, id, event);
  }
  if (req == RequestEvent::HOVER) {
    return Status::HOVERED;
  }
  return Status::HOVERED | checkGrabOver(req, id, event);
}

inline StatusFlags
EventDispatcher::checkGrabOver(RequestEvent req,
                               std::string_view id,
                               Event& event)
{
  if (pointerReleased != 0) {
    if (idGrabbed == id) {
      event = Event::ACTION;
      idGrabbed.clear();
    }
    return checkFocus(req, id, event);
  }
  if (pointerPressed != 1) {
    if (idGrabbed != id) {
      return req == RequestEvent::GRAB ? Status::NONE
                                       : gainFocus(req, id, event);
    }
    if (pointerPressed == 0) {
      return Status::GRABBED | checkFocus(req, id, event);
    }
    event = Event::CANCEL;
    idGrabbed.clear();
    return checkFocus(req, id, event);
  }
  event = Event::GRAB;
  idGrabbed = id;
  if (req == RequestEvent::GRAB) {
    return Status::GRABBED;
  }
  return Status::GRABBED | gainFocus(req, id, event);
}

inline StatusFlags
EventDispatcher::checkGrabOut(RequestEvent req,
                              std::string_view id,
                              Event& event)
{
  if (idGrabbed != id) {
    return pointerPressed == 0 ? checkFocus(req, id, event)
                               : loseFocus(req, id, event);
  }
  if (pointerReleased == 0 && pointerPressed == 0) {
    return Status::GRABBED | checkFocus(req, id, event);
  }
  event = Event::CANCEL;
  idGrabbed.clear();
  if (req == RequestEvent::GRAB || idFocus != id) {
    return Status::NONE;
  }
  if (pointerPressed != 0) {
    return loseFocus(req, id, event);
  }
  return checkFocus(req, id, event);
}

inline StatusFlags
EventDispatcher::checkFocus(RequestEvent req, std::string_view id, Event& event)
{
  if (idFocus == id) {
    if (idLosingFocus != id) {
      idNextFocus = id;
    }
    return Status::FOCUSED;
  }
  if (idLosingFocus == id) {
    event = Event::FOCUS_LOST;
    return Status::NONE;
  }
  if (idNextFocus == id) {
    idFocus = id;
    event = Event::FOCUS_GAINED;
    return Status::FOCUSED;
  }
  return Status::NONE;
}

inline StatusFlags
EventDispatcher::gainFocus(RequestEvent req, std::string_view id, Event& event)
{
  if (idFocus == id || idNextFocus == id) {
    return checkFocus(req, id, event);
  }
  if (!idNextFocus.empty()) {
    return Status::NONE;
  }
  idNextFocus = id;
  if (event != Event::NONE || !idLosingFocus.empty() ||
      (!idFocus.empty() && idFocus == idGrabbed)) {
    return Status::NONE;
  }
  idLosingFocus = idFocus;
  idFocus = id;
  event = Event::FOCUS_GAINED;
  return Status::FOCUSED;
}
inline StatusFlags
EventDispatcher::loseFocus(RequestEvent req, std::string_view id, Event& event)
{
  if (idFocus != id) {
    return checkFocus(req, id, event);
  }
  if (event == Event::NONE) {
    idFocus.clear();
    event = Event::FOCUS_LOST;
    return Status::NONE;
  }
  idLosingFocus = id;
  auto status = checkFocus(req, id, event);
  return status;
}

} // namespace boto

#endif // BOTO_CORE_EVENTDISPATCHER_HPP
