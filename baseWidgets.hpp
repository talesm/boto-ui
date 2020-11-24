#ifndef DUI_BASE_WIDGETS_HPP_
#define DUI_BASE_WIDGETS_HPP_

#include <string_view>
#include "Group.hpp"

namespace dui {

namespace style {

constexpr SDL_Color TEXT{0, 0, 0, 255};
constexpr SDL_Color BUTTON{224, 224, 224, 255};
constexpr SDL_Color BUTTON_ACTIVE{208, 208, 208, 255};
constexpr SDL_Color BUTTON_LIGHT{255, 255, 255, 255};
constexpr SDL_Color BUTTON_DARK{0, 0, 0, 255};
constexpr SDL_Color INPUT{240, 240, 240, 255};
constexpr SDL_Color INPUT_ACTIVE{255, 255, 255, 255};
constexpr SDL_Color INPUT_BORDER{0, 0, 0, 255};

}

SDL_Point
measure(char ch)
{
  return {8, 8};
}

SDL_Point
measure(std::string_view text)
{
  return {int(8 * text.size()), 8};
}

inline void
box(Group& target, SDL_Rect rect, SDL_Color c)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isBlocked());
  target.advance({rect.x + rect.w, rect.y + rect.h});
  auto caret = target.getCaret();
  rect.x += caret.x;
  rect.y += caret.y;
  state.display(Shape::Box(rect, c));
}

inline void
character(Group& target, char ch, const SDL_Point& p, SDL_Color c)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isBlocked());
  target.advance({p.x + 8, p.y + 8});
  auto caret = target.getCaret();
  state.display(Shape::Character({caret.x + p.x, caret.y + p.y}, c, ch));
}

inline void
text(Group& target, std::string_view text, SDL_Point p, SDL_Color c)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isBlocked());
  target.advance({p.x + 8 * int(text.size()), p.y + 8});
  auto caret = target.getCaret();
  for (auto ch : text) {
    state.display(Shape::Character({caret.x + p.x, caret.y + p.y}, c, ch));
    p.x += 8;
  }
}

inline void
label(Group& target,
      std::string_view value,
      const SDL_Point& p = {0},
      SDL_Color c = style::TEXT)
{
  auto adv = measure(value);
  auto g = group(target, value, {p.x, p.y, adv.x + 4, adv.y + 4}, Layout::NONE);
  text(g, value, {2, 2}, c);
}

inline void
renderButton(Group& target,
             const SDL_Rect& r,
             SDL_Color b = style::BUTTON,
             SDL_Color l = style::BUTTON_LIGHT,
             SDL_Color d = style::BUTTON_DARK)
{
  auto g = group(target, "background", {0}, Layout::NONE);
  box(g, {r.x + 1, r.y, r.w - 2, 1}, {l.r, l.g, l.b, l.a});
  box(g, {r.x, r.y + 1, 1, r.h - 2}, {l.r, l.g, l.b, l.a});
  box(g, {r.x + 1, r.y + r.h - 2 + 1, r.w - 2, 1}, {d.r, d.g, d.b, d.a});
  box(g, {r.x + r.w - 2 + 1, r.y + 1, 1, r.h - 2}, {d.r, d.g, d.b, d.a});
  box(g, {r.x + 1, r.y + 1, r.w - 2, r.h - 2}, {b.r, b.g, b.b, b.a});
}

inline void
renderButtonPressed(Group& target,
                    const SDL_Rect& r,
                    SDL_Color b = style::BUTTON,
                    SDL_Color l = style::BUTTON_LIGHT,
                    SDL_Color d = style::BUTTON_DARK)
{
  renderButton(target, r, b, d, l);
}

inline bool
button(Group& target,
       std::string_view id,
       bool inverted,
       const SDL_Point& p = {0})
{
  auto g = group(target, id, {p.x, p.y}, Layout::NONE);
  auto adv = measure(id);
  SDL_Rect r{0, 0, adv.x + 4, adv.y + 4};
  auto action = g.testMouse(id, r);
  text(g, id, {2, 2}, style::TEXT);
  bool grabbing = action == MouseAction::GRAB;
  SDL_Color base = grabbing ? style::BUTTON_ACTIVE : style::BUTTON;
  if (grabbing != inverted) {
    renderButtonPressed(g, r, base);
  } else {
    renderButton(g, r, base);
  }
  return action == MouseAction::ACTION;
}

inline bool
button(Group& target, std::string_view id, const SDL_Point& p = {0})
{
  return button(target, id, false, p);
}

inline bool
toggleButton(Group& target,
             std::string_view id,
             bool* value,
             const SDL_Point& p = {0})
{
  if (button(target, id, *value, p)) {
    *value = !*value;
    return true;
  }
  return false;
}

template<class T, class U>
inline bool
choiceButton(Group& target,
             std::string_view id,
             T* value,
             U option,
             const SDL_Point& p = {0})
{
  bool selected = *value == option;
  if (button(target, id, selected, p) && !selected) {
    *value = option;
    return true;
  }
  return false;
}

inline void
renderInput(Group& target,
            const SDL_Rect& r,
            SDL_Color b = style::INPUT,
            SDL_Color d = style::INPUT_BORDER)
{
  auto g = group(target, "background", {0}, Layout::NONE);
  box(g, {r.x + 1, r.y, r.w - 2, 1}, {d.r, d.g, d.b, d.a});
  box(g, {r.x, r.y + 1, 1, r.h - 2}, {d.r, d.g, d.b, d.a});
  box(g, {r.x + 1, r.y + r.h - 2 + 1, r.w - 2, 1}, {d.r, d.g, d.b, d.a});
  box(g, {r.x + r.w - 2 + 1, r.y + 1, 1, r.h - 2}, {d.r, d.g, d.b, d.a});
  box(g, {r.x + 1, r.y + 1, r.w - 2, r.h - 2}, {b.r, b.g, b.b, b.a});
  g.advance({r.x + r.w, r.y + r.h});
}

inline void
fixInputSize(SDL_Rect* r)
{
  if (r->w == 0 || r->h == 0) {
    auto sz = measure('m'); // TODO allow customization for this
    if (r->w == 0) {
      r->w = sz.x * 16 + 4;
    }
    if (r->h == 0) {
      r->h = sz.y + 4;
    }
  }
}

inline TextAction
textBoxBase(Group& target,
            std::string_view id,
            std::string_view value,
            SDL_Rect r)
{
  fixInputSize(&r);
  auto g = group(target, id, r, Layout::NONE);
  r.x = r.y = 0; // Inside the group we use local coords
  g.testMouse(id, r);

  auto action = g.checkText(id);
  bool active = false;
  switch (action) {
    case TextAction::NONE:
      active = g.isActive(id);
      break;
    case TextAction::INPUT:
    case TextAction::BACKSPACE:
      active = true;
      break;
  }
  text(g, value, {2, 2}, style::TEXT);

  SDL_Color bgColor = style::INPUT;
  if (active) {
    // Show cursor
    box(g, {int(value.size()) * 8 + 2, 2, 1, r.h - 4}, {0, 0, 0, 255});

    // Set bg color
    bgColor = style::INPUT_ACTIVE;
  }
  renderInput(g, r, bgColor, style::INPUT_BORDER);
  return action;
}

inline bool
textBox(Group& target,
        std::string_view id,
        char* value,
        size_t maxSize,
        const SDL_Rect& r)
{
  auto len = strlen(value);
  switch (textBoxBase(target, id, {value, len}, r)) {
    case TextAction::NONE:
      break;
    case TextAction::INPUT:
      if (auto input = target.getText(); !input.empty() && maxSize > 0) {
        if (len >= maxSize - 1) {
          value[maxSize - 2] = input[0];
          value[maxSize - 1] = 0;
        } else {
          auto count = std::min(maxSize - len - 1, input.size());
          for (size_t i = 0; i < count; ++i) {
            value[len + i] = input[i];
          }
          value[len + count] = 0;
        }
        return true;
      }
      break;
    case TextAction::BACKSPACE:
      if (len > 0) {
        value[len - 1] = 0;
        return true;
      }
      break;
  }
  return false;
}

inline bool
textBox(Group& target,
        std::string_view id,
        std::string* value,
        const SDL_Rect& r)
{
  switch (textBoxBase(target, id, *value, r)) {
    case TextAction::NONE:
      break;
    case TextAction::INPUT:
      if (auto input = target.getText(); !input.empty()) {
        *value += input;
        return true;
      }
      break;
    case TextAction::BACKSPACE:
      if (value->size() > 0) {
        value->pop_back();
        return true;
      }
      break;
  }
  return false;
}
} // namespace dui

#endif // DUI_BASE_WIDGETS_HPP_
