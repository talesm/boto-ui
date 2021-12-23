#ifndef BOTO_CORE_FRAME_HPP_
#define BOTO_CORE_FRAME_HPP_

#include "Target.hpp"
#include "core/State.hpp"

namespace boto {

class ContainerState;
class Container;

/**
 * @brief Represents a single frame on the app
 *
 */
class Frame : public CookieBase<State, State::FrameGuard>
{
public:
  Frame() = default;

  // TODO remove me
  using CookieBase::get;

  /**
   * @brief Ends and then renders the frame
   *
   * This is equivalent to call end(), followed by State.render().
   */
  void render()
  {
    if (auto* state = get()) {
      end();
      state->render();
    }
  }

  DisplayList& displayList() { return get()->displayList(); }

  Container container(std::string_view id,
                      SDL_Rect r,
                      const SDL_Point& offset = {},
                      const SDL_Point& endPadding = {},
                      Layout layout = Layout::NONE,
                      int elementSpacing = 0);

  /// Convert to target
  operator Target() &
  {
    return {get(), {}, rect, topLeft, bottomRight, locked, {0, Layout::NONE}};
  }

private:
  Frame(State* state)
    : CookieBase(state)
  {}

  void popContainer();

  struct ContainerPopper
  {
    void operator()(Frame* frame) { frame->popContainer(); }
  };

  friend class State;
  friend class Container;

  SDL_Rect rect{0};
  SDL_Point topLeft{0};
  SDL_Point bottomRight{0};
  bool locked = false;
  std::vector<ContainerState> containers;
};

/**
 * @brief Starts a new frame
 *
 * The state must be unlocked, i.e no Frame object created from it must be
 * exist, or if exist it must have be finished by end().
 *
 * @param state the ui state object
 * @return Frame
 */
inline Frame
frame(State& state)
{
  return state.frame();
}

inline Frame
State::frame()
{
  SDL_assert(isInFrame() == false);
  ticksCount = SDL_GetTicks();
  levelChanged = true;
  dList.clear();
  lastId.clear();
  elements.push_back({});
  return {this};
}

inline void
State::endFrame()
{
  SDL_assert(isInFrame() == true);
  elements.pop_back();
  tKeysym = {};
  dispatcher.reset();
}
} // namespace boto

#include "Container.hpp"

#endif // BOTO_CORE_FRAME_HPP_
