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
  State::Context context;

public:
  Frame(State* state = nullptr);
  ~Frame() { end(); }
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

protected:
  void afterLock(int deepness, const SDL_Rect& r) final { context.pushGroup(); }
  void beforeUnlock(int deepness, const SDL_Rect& r) final
  {
    context.popGroup(r);
  }
};

inline Frame
frame(State& state)
{
  return {&state};
}

inline Frame::Frame(State* state)
  : Group("", {0}, state, Layout::VERTICAL)
  , context(state->lockFrame())
{}

inline void
Frame::end()
{
  if (state) {
    context.unlockFrame();
    Group::end();
  }
}

} // namespace dui

#endif // _DUI_FRAME_HPP
