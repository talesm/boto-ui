Boto User Interface
=====================

[Click here to GET the latest single header file][dui_single]

A simple immediate mode user interface designed for using with [SDL2][SDL2] 2d
renderer.

![Elements demo](examples/elements_demo.png)

The project also can be used as it is, just by [downloading][dui_single] or
checking out the current version. Be aware though that it might have some
breakage and bugs as it still a very early stage project;

[SDL2]: https://www.libsdl.org/
[dui_single]: https://github.com/talesm/boto/releases/download/v0.3/dui_single.hpp

Introduction
------------

We aim to provide a simple non intrusive way to define simple user interfaces,
based on rows. The following is a minimal example:

```cpp
#include <SDL.h>
#include "boto.hpp"

int
main(int argc, char** argv)
{
   // Init SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &window, &renderer);
  
  // Create ui state
  boto::State state{renderer};

  // Main loop
  for (;;) {
    //Event handling
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      // Send event to the state
      state.event(ev);

      if (ev.type == SDL_QUIT) {
        return 0;
      }
    }

    // Begin frame
    auto f = boto::frame(state);

    // Add elements
    boto::label(f, "Hello World", {350, 200});
    if (boto::button(f, "Close App", {350, 220})) {
      return 0;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, nullptr);

    // End frame and render state
    f.render();

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  return 1;
}
```

Let's dissect the code. Firstly we have the include. We just use the `boto.hpp`
that includes all the necessary files. We could also use the boto
`dui_single.hpp` that is the single file version so it is easier to attach on a
project.

```cpp
#include "boto.hpp"
```

Then we create the State just before the main loop. A state holds all persistent
UI data, like the mouse position, if it is pressed, as well as the current
active element and so on. Think it as the main ui component.

```cpp
  // Create ui state
  boto::State state{renderer};

  // Main loop
  for (;;) {
    ...
  }
```

We need to send the events to the state, so it knowns the mouse and keyboard
status and store it for the elements.

```cpp
    //Event handling
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      // Send event to the state
      state.event(ev);

      ...
    }
```

After all events are received, we can then
begin the frame and add elements:

```cpp
    // Begin frame
    auto f = boto::frame(state);

    // Add elements
    boto::label(f, "Hello World", {350, 200});
    if (boto::button(f, "Close App", {350, 220})) {
      return 0;
    }
```

The frame starts with the creation of a Frame object, which is done by frame()
auxiliary function. Then when appropriated, we can call render() on it, which
ends the frame and render it.

```cpp
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, nullptr);

    // End frame and render state
    f.render();
```

This is all what we need to get the following rendering. The button
even works!

![Hello world](examples/hello_demo.png)

You can see this example complete with better error handling at
[hello_demo.cpp][hello_demo]

[hello_demo]: examples/hello_demo.cpp

### How to know when DUI is using the Mouse and Keyboard

In many situations, you don't want to delegate all interaction for boto, but
manage some interactions by yourself. There are two methods that help with this,
wantsMouse() and wantsKeyboard(). You can call these before instantiating the
Frame to know if the mouse or keyboard are being used. As an example let's
modify our previous example by adding this:

```cpp
  // main loop
  for(;;) {
    ...

    // anywhere before frame() call
    bool mouseFocus = state.wantsMouse();
    bool keyboardFocus = state.wantsKeyboard();

    ...
  }
```

The mouse is only required if the pointer is hovering a named group or an
actionable element *or* if you are actively draging an actionable element. The
keyboard is requested only if you click an actionable element.

You could tests mouseFocus or keyboardFocus on the events to check if they're
free to you. Here in this example we want add some labels identifying if you
have either focus. Let's use this moment to introduce a new element too:

```cpp
    // Frame begin
    auto f = boto::frame(state);

    // Panel begins
    auto p = boto::panel(f, "mainPanel", {10, 10, 300, 500});

    // Panels are layout vertically by default, so no need add position
    // If you do add the position, it will be used as an offset from the panel
    boto::label(p, "Mouse Focus");
    boto::label(p, (mouseFocus ? "Yes" : "No"), {5, 0}); // 5 pixels x-offset
    boto::label(p, "Keyboard Focus");
    boto::label(p, (keyboardFocus ? "Yes" : "No"), {5, 0});
    boto::button(p, "dummy button");

    ...

    // Panel ends, you can add elements directly to f after that
    p.end();
```

If hover the mouse inside the panel, mouseFocus is true, if you hover outside,
it is false. If you press and hold the left mouse button on `dummy button`,
mouseFocus will stay true even if you move outside the panel, until you release
the button. The keyboardFocus in other hand, will only be true if you click in
the button and will stay true until you click elsewhere,

You can see this example, expanded with colors and more elements at
[focus_demo.cpp][focus_demo]. The complete example looks like the image bellow:

![Focus](examples/focus_demo.gif)

[focus_demo]: examples/focus_demo.cpp

Build
-----

You need only to have a C++17 compiler and SDL library installed. The library
itself is header only, but we use CMake to build examples and the single file
header:

### Building examples

The examples are all inside the examples subdirectory. You can build them using
the cmake file provided on DUI root directory. They're built by default.

### Building single file header

There is the custom target "single_header", that is disabled by default. It
needs a decent version of nodejs installed on the system to work.

Elements
--------

We support a short but useful number of elements. You can see a comprehensive
example at [elements_demo.cpp][elements_demo].

[![Elements demo](examples/elements_demo.png)][elements_demo]

[elements_demo]: examples/elements_demo.cpp

Contributing
------------

Just open an issue if you have a problem, and just open a pull request if you
have an solution.

Credits
-------

- font.h: CC0 (Public domain) from
  <https://opengameart.org/content/8x8-1bit-roguelike-tiles-bitmap-font>
