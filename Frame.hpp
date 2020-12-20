#ifndef _DUI_FRAME_HPP
#define _DUI_FRAME_HPP

#include <string_view>
#include "Group.hpp"
#include "State.hpp"

namespace dui {

/**
 * @brief Represents a single frame on the app
 *
 */
class Frame
{
  State::Context context;
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
    SDL_assert(context.valid());
    auto& state = context.getState();
    end();
    state.render();
  }

  // It is called by render normally
  void end();

  operator Target() &
  {
    return {
      &context.getState(),
      {},
      rect,
      topLeft,
      bottomRight,
      Layout::NONE,
      locked,
    };
  }
};

inline Frame
frame(State& state)
{
  return {&state};
}

inline Frame::Frame(State* state)
  : context(state->lockFrame())
  , rect({0, 0, 0, 0})
  , topLeft({0, 0})
  , bottomRight({0, 0})
{}

inline void
Frame::end()
{
  context.unlockFrame();
  locked = false;
}

} // namespace dui

#endif // _DUI_FRAME_HPP
