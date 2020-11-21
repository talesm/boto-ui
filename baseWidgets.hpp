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

}

inline void
renderLabel(Group& target,
            std::string_view text,
            const SDL_Point& p,
            SDL_Color color = style::TEXT)
{
  target.string(p, color, text);
}

inline void
label(Group& target,
      std::string_view text,
      const SDL_Point& p = {0},
      SDL_Color color = style::TEXT)
{
  auto adv = target.measure(text);
  adv.x += p.x + 2;
  adv.y += p.y + 2;
  renderLabel(target, text, {p.x + 1, p.y + 1}, color);
  target.advance(adv);
}

inline void
renderButton(Group& target,
             const SDL_Rect& r,
             SDL_Color b = style::BUTTON,
             SDL_Color l = style::BUTTON_LIGHT,
             SDL_Color d = style::BUTTON_DARK)
{
  target.box({r.x + 1, r.y, r.w - 2, 1}, {l.r, l.g, l.b, l.a});
  target.box({r.x, r.y + 1, 1, r.h - 2}, {l.r, l.g, l.b, l.a});
  target.box({r.x + 1, r.y + r.h - 2 + 1, r.w - 2, 1}, {d.r, d.g, d.b, d.a});
  target.box({r.x + r.w - 2 + 1, r.y + 1, 1, r.h - 2}, {d.r, d.g, d.b, d.a});
  target.box({r.x + 1, r.y + 1, r.w - 2, r.h - 2}, {b.r, b.g, b.b, b.a});
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
       std::string_view text,
       bool inverted,
       const SDL_Point& p = {0})
{
  auto adv = target.measure(text);
  SDL_Rect r{p.x, p.y, adv.x + 2, adv.y + 2};
  auto action = target.testMouse(text, r);
  renderLabel(target, text, {p.x + 1, p.y + 1}, style::TEXT);
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
button(Group& target, std::string_view text, const SDL_Point& p = {0})
{
  return button(target, text, false, p);
}

inline bool
toggleButton(Group& target,
             std::string_view text,
             bool* value,
             const SDL_Point& p = {0})
{
  if (button(target, text, *value, p)) {
    *value = !*value;
    return true;
  }
  return false;
}

template<class T, class U>
inline bool
choiceButton(Group& target,
             std::string_view text,
             T* value,
             U option,
             const SDL_Point& p = {0})
{
  bool selected = *value == option;
  if (button(target, text, selected, p) && !selected) {
    *value = option;
    return true;
  }
  return false;
}

} // namespace dui

#endif // DUI_BASE_WIDGETS_HPP_
