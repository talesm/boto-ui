#ifndef BOTO_TARGET_HPP_
#define BOTO_TARGET_HPP_

#include <string_view>
#include <SDL.h>
#include "core/Container.hpp"
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
    : state(nullptr)
    , stackSize(0)
  {}

  /// Ctor
  Target(Frame& frame)
    : state(frame.get())
    , stackSize(0)
  {
    SDL_assert(state->containers.size() == 0);
  }

  /// Ctor
  Target(Container& container)
    : state(container.get())
    , stackSize(state->containers.size())
  {}

  /**
   * @brief Check the leaf element for events
   *
   * @param id element id
   * @param r the element local rect (Use State.container() for global rect)
   * @param req the events it might accept
   * @return EventTargetState
   */
  const EventTargetState& check(std::string_view id,
                                const SDL_Rect& r,
                                RequestEvent req = RequestEvent::INPUT)
  {
    SDL_assert(state->containers.size() == stackSize);
    if (id.empty()) {
      lastElementState = state->element(r, req);
      lastElementId = {};
    } else if (id != lastElementId) {
      lastElementState = state->element(id, r, req);
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
    SDL_assert(state->containers.size() == stackSize);
    return state->container(id, r, offset, endPadding, layout, elementSpacing);
  }

  /**
   * @brief Get the last input text
   *
   * To check if the text was for the current element and frame, use check()
   *
   * @return std::string_view
   */
  std::string_view input() const { return state->input(); }

  /**
   * @brief Last mouse position
   *
   * @return SDL_Point the last mouse pos
   */
  SDL_Point pointerPosition() const
  {
    auto pos = state->pointerPosition();
    pos.x -= lastElementState.rect.x;
    pos.x -= lastElementState.rect.y;
    return pos;
  }

  /// Get the target's displayList
  DisplayList& getDisplayList() const { return state->displayList(); }

  /// Get the position where the next element can be added
  SDL_Point getCaret() const
  {
    if (auto* c = state->getTop())
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
    if (auto* c = state->getTop())
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
    if (auto* c = state->getTop())
      return c->eventTarget.rect();
    static SDL_Rect defaultRect{0, 0, Undefined, Undefined};
    return defaultRect;
  }

  /// Get the current size
  SDL_Point size() const
  {

    if (auto* c = state->getTop())
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
  operator bool() const { return state; }

  /// Get Font
  const Font& getFont() const { return state->getFont(); }

  int ticks() const { return state->ticks(); }

  template<class T>
  const auto& styleFor()
  {
    return state->theme.of<T>();
  }

private:
  State* state;
  size_t stackSize;
  std::string_view lastElementId;
  EventTargetState lastElementState;
};

} // namespace boto

#endif // BOTO_TARGET_HPP_
