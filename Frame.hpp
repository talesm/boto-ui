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
class Frame : public Group
{
  State::Cookie cookie;

public:
  Frame(State* state = nullptr);
  Frame(const Frame&) = delete;
  Frame(Frame&& rhs) = default;
  Frame& operator=(const Frame&) = delete;
  Frame& operator=(Frame&& rhs) = default;

  void render()
  {
    SDL_assert(state);
    end();
    state->render();
  }

  void end();
};

inline Frame
frame(State& state)
{
  return {&state};
}

inline Frame::Frame(State* state)
  : Group("", {0}, state, Layout::VERTICAL)
  , cookie(state->lockFrame())
{}

inline void
Frame::end()
{
  cookie.unlock();
}

} // namespace dui

#endif // _DUI_FRAME_HPP
