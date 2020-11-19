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

  for (;;) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) {
        return 0;
      }
    }
    dui::Frame f{&state};
    dui::label(&f, "Hello world", {10, 10});
    dui::label(&f, "Hello world", {10, 20}, {0xf0, 0x80, 0x80, 0xff});

    SDL_SetRenderDrawColor(renderer, 0xfa, 0xfa, 0xd2, 0xff);
    SDL_RenderClear(renderer);

    f.render();

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  return 1;
}
