#ifndef DUI_BASE_WIDGETS_HPP_
#define DUI_BASE_WIDGETS_HPP_

#include <string_view>
#include "Group.hpp"

namespace dui {

namespace style {

constexpr SDL_Color TEXT{0, 0, 0, 255};
constexpr SDL_Color BUTTON{224, 224, 224, 255};
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
box(Group& target, SDL_Rect rect, SDL_Color color)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isBlocked());
  target.advance({rect.x + rect.w, rect.y + rect.h});
  auto caret = target.getCaret();
  rect.x += caret.x;
  rect.y += caret.y;
  state.display(rect, color);
}

inline void
character(Group& target, char ch, const SDL_Point& p, SDL_Color color)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isBlocked());
  target.advance({p.x + 8, p.y + 8});
  auto caret = target.getCaret();
  state.display({caret.x + p.x, caret.y + p.y, 8, 8}, color, ch);
}

inline void
text(Group& target, std::string_view text, SDL_Point p, SDL_Color color)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isBlocked());
  target.advance({p.x + 8 * int(text.size()), p.y + 8});
  auto caret = target.getCaret();
  for (auto ch : text) {
    state.display({caret.x + p.x, caret.y + p.y, 8, 8}, color, ch);
    p.x += 8;
  }
}

inline void
label(Group& target,
      std::string_view value,
      const SDL_Point& p = {0},
      SDL_Color color = style::TEXT)
{
  auto adv = measure(value);
  auto g = group(target, value, {p.x, p.y, adv.x + 2, adv.y + 2}, Layout::NONE);
  text(g, value, {1, 1}, color);
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
  SDL_Rect r{0, 0, adv.x + 2, adv.y + 2};
  auto action = g.testMouse(id, r);
  text(g, id, {1, 1}, style::TEXT);
  if ((action == MouseAction::GRAB) != inverted) {
    renderButtonPressed(g, r);
  } else {
    renderButton(g, r);
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

inline bool
textBox(Group& target,
        std::string_view id,
        char* value,
        size_t maxSize,
        SDL_Rect r)
{
  if (r.w == 0 || r.h == 0) {
    auto sz = measure('m'); // TODO allow customization for this
    if (r.w == 0) {
      r.w = sz.x * 16 + 2;
    }
    if (r.h == 0) {
      r.h = sz.y + 2;
    }
  }
  auto g = group(target, id, r, Layout::NONE);
  r.x = r.y = 0;
  g.testMouse(id, r);
  bool active = g.isActive(id);
  if (active && g.hasText()) {
    auto input = g.getText();
    if (!input.empty() || maxSize == 0) {
      auto len = strlen(value);
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
    }
  }
  text(g, value, {1, 1}, style::TEXT);
  renderInput(
    g, r, active ? style::INPUT_ACTIVE : style::INPUT, style::INPUT_BORDER);
  return false;
}

} // namespace dui

#endif // DUI_BASE_WIDGETS_HPP_
