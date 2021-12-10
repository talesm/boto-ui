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

    // Add elements
    boto::label(f, "Hello World", {350, 200});
    if (boto::button(f, "Close App", {350, 220})) {
      // Push a QUIT event so we can see briefly the button released
      SDL_Event ev{0};
      ev.type = SDL_QUIT;
      SDL_PushEvent(&ev);
    }

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
