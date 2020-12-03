#include <iostream>
#include <SDL.h>
#include "dui.hpp"

void
yesOrNo(dui::Group& target, bool value)
{
  if (value) {
    dui::label(
      target, "Yes", {5}, dui::style::LABEL.withText({20, 128, 100, 255}));
  } else {
    dui::label(
      target, "No", {5}, dui::style::LABEL.withText({200, 100, 20, 255}));
  }
}

int
main(int argc, char** argv)
{
  using namespace std;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    cerr << SDL_GetError() << '\n';
    return 1;
  }
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  if (SDL_CreateWindowAndRenderer(
        800, 600, SDL_WINDOW_SHOWN, &window, &renderer) < 0) {
    cerr << SDL_GetError() << '\n';
    return 1;
  }

  // The ui state
  dui::State state{renderer};

  for (;;) {
    bool mouseFocus = state.wantsMouse();
    bool keyboardFocus = state.wantsKeyboard();

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      state.event(ev);
      if (ev.type == SDL_QUIT) {
        return 0;
      }
    }

    // UI
    auto f = dui::frame(state);
    dui::label(f, "This doesn't take focus", {350, 200});
    dui::button(f, "This takes focus", {350, 220});
    auto p = dui::panel(f, "mainPanel", {10, 10, 300, 500});
    dui::label(p, "Mouse Focus");
    yesOrNo(p, mouseFocus);
    dui::label(p, "Keyboard Focus");
    yesOrNo(p, keyboardFocus);
    dui::button(p, "dummy button");
    static std::string dummyText{"Ahoy"};
    dui::textField(p, "dummy text", &dummyText);

    p.end();

    // Render
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, nullptr);

    f.render();

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  return 1;
}
