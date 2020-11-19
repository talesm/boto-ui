#ifndef _DUI_FRAME_HPP
#define _DUI_FRAME_HPP

#include "State.hpp"

namespace dui {
/**
 * @brief Represents a single frame on the app
 *
 */
class Frame
{
  State* state;

public:
  Frame(State* state = nullptr);
  ~Frame();

  void render()
  {
    SDL_assert(state);
    auto st = state;
    end();
    st->render();
  }

  Frame(const Frame&) = delete;
  Frame(Frame&& rhs);
  Frame& operator=(const Frame&) = delete;
  Frame& operator=(Frame&& rhs);

  void reset(State* state);
  void end();

  void box(const SDL_Rect& rect, SDL_Color color)
  {
    SDL_assert(state->inFrame);
    state->dList.insert(rect, color, 0);
  }

  void character(const SDL_Rect& rect, SDL_Color color, char ch)
  {
    SDL_assert(state->inFrame);
    state->dList.insert(rect, color, ch);
  }
};

inline Frame::Frame(State* state)
  : state(state)
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

inline Frame::Frame(Frame&& rhs)
{
  state = rhs.state;
  rhs.state = nullptr;
}

inline Frame&
Frame::operator=(Frame&& rhs)
{
  std::swap(state, rhs.state);
  return *this;
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
}
} // namespace dui

#endif // _DUI_FRAME_HPP
