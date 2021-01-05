#include <SDL.h>
#include "dui.hpp"

int
main(int argc, char** argv)
{
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "%s\n", SDL_GetError());
    return 1;
  }

  // Create window and renderer
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  if (SDL_CreateWindowAndRenderer(
        800, 600, SDL_WINDOW_SHOWN, &window, &renderer) < 0) {
    fprintf(stderr, "%s\n", SDL_GetError());
    return 1;
  }

  // Create ui state
  dui::State state{renderer};

  // variables
  int intVar = 10;

  // Main loop
  for (;;) {
    // Event handling
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      // Send event to the state
      state.event(ev);

      // Normal event handling
      if (ev.type == SDL_QUIT) {
        return 0;
      }
    }

    // Begin Frame
    auto f = dui::frame(state);

    // UI
    auto g = dui::group(f, "main", {0});
    dui::numberField(g, "intVar1", &intVar);
    dui::sliderBox(g, "intScroll1", &intVar, 0, 100);
    dui::sliderBox(g, "intScroll2", &intVar, 0, 10);
    dui::sliderBoxV(g, "intScroll3", &intVar, 0, 100);
    g.end();

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, nullptr);

    // End frame and render state
    f.render();

    // Present and wait a bit
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  return 1;
}
