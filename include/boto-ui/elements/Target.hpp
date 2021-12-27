#ifndef BOTO_TARGET_HPP_
#define BOTO_TARGET_HPP_

#include <string_view>
#include <SDL.h>
#include "core/EventTargetState.hpp"
#include "core/Frame.hpp"

namespace boto {

/**
 * @brief A target where elements can be added to
 *
 */
class Target
{

public:
  Target()
    : frame(nullptr)
  {}

  /// Ctor
  Target(Frame& frame)
    : frame(&frame)
  {}

  /// Ctor
  Target(Container& container)
    : frame(container.get())
  {}

  /**
   * @brief Check the leaf element for events
   *
   * @param id element id
   * @param r the element local rect (Use State.checkMouse() for global rect)
   * @param req the events it might accept
   * @return EventTargetState
   */
  const EventTargetState& check(std::string_view id,
                                const SDL_Rect& r,
                                RequestEvent req = RequestEvent::INPUT)
  {
    if (id.empty()) {
      lastElementState = frame->element(r, req);
      lastElementId = {};
    } else if (id != lastElementId) {
      lastElementState = frame->element(id, r, req);
      lastElementId = id;
    }
    return lastElementState;
  }

  /**
   * @brief Create container
   *
   * @param id element id
   * @param r the element local rect
   * @param offset the offset
   * @param endPadding the endPadding
   * @param layout the layout
   * @param elementSpacing the spacing between its sub elements
   * @return Container
   */
  Container container(std::string_view id,
                      const SDL_Rect& r,
                      const SDL_Point& offset = {},
                      const SDL_Point& endPadding = {},
                      Layout layout = Layout::NONE,
                      int elementSpacing = 0)
  {
    return frame->container(id, r, offset, endPadding, layout, elementSpacing);
  }

  /**
   * @brief Check the mouse action/status for element in this group
   *
   * @param id element id
   * @param r the element local rect (Use State.checkMouse() for global rect)
   * @return MouseAction
   * @deprecated description
   */
  MouseAction checkMouse(std::string_view id, SDL_Rect r);

  /**
   * @brief Check if given contained element is active
   *
   * @param id the id to check
   * @return true
   * @return false
   */
  bool hasFocus(std::string_view id)
  {
    return check(id, {}).status.test(Status::FOCUSED);
  }
  bool isActive(std::string_view id) { return hasFocus(id); }

  /**
   * @brief Check the text action/status for element in this group
   *
   * @param id the element id
   * @return TextAction
   */
  TextAction checkText(std::string_view id);

  /**
   * @brief Get the last input text
   *
   * To check if the text was for the current element and frame, use checkText()
   * or Frame.checkText().
   *
   * @return std::string_view
   */
  std::string_view lastText() const { return frame->input(); }
  /**
   * @brief Get the last key down
   *
   * To check if the text was for the current element and frame, use checkText()
   * or Frame.checkText().
   *
   * @return std::string_view
   */
  SDL_Keysym lastKeyDown() const { return frame->lastKeyDown(); }

  /**
   * @brief Last mouse position
   *
   * @return SDL_Point the last mouse pos
   */
  SDL_Point lastMousePos() const
  {
    auto pos = frame->pointerPosition();
    pos.x -= lastElementState.rect.x;
    pos.x -= lastElementState.rect.y;
    return pos;
  }

  /// Get the target's displayList
  DisplayList& getDisplayList() const { return frame->displayList(); }

  /// Get the position where the next element can be added
  SDL_Point getCaret() const
  {
    if (auto* c = frame->getTop())
      return c->caret();
    return {};
  }

  /// Return true if there is a subtarget active.
  /// You can not add an element to target if until that subtarget is
  /// finished.
  bool isLocked() const { return false; }

  /// Return the layout
  Layout getLayout() const
  {
    if (auto* c = frame->getTop())
      return c->layout;
    return Layout::NONE;
  }

  /**
   * @brief Return the initially given dimensions
   *
   * The x and y are relative to its parent, if any.
   *
   * The w and h are its size. A 0 value in either of these means the group will
   * change it to what it considers good values for them, respectively.
   */
  const SDL_Rect& getRect() const
  {
    if (auto* c = frame->getTop())
      return c->eventTarget.rect();
    static SDL_Rect defaultRect{0, 0, Undefined, Undefined};
    return defaultRect;
  }

  /// Get the current size
  SDL_Point size() const
  {

    if (auto* c = frame->getTop())
      return c->size();
    return {Undefined, Undefined};
  }

  /// Get current width. This might be different than the returned by getRect()
  int width() const { return size().x; }

  /// Get the width currently occupied by elements contained in this group
  // int contentWidth() const { return bottomRight->x - topLeft->x; }

  /// Get current height. This might be different than the returned by getRect()
  int height() const { return size().y; }

  /// Get the height currently occupied by elements contained in this group
  // int contentHeight() const { return bottomRight->y - topLeft->y; }

  /// Returns true if this is valid
  operator bool() const { return frame; }

  /// Get Font
  const Font& getFont() const { return frame->getFont(); }

  int getTicks() const { return frame->getTicks(); }

private:
  Frame* frame;
  std::string_view lastElementId;
  EventTargetState lastElementState;
};

inline MouseAction
Target::checkMouse(std::string_view id, SDL_Rect r)
{
  auto& elState = check(id, r);
  switch (elState.event) {
  case Event::GRAB:
    return MouseAction::GRAB;
  case Event::ACTION:
    return MouseAction::ACTION;
  case Event::CANCEL:
    return MouseAction::CANCEL;
  default:
    if (!elState.status.test(Status::GRABBED)) {
      return MouseAction::NONE;
    }
    return elState.status.test(Status::HOVERED) ? MouseAction::HOLD
                                                : MouseAction::DRAG;
  }
}

inline TextAction
Target::checkText(std::string_view id)
{
  auto& elState = check(id, {});
  switch (elState.event) {
  case Event::INPUT:
    return TextAction::INPUT;
  case Event::END_LINE:
  case Event::SPACE:
  case Event::BACKSPACE:
    return TextAction::KEYDOWN;
  default:
    return TextAction::NONE;
  }
}

} // namespace boto

#endif // BOTO_TARGET_HPP_
