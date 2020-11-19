#ifndef DUI_STATE_HPP_
#define DUI_STATE_HPP_

#include "DisplayList.hpp"

namespace dui {
// Forward decl
class Frame;

class State
{
private:
  bool inFrame = false;
  SDL_Renderer* renderer;
  DisplayList dList;

public:
  State(SDL_Renderer* renderer)
    : renderer(renderer)
  {}

  void render()
  {
    SDL_assert(!inFrame);
    dList.render(renderer);
  }

private:
  friend class Frame;

  void beginFrame()
  {
    inFrame = true;
    dList.clear();
  }

  void endFrame() { inFrame = false; }
};
} // namespace dui

#endif // DUI_STATE_HPP_
