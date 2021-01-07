#ifndef _DUI_FRAME_HPP
#define _DUI_FRAME_HPP

#include "State.hpp"
#include "Target.hpp"

namespace dui {

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
  Frame(State* state = nullptr);
  Frame(const Frame&) = delete;
  Frame(Frame&& rhs) = default;
  Frame& operator=(const Frame&) = delete;
  Frame& operator=(Frame&& rhs) = default;

  void render()
  {
    SDL_assert(state != nullptr);
    auto& state = *this->state;
    end();
    state.render();
  }

  // It is called by render normally
  void end();

  operator Target() &
  {
    return {state, {}, rect, topLeft, bottomRight, locked, {0, Layout::NONE}};
  }
};

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

} // namespace dui

#endif // _DUI_FRAME_HPP
