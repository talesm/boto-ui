#include <iostream>
#include <SDL.h>
#include "dui.hpp"

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

  dui::State state{renderer};
  bool toggleOption = false;

  for (;;) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      state.event(ev);
      if (ev.type == SDL_QUIT) {
        return 0;
      }
    }
    auto f = dui::frame(state);
    dui::label(f, "Hello world", {10, 10});
    dui::label(f, "Hello world", {10, 0}, {0xf0, 0x80, 0x80, 0xff});
    if (dui::button(f, "Click me!", {10, 0})) {
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                               "Clicked",
                               "You clicked the button",
                               window);
    }
    dui::label(f, toggleOption ? "activated" : "not activated", {10, 0});
    if (dui::toggleButton(f, "Toggle", &toggleOption, {10, 0})) {
      SDL_Log("Toggled options, new value is, %s",
              toggleOption ? "true" : "false");
    }

    SDL_SetRenderDrawColor(renderer, 0xfa, 0xfa, 0xd2, 0xff);
    SDL_RenderClear(renderer);

    f.render();

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  return 1;
}
