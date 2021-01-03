#ifndef DUI_SCROLLBAR_HPP_
#define DUI_SCROLLBAR_HPP_

#include <algorithm>
#include <SDL.h>
#include "Button.hpp"
#include "Group.hpp"
#include "ScrollBarStyle.hpp"
#include "Target.hpp"

namespace dui {

inline bool
scrollBarSlider(Target target,
                std::string_view id,
                int* value,
                int min,
                int max,
                SDL_Rect r = {0},
                const ScrollBarSliderStyle& style = themeFor<ScrollBarSlider>())
{
  SDL_assert(value != nullptr);
  auto g = panel(target, id, r, Layout::NONE, style.panel);

  // TODO handle min>=max
  int distance = max - min;
  int cursorWidth = std::max(r.w / distance, style.minCursor);
  int cursorMax = r.w - cursorWidth;
  int cursorPos =
    std::clamp((*value - min) * cursorMax / distance, 0, cursorMax);
  box(g, {cursorPos - 1, -1, cursorWidth, r.h}, style.cursor);
  g.end();
  return false;
}

inline bool
scrollBar(Target target,
          std::string_view id,
          int* value,
          int min,
          int max,
          SDL_Rect r = {0},
          const ScrollBarStyle& style = themeFor<ScrollBar>())
{
  if (r.w == 0) {
    r.w = makeInputSize({r.w, r.h},
                        style.buttons.font,
                        0,
                        style.buttons.border + style.buttons.padding)
            .x;
  }
  auto& buttonStyle = style.buttons;
  int buttonWidth = buttonStyle.padding.left + buttonStyle.padding.right +
                    buttonStyle.border.left + buttonStyle.border.right + 8;
  if (r.w < buttonWidth * 4) {
    r.w = buttonWidth * 4;
  }
  int buttonHeight = buttonStyle.padding.top + buttonStyle.padding.bottom +
                     buttonStyle.border.top + buttonStyle.border.bottom + 8;
  if (buttonHeight < r.h) {
    buttonHeight = r.h;
  } else {
    r.h = buttonHeight;
  }
  bool action = false;
  Group g = group(target, id, r, Layout::NONE);
  if (button(g, "prev", "<")) {
    *value = *value > min ? *value - 1 : min;
    action = true;
  }
  if (button(g, "next", ">", {r.w - buttonWidth, 0})) {
    *value = *value < max ? *value + 1 : max;
    action = true;
  }
  action |= scrollBarSlider(
    g,
    "bar",
    value,
    min,
    max,
    {buttonWidth - 1, 0, r.w - buttonWidth * 2 + 2, buttonHeight},
    style.bar);
  g.end();
  return action;
}

} // namespace dui

#endif // DUI_SCROLLBAR_HPP_
