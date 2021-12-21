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
  State* state;
  SDL_Rect rect;
  SDL_Point topLeft;
  SDL_Point bottomRight;
  bool locked = false;

public:
  /// Base ctor
  Frame(State* state = nullptr);

  Frame(const Frame&) = delete;

  /// move ctor
  Frame(Frame&& rhs) = default;

  Frame& operator=(const Frame&) = delete;

  /// Assignment op
  Frame& operator=(Frame&& rhs) = default;

  /**
   * @brief Ends and then renders the frame
   *
   * This is equivalent to call end(), followed by State.render().
   */
  void render()
  {
    SDL_assert(state != nullptr);
    auto& state = *this->state;
    end();
    state.render();
  }

  /// Finishes the frame and unlock the state
  void end();

  /// Convert to target
  operator Target() &
  {
    return {state, {}, rect, topLeft, bottomRight, locked, {0, Layout::NONE}};
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
  : state(state)
  , rect({0, 0, 0, 0})
  , topLeft({0, 0})
  , bottomRight({0, 0})
{
  state->beginFrame();
}

inline void
Frame::end()
{
  state->endFrame();
  locked = false;
}

} // namespace boto

#endif // _BOTO_FRAME_HPP
