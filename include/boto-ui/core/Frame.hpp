#ifndef BOTO_CORE_FRAME_HPP_
#define BOTO_CORE_FRAME_HPP_

#include "Layout.hpp"
#include "State.hpp"

namespace boto {

/**
 * @brief Represents a single frame on the app
 *
 */
class Frame
{
public:
  /// Ctors
  Frame() = default;
  Frame(CookieBase<State, State::FrameGuard> cookie)
    : cookie(std::move(cookie))
  {}

  State* get() const { return cookie.get(); }

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

  void end() { cookie.end(); }

  operator bool() const { return bool(cookie); }

private:
  CookieBase<State, State::FrameGuard> cookie;
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

#endif // BOTO_CORE_FRAME_HPP_
