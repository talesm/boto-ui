#ifndef _DUI_FRAME_HPP
#define _DUI_FRAME_HPP

#include <string_view>
#include "State.hpp"

namespace dui {

enum class MouseAction
{
  NONE,   ///< Default status
  GRAB,   ///< The mouse grabbed this element an is holding inside its bounds
  ACTION, ///< The mouse was just released inside its bounds (do something!)
  DRAG,   ///< The mouse had this grabbed, but was moved to outside its bounds
};

/**
 * @brief Represents a single frame on the app
 *
 */
class Frame
{
  State* state;
  SDL_Point caret{0};

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

  void box(SDL_Rect rect, SDL_Color color)
  {
    SDL_assert(state->inFrame);
    rect.x += caret.x;
    rect.y += caret.y;
    state->dList.insert(rect, color, 0);
  }

  void character(const SDL_Point& p, SDL_Color color, char ch)
  {
    SDL_assert(state->inFrame);
    state->dList.insert({caret.x + p.x, caret.y + p.y, 8, 8}, color, ch);
  }

  SDL_Point measure(char ch) { return {8, 8}; }

  void string(SDL_Point p, SDL_Color color, std::string_view text)
  {
    SDL_assert(state->inFrame);

    for (auto ch : text) {
      state->dList.insert({caret.x + p.x, caret.y + p.y, 8, 8}, color, ch);
      p.x += 8;
    }
  }

  SDL_Point measure(std::string_view text) { return {int(8 * text.size()), 8}; }

  MouseAction testMouse(std::string_view id, SDL_Rect r);

  void advance(const SDL_Point& p) { caret.y += p.y + 2; }
};

inline Frame
frame(State& state)
{
  return {&state};
}

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
  caret = rhs.caret;
  rhs.state = nullptr;
}

inline Frame&
Frame::operator=(Frame&& rhs)
{
  std::swap(state, rhs.state);
  caret = rhs.caret;
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
  caret = {0};
}

inline MouseAction
Frame::testMouse(std::string_view id, SDL_Rect r)
{
  r.x += caret.x;
  r.y += caret.y;
  if (state->mGrabbed.empty()) {
    if (state->mLeftPressed && SDL_PointInRect(&state->mPos, &r)) {
      state->mGrabbed = id;
      return MouseAction::GRAB;
    }
    return MouseAction::NONE;
  }
  if (state->mGrabbed != id) {
    return MouseAction::NONE;
  }
  if (state->mLeftPressed) {
    return SDL_PointInRect(&state->mPos, &r) ? MouseAction::GRAB
                                             : MouseAction::DRAG;
  }
  state->mGrabbed.clear();
  return SDL_PointInRect(&state->mPos, &r) ? MouseAction::ACTION
                                           : MouseAction::NONE;
}

} // namespace dui

#endif // _DUI_FRAME_HPP
