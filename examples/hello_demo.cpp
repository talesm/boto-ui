#include <iostream>
#include <SDL.h>
#include "dui.hpp"

int
main(int argc, char** argv)
{
  using namespace std;

  // Init SDL
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

  // Create ui state
  dui::State state{renderer};

  for (;;) {

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      state.event(ev);
      if (ev.type == SDL_QUIT) {
        return 0;
      }
    }

    // Add elements
    auto f = dui::frame(state);
    dui::label(f, "Hello World", {350, 200});
    if (dui::button(f, "Close App", {350, 220})) {
      SDL_Event ev{0};
      ev.type = SDL_QUIT;
      SDL_PushEvent(&ev);
    }

    // Render
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, nullptr);

    f.render();

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  return 1;
}
