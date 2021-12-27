#ifndef BOTO_CORE_FRAME_HPP_
#define BOTO_CORE_FRAME_HPP_

#include "Layout.hpp"
#include "State.hpp"

namespace boto {

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

  /**
   * @brief Returns the last text input
   *
   * You probably will want to check for focus before calling this
   * @return std::string_view
   */

private:
  Frame(State* state)
    : CookieBase(state){};

  friend class State;
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
  dList.clear();
  inFrame = true;
  return {this};
}

inline void
State::endFrame()
{
  SDL_assert(isInFrame() == true);
  inFrame = false;
  tKeysym = {};
  dispatcher.reset();
}
} // namespace boto

#include "Container.hpp"

#endif // BOTO_CORE_FRAME_HPP_
