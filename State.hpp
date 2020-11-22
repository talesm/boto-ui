#ifndef DUI_STATE_HPP_
#define DUI_STATE_HPP_

#include <SDL_events.h>
#include "DisplayList.hpp"

namespace dui {
// Forward decl
class Frame;
class Group;

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
    }
  }

  bool hasText() const { return tChanged; }
  std::string_view getText() const { return {tBuffer}; }

private:
  friend class Frame;
  friend class Group;

  void beginFrame()
  {
    inFrame = true;
    dList.clear();
  }

  void endFrame()
  {
    inFrame = false;
    tChanged = false;
  }
};
} // namespace dui

#endif // DUI_STATE_HPP_
