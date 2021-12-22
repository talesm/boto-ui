#ifndef _BOTO_FRAME_HPP
#define _BOTO_FRAME_HPP

#include "Target.hpp"
#include "core/State.hpp"

namespace boto {

/**
 * @brief Represents a single frame on the app
 *
 */
class Frame
{
  State::Frame cookie;
  SDL_Rect rect{0};
  SDL_Point topLeft{0};
  SDL_Point bottomRight{0};
  bool locked = false;

public:
  /// Base ctor
  constexpr Frame() = default;
  Frame(State* state);

  /**
   * @brief Ends and then renders the frame
   *
   * This is equivalent to call end(), followed by State.render().
   */
  void render() { cookie.render(); }

  /// Finishes the frame and unlock the state
  void end() { cookie.end(); }

  /// Convert to target
  operator Target() &
  {
    return {
      cookie.get(), {}, rect, topLeft, bottomRight, locked, {0, Layout::NONE}};
  }
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
  return {&state};
}

inline Frame::Frame(State* state)
  : cookie(state->frame())
{}

} // namespace boto

#endif // _BOTO_FRAME_HPP
