#include <SDL.h>
#include "boto.hpp"

void
yesOrNo(boto::Target target, bool value)
{
  if (value) {
    boto::label(target,
                "Yes",
                {5},
                boto::themeFor<boto::Label>().withText({20, 128, 100, 255}));
  } else {
    boto::label(target,
                "No",
                {5},
                boto::themeFor<boto::Label>().withText({200, 100, 20, 255}));
  }
}

int
main(int argc, char** argv)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "%s\n", SDL_GetError());
    return 1;
  }
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  if (SDL_CreateWindowAndRenderer(
        800, 600, SDL_WINDOW_SHOWN, &window, &renderer) < 0) {
    fprintf(stderr, "%s\n", SDL_GetError());
    return 1;
  }

  // The ui state
  boto::State state{renderer};

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
    auto f = boto::frame(state);
    boto::label(f, "This doesn't take focus", {350, 200});
    boto::button(f, "This takes focus", {350, 220});
    if (auto p = boto::panel(f, "mainPanel", {10, 10, 300, 500})) {
      ;
      boto::label(p, "Mouse Focus");
      yesOrNo(p, mouseFocus);
      boto::label(p, "Keyboard Focus");
      yesOrNo(p, keyboardFocus);
      boto::button(p, "dummy button");
      static std::string dummyText{"Ahoy"};
      boto::textField(p, "dummy text", &dummyText);
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
