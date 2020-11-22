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
  auto caret = target.getCaret();
  state.display({caret.x + p.x, caret.y + p.y, 8, 8}, color, ch);
}

inline void
text(Group& target, std::string_view text, SDL_Point p, SDL_Color color)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isBlocked());
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
  adv.x += p.x + 2;
  adv.y += p.y + 2;
  text(target, value, {p.x + 1, p.y + 1}, color);
  target.advance(adv);
}

inline void
renderButton(Group& target,
             const SDL_Rect& r,
             SDL_Color b = style::BUTTON,
             SDL_Color l = style::BUTTON_LIGHT,
             SDL_Color d = style::BUTTON_DARK)
{
  box(target, {r.x + 1, r.y, r.w - 2, 1}, {l.r, l.g, l.b, l.a});
  box(target, {r.x, r.y + 1, 1, r.h - 2}, {l.r, l.g, l.b, l.a});
  box(target, {r.x + 1, r.y + r.h - 2 + 1, r.w - 2, 1}, {d.r, d.g, d.b, d.a});
  box(target, {r.x + r.w - 2 + 1, r.y + 1, 1, r.h - 2}, {d.r, d.g, d.b, d.a});
  box(target, {r.x + 1, r.y + 1, r.w - 2, r.h - 2}, {b.r, b.g, b.b, b.a});
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
  auto adv = measure(id);
  SDL_Rect r{p.x, p.y, adv.x + 2, adv.y + 2};
  auto action = target.testMouse(id, r);
  text(target, id, {p.x + 1, p.y + 1}, style::TEXT);
  if ((action == MouseAction::GRAB) != inverted) {
    renderButtonPressed(target, r);
  } else {
    renderButton(target, r);
  }
  adv.x += p.x + 2;
  adv.y += p.y + 2;
  target.advance(adv);
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
  box(target, {r.x + 1, r.y, r.w - 2, 1}, {d.r, d.g, d.b, d.a});
  box(target, {r.x, r.y + 1, 1, r.h - 2}, {d.r, d.g, d.b, d.a});
  box(target, {r.x + 1, r.y + r.h - 2 + 1, r.w - 2, 1}, {d.r, d.g, d.b, d.a});
  box(target, {r.x + r.w - 2 + 1, r.y + 1, 1, r.h - 2}, {d.r, d.g, d.b, d.a});
  box(target, {r.x + 1, r.y + 1, r.w - 2, r.h - 2}, {b.r, b.g, b.b, b.a});
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
  target.testMouse(id, r);
  bool active = target.isActive(id);
  if (active && target.hasText()) {
    auto input = target.getText();
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
  text(target, value, {r.x + 1, r.y + 1}, style::TEXT);
  renderInput(target,
              r,
              active ? style::INPUT_ACTIVE : style::INPUT,
              style::INPUT_BORDER);
  return false;
}

} // namespace dui

#endif // DUI_BASE_WIDGETS_HPP_
