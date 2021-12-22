#ifndef _BOTO_FRAME_HPP
#define _BOTO_FRAME_HPP

#include "Target.hpp"
#include "core/State.hpp"

namespace boto {

/**
 * @brief Represents a single frame on the app
 *
 */
class State::Frame : public CookieBase<State, State::FrameGuard>
{
public:
  constexpr Frame() = default;

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

  /// Convert to target
  operator Target() &
  {
    return {get(), {}, rect, topLeft, bottomRight, locked, {0, Layout::NONE}};
  }

private:
  Frame(State* state)
    : CookieBase(state)
  {}

  friend class State;

  SDL_Rect rect{0};
  SDL_Point topLeft{0};
  SDL_Point bottomRight{0};
  bool locked = false;
};

using Frame = State::Frame;

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

inline State::Frame
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

#endif // _BOTO_FRAME_HPP
