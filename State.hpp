#ifndef DUI_STATE_HPP_
#define DUI_STATE_HPP_

#include <SDL_events.h>
#include "DisplayList.hpp"

namespace dui {

enum class MouseAction
{
  NONE,   ///< Default status
  GRAB,   ///< The mouse grabbed this element an is holding inside its bounds
  ACTION, ///< The mouse was just released inside its bounds (do something!)
  DRAG,   ///< The mouse had this grabbed, but was moved to outside its bounds
};

enum class TextAction
{
  NONE,      ///< Default status
  INPUT,     ///< text input
  BACKSPACE, ///< erased last character
};

class State
{
private:
  bool inFrame = false;
  SDL_Renderer* renderer;
  DisplayList dList;

  SDL_Point mPos;
  bool mLeftPressed = false;
  std::string eGrabbed;
  std::string eActive;
  char tBuffer[SDL_TEXTINPUTEVENT_TEXT_SIZE];
  bool tChanged = false;
  TextAction tAction = TextAction::NONE;

public:
  State(SDL_Renderer* renderer)
    : renderer(renderer)
  {}

  void render()
  {
    SDL_assert(!inFrame);
    dList.render(renderer);
  }

  void event(SDL_Event& ev)
  {
    if (ev.type == SDL_MOUSEBUTTONDOWN) {
      mPos = {ev.button.x, ev.button.y};
      if (ev.button.button == SDL_BUTTON_LEFT) {
        mLeftPressed = true;
      }
    } else if (ev.type == SDL_MOUSEBUTTONUP) {
      mPos = {ev.button.x, ev.button.y};
      mLeftPressed = false;
    } else if (ev.type == SDL_TEXTINPUT) {
      if (eActive.empty()) {
        return;
      }
      for (int i = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; ++i) {
        tBuffer[i] = ev.text.text[i];
        if (tBuffer[i] == 0) {
          break;
        }
      }
      tChanged = true;
      tAction = TextAction::INPUT;
    } else if (ev.type == SDL_KEYDOWN) {
      if (ev.key.keysym.sym == SDLK_BACKSPACE) {
        tChanged = true;
        tAction = TextAction::BACKSPACE;
      }
    }
  }

  bool isInFrame() const { return inFrame; }

  bool isActive(std::string_view id) const { return eActive == id; }

  /// Check mouse for given element
  MouseAction testMouse(std::string_view id, SDL_Rect r);

  TextAction checkText(std::string_view id) const
  {
    if (!tChanged || eActive != id) {
      return TextAction::NONE;
    }
    return tAction;
  }

  std::string_view getText() const { return {tBuffer}; }

  void display(const Shape& item) { dList.insert(item); }

  class Context
  {
  private:
    State* state;
    Context(State* state)
      : state(state)
    {
      state->beginFrame();
    }
    friend class State;

  public:
    ~Context() { unlockFrame(); }

    void unlockFrame()
    {
      if (state) {
        state->endFrame();
        state = nullptr;
      }
    }
    Context(const Context&) = delete;
    Context(Context&& rhs) { std::swap(state, rhs.state); }
    Context& operator=(Context rhs)
    {
      std::swap(state, rhs.state);
      return *this;
    }

    void pushGroup() { state->dList.popClip(); }
    void popGroup(const SDL_Rect& r) { state->dList.pushClip(r); }
  };

  Context lockFrame() { return Context{this}; }

private:
  void beginFrame()
  {
    SDL_assert(inFrame == false);
    inFrame = true;
    dList.clear();
  }

  void endFrame()
  {
    SDL_assert(inFrame == true);
    inFrame = false;
    tChanged = false;
  }
};

inline MouseAction
State::testMouse(std::string_view id, SDL_Rect r)
{
  SDL_assert(inFrame);
  if (eGrabbed.empty()) {
    if (mLeftPressed && SDL_PointInRect(&mPos, &r)) {
      eGrabbed = id;
      eActive = id;
      return MouseAction::GRAB;
    }
    if (mLeftPressed && eActive == id) {
      eActive.clear();
    }
    return MouseAction::NONE;
  }
  if (eGrabbed != id) {
    return MouseAction::NONE;
  }
  if (mLeftPressed) {
    return SDL_PointInRect(&mPos, &r) ? MouseAction::GRAB : MouseAction::DRAG;
  }
  eGrabbed.clear();
  if (!SDL_PointInRect(&mPos, &r)) {
    return MouseAction::NONE;
  }
  return MouseAction::ACTION;
}
} // namespace dui

#endif // DUI_STATE_HPP_
