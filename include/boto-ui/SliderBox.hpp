#ifndef BOTO_SLIDERBOX_HPP_
#define BOTO_SLIDERBOX_HPP_

#include <algorithm>
#include <optional>
#include <SDL.h>
#include "Button.hpp"
#include "Group.hpp"
#include "SliderBoxStyle.hpp"
#include "Target.hpp"

namespace boto {

/// Returns delta
inline std::optional<SDL_Point>
sliderBoxBarCaret(Target target,
                  std::string_view id,
                  const SDL_Rect& r,
                  const ElementStyle& style = themeFor<Element>())
{
  element(target, r, style);
  static SDL_Point mouseOffset;
  auto action = target.checkMouse(id, r);
  if (action == MouseAction::HOLD) {
    return {{0, 0}};
  }
  if (action == MouseAction::GRAB) {
    auto pos = target.lastMousePos();
    mouseOffset = {pos.x - r.x, pos.y - r.y};
    return {{0, 0}};
  }
  if (action != MouseAction::DRAG) {
    return {};
  }
  auto pos = target.lastMousePos();
  SDL_Point delta{pos.x - r.x - mouseOffset.x, pos.y - r.y - mouseOffset.y};
  if (delta.x > 0 ? pos.x < r.x : pos.x > r.x) {
    delta.x = 0;
  }
  if (delta.y > 0 ? pos.y < r.y : pos.y > r.y) {
    delta.y = 0;
  }
  return delta;
}

/// SliderBoxBar's orientation
enum Orientation : bool
{
  HORIZONTAL,
  VERTICAL,
};

/// The draggable bar part of a slider box
inline bool
sliderBoxBar(Target target,
             std::string_view id,
             int* value,
             int min,
             int max,
             const SDL_Rect& r,
             Orientation orientation,
             const SliderBoxBarStyle& style = themeFor<SliderBoxBar>())
{
  SDL_assert(value != nullptr);
  auto g = panel(target, id, r, Layout::NONE, style.panel);

  // TODO handle min>=max
  int distance = max - min;
  int cursorMax;
  SDL_Rect cursorRect;
  if (orientation == HORIZONTAL) {
    int cursorW = std::max(r.w / distance, style.minCursor);
    cursorMax = r.w - cursorW;
    int cursorPos =
      std::clamp((*value - min) * cursorMax / distance, 0, cursorMax);
    cursorRect = {cursorPos - 1, -1, cursorW, r.h};
  } else {
    int cursorH = std::max(r.h / distance, style.minCursor);
    cursorMax = r.h - cursorH;
    int cursorPos =
      std::clamp((*value - min) * cursorMax / distance, 0, cursorMax);
    cursorRect = {-1, cursorPos - 1, r.w, cursorH};
  }

  if (auto result = sliderBoxBarCaret(g, "caret", cursorRect, style.cursor)) {
    int delta = orientation == HORIZONTAL ? result->x * distance / cursorMax
                                          : result->y * distance / cursorMax;
    if (delta == 0) {
      return false;
    }
    *value = std::clamp(*value + delta, min, max);
    return true;
  }
  g.end();
  auto action = target.checkMouse(id, r);
  if (action != MouseAction::ACTION) {
    return false;
  }
  SDL_Point mPos = target.lastMousePos();
  int delta = std::max(distance / 8, 1);
  bool downward = orientation == HORIZONTAL ? mPos.x - r.x < cursorRect.x
                                            : mPos.y - r.y < cursorRect.y;
  if (downward) {
    if (*value == min) {
      return false;
    }
    if (*value - delta <= min) {
      *value = min;
    } else {
      *value -= delta;
    }
  } else {
    if (*value == max) {
      return false;
    }
    if (*value + delta >= max) {
      *value = max;
    } else {
      *value += delta;
    }
  }
  return true;
}

/// An horizontal slider box
/// @ingroup elements
inline bool
sliderBox(Target target,
          std::string_view id,
          int* value,
          int min,
          int max,
          SDL_Rect r = {0},
          const SliderBoxStyle& style = themeFor<SliderBox>())
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
  if (r.w < buttonWidth * 2) {
    r.w = buttonWidth * 2;
  }
  int buttonHeight = buttonStyle.padding.top + buttonStyle.padding.bottom +
                     buttonStyle.border.top + buttonStyle.border.bottom + 8;
  if (buttonHeight < r.h) {
    buttonHeight = r.h;
  } else {
    r.h = buttonHeight;
  }
  bool action = false;
  auto g = group(target, id, r, Layout::NONE);
  if (button(g, "prev", "<")) {
    *value = std::clamp(*value - 1, min, max);
    action = true;
  }
  if (button(g, "next", ">", {r.w - buttonWidth, 0})) {
    *value = std::clamp(*value + 1, min, max);
    action = true;
  }
  action |=
    sliderBoxBar(g,
                 "bar",
                 value,
                 min,
                 max,
                 {buttonWidth - 1, 0, r.w - buttonWidth * 2 + 2, buttonHeight},
                 HORIZONTAL,
                 style.bar);
  return action;
}

/// A vertical slider box
/// @ingroup elements
inline bool
sliderBoxV(Target target,
           std::string_view id,
           int* value,
           int min,
           int max,
           SDL_Rect r = {0},
           const SliderBoxStyle& style = themeFor<SliderBox>())
{
  if (r.h == 0) {
    r.h = makeInputSize({0},
                        style.buttons.font,
                        0,
                        style.buttons.border + style.buttons.padding)
            .x;
  }
  auto& buttonStyle = style.buttons;
  int buttonWidth = buttonStyle.padding.left + buttonStyle.padding.right +
                    buttonStyle.border.left + buttonStyle.border.right + 8;
  if (r.w < buttonWidth) {
    r.w = buttonWidth;
  }
  int buttonHeight = buttonStyle.padding.top + buttonStyle.padding.bottom +
                     buttonStyle.border.top + buttonStyle.border.bottom + 8;
  if (buttonHeight * 2 > r.h) {
    r.h = buttonHeight * 2;
  }
  bool action = false;
  auto g = group(target, id, r, Layout::NONE);
  if (button(g, "prev", "^")) {
    *value = std::clamp(*value - 1, min, max);
    action = true;
  }
  if (button(g, "next", "v", {0, r.h - buttonHeight})) {
    *value = std::clamp(*value + 1, min, max);
    action = true;
  }
  action |=
    sliderBoxBar(g,
                 "bar",
                 value,
                 min,
                 max,
                 {0, buttonHeight - 1, buttonWidth, r.h - buttonHeight * 2 + 2},
                 VERTICAL,
                 style.bar);
  return action;
}

} // namespace boto

#endif // BOTO_SLIDERBOX_HPP_
