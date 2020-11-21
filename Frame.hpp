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
struct Frame : Group
{
  Frame(State* state = nullptr);
  ~Frame();
  Frame(const Frame&) = delete;
  Frame(Frame&& rhs) = default;
  Frame& operator=(const Frame&) = delete;
  Frame& operator=(Frame&& rhs) = default;

  void render()
  {
    SDL_assert(state);
    auto st = state;
    end();
    st->render();
  }

  void reset(State* state);
  void end();
};

inline Frame
frame(State& state)
{
  return {&state};
}

inline Frame::Frame(State* state)
  : Group("", {0}, state)
{
  if (state != nullptr) {
    SDL_assert(state->inFrame == false);
    state->beginFrame();
  }
}

inline Frame::~Frame()
{
  if (state != nullptr) {
    SDL_assert(state->inFrame == true);
    state->endFrame();
  }
}

inline void
Frame::end()
{
  this->~Frame();
  state = nullptr;
}

inline void
Frame::reset(State* state)
{
  this->~Frame();
  new (this) Frame(state);
  Group::reset();
}

} // namespace dui

#endif // _DUI_FRAME_HPP
