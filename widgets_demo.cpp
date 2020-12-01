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

  // The ui state
  dui::State state{renderer};

  // Some test variables
  bool toggleOption = false;
  enum MultiOption
  {
    OPTION1,
    OPTION2,
    OPTION3,
  };
  MultiOption multiOption{};

  constexpr size_t str1Size = 100;
  char str1[str1Size] = "str1";
  std::string str2 = "str2";
  int value1 = 42;
  double value2 = 11.25;

  for (;;) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      state.event(ev);
      if (ev.type == SDL_QUIT) {
        return 0;
      }
    }

    // UI
    auto f = dui::frame(state);
    auto p = dui::panel(f, "mainPanel", {10, 10, 300, 500});
    dui::label(p, "Hello world");
    dui::label(p, "Hello world", {5});
    dui::label(p,
               "Hello world",
               {},
               dui::style::LABEL.withText({0xf0, 0x80, 0x80, 0xff}));
    if (dui::button(p, "Click me!")) {
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                               "Clicked",
                               "You clicked the button",
                               window);
    }
    dui::label(p, toggleOption ? "activated" : "not activated");
    if (dui::toggleButton(p, "Toggle", &toggleOption)) {
      SDL_Log("Toggled options, new value is, %s",
              toggleOption ? "true" : "false");
    }
    if (dui::choiceButton(p, "Option 1", &multiOption, OPTION1, {0, 5})) {
      SDL_Log("Selected Option %d", 1 + multiOption);
    }
    dui::choiceButton(p, "Option 2", &multiOption, OPTION2);
    dui::choiceButton(p, "Option 3", &multiOption, OPTION3);
    if (auto g = dui::panel(p, "group1", {0, 10})) {
      dui::label(g, "Grouped Label");
      dui::button(g, "Grouped button");
      g.end();
    }
    auto panelStyle = dui::style::PANEL;
    panelStyle.border.center = {224, 255, 224, 255};
    if (auto g = dui::panel(
          p, "group2", {0, 10}, dui::Layout::HORIZONTAL, panelStyle)) {
      dui::label(g, "Grouped Label");
      dui::button(g, "Grouped button");
      g.end();
    }

    dui::label(p, "Text input", {0, 10});
    dui::textField(p, "Str1", str1, str1Size);
    dui::textField(p, "Str2", &str2);

    dui::label(p, "Number input", {0, 10});
    dui::intField(p, "value1", &value1);
    dui::doubleField(p, "value2", &value2);

    p.end();

    // Render
    SDL_SetRenderDrawColor(renderer, 0xfa, 0xfa, 0xd2, 0xff);
    SDL_RenderFillRect(renderer, nullptr);

    f.render();

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  return 1;
}
