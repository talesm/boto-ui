#ifndef BOTO_SLIDERBOX_HPP_
#define BOTO_SLIDERBOX_HPP_

#include <algorithm>
#include <optional>
#include <SDL.h>
#include "Button.hpp"
#include "Group.hpp"
#include "core/Target.hpp"

namespace boto {

/// Returns delta
inline std::optional<SDL_Point>
sliderBoxBarCaret(Target target,
                  std::string_view id,
                  const SDL_Rect& r,
                  const ElementStyle& style)
{
  auto state = target.element(id, r, RequestEvent::GRAB);
  presentElement(target.getDisplayList(), state.rect, style);
  if (state.status != Status::GRABBED) {
    return {};
  }
  static SDL_Point mouseOffset;
  if (state.event == Event::GRAB) {
    auto pos = target.pointerPosition();
    mouseOffset = {pos.x - r.x, pos.y - r.y};
    return {{0, 0}};
  }
  auto pos = target.pointerPosition();
  SDL_Point delta{pos.x - r.x - mouseOffset.x, pos.y - r.y - mouseOffset.y};
  if (delta.x > 0 ? pos.x < r.x : pos.x > r.x) {
    delta.x = 0;
  }
  if (delta.y > 0 ? pos.y < r.y : pos.y > r.y) {
    delta.y = 0;
  }
  return delta;
}
inline std::optional<SDL_Point>
sliderBoxBarCaret(Target target, std::string_view id, const SDL_Rect& r)
{
  return sliderBoxBarCaret(target, id, r, target.styleFor<Element>());
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
             const SliderBoxBarStyle& style)
{
  SDL_assert(value != nullptr);
  auto g = panel(target, id, r, style.panel.withLayout(Layout::NONE));

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
  auto action = target.element(id, r, RequestEvent::GRAB).event;
  if (action != Event::ACTION) {
    return false;
  }
  SDL_Point mPos = target.pointerPosition();
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
inline bool
sliderBoxBar(Target target,
             std::string_view id,
             int* value,
             int min,
             int max,
             const SDL_Rect& r,
             Orientation orientation)
{
  return sliderBoxBar(target,
                      id,
                      value,
                      min,
                      max,
                      r,
                      orientation,
                      target.styleFor<SliderBoxBar>());
}

/// An horizontal slider box
/// @ingroup elements
inline bool
sliderBox(Target target,
          std::string_view id,
          int* value,
          int min,
          int max,
          SDL_Rect r,
          const SliderBoxStyle& style)
{
  auto& buttonStyle = style.buttons.normal;
  if (r.w == 0) {
    r.w = makeInputSize({r.w, r.h},
                        buttonStyle.text.font,
                        0,
                        buttonStyle.decoration.border + buttonStyle.padding)
            .x;
  }
  int buttonWidth = buttonStyle.padding.left + buttonStyle.padding.right +
                    buttonStyle.decoration.border.left +
                    buttonStyle.decoration.border.right + 8;
  if (r.w < buttonWidth * 2) {
    r.w = buttonWidth * 2;
  }
  int buttonHeight = buttonStyle.padding.top + buttonStyle.padding.bottom +
                     buttonStyle.decoration.border.top +
                     buttonStyle.decoration.border.bottom + 8;
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

inline bool
sliderBox(Target target,
          std::string_view id,
          int* value,
          int min,
          int max,
          const SDL_Rect& r = {0})
{
  return sliderBox(
    target, id, value, min, max, r, target.styleFor<SliderBox>());
}

/// A vertical slider box
/// @ingroup elements
inline bool
sliderBoxV(Target target,
           std::string_view id,
           int* value,
           int min,
           int max,
           SDL_Rect r,
           const SliderBoxStyle& style)
{
  auto& buttonStyle = style.buttons.normal;
  if (r.h == 0) {
    r.h = makeInputSize({0},
                        buttonStyle.text.font,
                        0,
                        buttonStyle.decoration.border + buttonStyle.padding)
            .x;
  }
  int buttonWidth = buttonStyle.padding.left + buttonStyle.padding.right +
                    buttonStyle.decoration.border.left +
                    buttonStyle.decoration.border.right + 8;
  if (r.w < buttonWidth) {
    r.w = buttonWidth;
  }
  int buttonHeight = buttonStyle.padding.top + buttonStyle.padding.bottom +
                     buttonStyle.decoration.border.top +
                     buttonStyle.decoration.border.bottom + 8;
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
inline bool
sliderBoxV(Target target,
           std::string_view id,
           int* value,
           int min,
           int max,
           const SDL_Rect& r = {0})
{
  return sliderBoxV(
    target, id, value, min, max, r, target.styleFor<SliderBox>());
}

} // namespace boto

#endif // BOTO_SLIDERBOX_HPP_
