#ifndef BOTO_TARGET_HPP_
#define BOTO_TARGET_HPP_

#include <string_view>
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
  EventTargetState element(std::string_view id,
                           const SDL_Rect& r,
                           RequestEvent req = RequestEvent::INPUT)
  {
    SDL_assert(state->containers.size() == stackSize);
    if (!id.empty() && id == lastElementId) {
      throw std::runtime_error{"Can not use the same id twice"};
    }
    lastElementId = id;
    return state->element(lastElementId, r, req);
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
                      RequestEvent req,
                      const SDL_Point& offset = {},
                      const SDL_Point& endPadding = {},
                      Layout layout = Layout::NONE,
                      int elementSpacing = 0)
  {
    SDL_assert(state->containers.size() == stackSize);
    lastElementId = {};
    return state->container(
      id, r, req, offset, endPadding, layout, elementSpacing);
  }

  /**
   * @brief Get the last input text
   *
   * To check if the text was for the current element and frame, use element()
   *
   * @return std::string_view
   */
  std::string_view input() const { return state->input(); }

  /**
   * @brief Last mouse position
   *
   * @return SDL_Point the last mouse pos
   */
  SDL_Point pointerPosition() const { return state->pointerPosition(); }

  /// Get the target's displayList
  DisplayList& displayList() const { return state->displayList(); }

  /// Get's theme
  Theme& theme() const { return state->theme; }

  /// Return the layout
  Layout layout() const
  {
    if (auto* c = state->getTop())
      return c->layout;
    return Layout::NONE;
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
};

} // namespace boto

#endif // BOTO_TARGET_HPP_
