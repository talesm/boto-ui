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

  // Other state
  bool modalOpen = true;
  bool messageOpen = false;

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

    // Add elements
    dui::label(f, "Hello World", {350, 200});
    if (dui::button(f, "open modal", {350, 220})) {
      modalOpen = true;
    }
    if (dui::button(f, "open message", {350, 240})) {
      messageOpen = true;
    }
    if (modalOpen) {
      if (auto m = dui::layer(f, "modal", {350, 198})) {
        auto g = dui::group(m, "group");
        dui::label(g,
                   "A label",
                   {0},
                   dui::themeFor<dui::Label>().withText({255, 0, 0, 255}));
        dui::button(g, "a button");
        if (dui::button(g, "close")) {
          modalOpen = false;
        }
      }
    }
    if (dui::messageBox(f, "Some message!", &messageOpen)) {
      modalOpen = true;
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
