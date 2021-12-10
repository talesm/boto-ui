#include <SDL.h>
#include "boto.hpp"

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
  boto::State state{renderer};

  // variables
  SDL_Point var1 = {0, 10};

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
    auto f = boto::frame(state);

    // UI
    auto g = boto::group(f, "main", {0});
    boto::numberField(g, "intVar1", &var1.y);
    boto::sliderBox(g, "intScroll1", &var1.y, 0, 100);
    boto::sliderBox(g, "intScroll2", &var1.y, 0, 10);
    boto::sliderBoxV(g, "intScroll3", &var1.y, 0, 100);
    if (auto sg = boto::scrollablePanel(g, "scrollGroup", &var1)) {
      for (int i = 0; i < 5; ++i) {
        boto::label(sg, "Label");
      }
    }
    boto::label(g, "End");
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
