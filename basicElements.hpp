#ifndef DUI_BASIC_WIDGETS_HPP_
#define DUI_BASIC_WIDGETS_HPP_

#include <string_view>
#include "Group.hpp"
#include "WrapperGroup.hpp"

namespace dui {

namespace style {

constexpr SDL_Color TEXT{0, 0, 0, 255};
constexpr SDL_Color BUTTON{224, 224, 224, 255};
constexpr SDL_Color BUTTON_ACTIVE{208, 208, 208, 255};
constexpr SDL_Color BUTTON_LIGHT{255, 255, 255, 255};
constexpr SDL_Color BUTTON_DARK{0, 0, 0, 255};
constexpr SDL_Color INPUTBOX{240, 240, 240, 255};
constexpr SDL_Color INPUTBOX_ACTIVE{255, 255, 255, 255};
constexpr SDL_Color INPUTBOX_BORDER{0, 0, 0, 255};

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
  SDL_assert(!target.isLocked());
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
  SDL_assert(!target.isLocked());
  target.advance({p.x + 8, p.y + 8});
  auto caret = target.getCaret();
  state.display(Shape::Character({caret.x + p.x, caret.y + p.y}, c, ch));
}

inline void
text(Group& target, std::string_view text, SDL_Point p, SDL_Color c)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  target.advance({p.x + 8 * int(text.size()), p.y + 8});
  auto caret = target.getCaret();
  for (auto ch : text) {
    state.display(Shape::Character({caret.x + p.x, caret.y + p.y}, c, ch));
    p.x += 8;
  }
}

struct LabelStyle
{
  SDL_Color text;
  EdgeSize margin;

  constexpr LabelStyle withText(SDL_Color c) const
  {
    auto next = *this;
    next.text = c;
    return next;
  }

  constexpr LabelStyle withMargin(EdgeSize sz) const
  {
    auto next = *this;
    next.margin = sz;
    return next;
  }
};

namespace style {
constexpr LabelStyle LABEL{TEXT, EdgeSize::all(2)};
} // namespace style

inline void
label(Group& target,
      std::string_view value,
      const SDL_Point& p = {0},
      const LabelStyle& style = style::LABEL)
{
  auto adv = measure(value);
  auto margin = style.margin;
  SDL_Rect r{p.x,
             p.y,
             adv.x + margin.left + margin.right,
             adv.y + margin.top + margin.bottom};
  auto g = group(target, value, r, Layout::NONE);
  text(g, value, {margin.left, margin.top}, style.text);
  g.end();
}

struct BorderedBoxStyle
{
  SDL_Color center;
  SDL_Color left;
  SDL_Color top;
  SDL_Color right;
  SDL_Color bottom;
};

inline void
borderedBox(Group& target,
            std::string_view id,
            const SDL_Rect& r,
            const BorderedBoxStyle& style)
{
  auto c = style.center;
  auto e = style.right;
  auto n = style.top;
  auto w = style.left;
  auto s = style.bottom;
  auto g = group(target, id, {0}, Layout::NONE);
  box(g, {r.x + 1, r.y, r.w - 2, 1}, {n.r, n.g, n.b, n.a});
  box(g, {r.x, r.y + 1, 1, r.h - 2}, {w.r, w.g, w.b, w.a});
  box(g, {r.x + 1, r.y + r.h - 2 + 1, r.w - 2, 1}, {s.r, s.g, s.b, s.a});
  box(g, {r.x + r.w - 2 + 1, r.y + 1, 1, r.h - 2}, {e.r, e.g, e.b, e.a});
  box(g, {r.x + 1, r.y + 1, r.w - 2, r.h - 2}, {c.r, c.g, c.b, c.a});
  g.end();
}

struct PanelStyle
{
  BorderedBoxStyle border;
  EdgeSize padding;
};

namespace style {
constexpr PanelStyle PANEL{{{0}, TEXT, TEXT, TEXT, TEXT}, EdgeSize::all(2)};
}
/// A panel
class Panel final : public WrapperGroup
{
  PanelStyle style;

public:
  Panel(Group* parent,
        std::string_view id,
        const SDL_Rect& r,
        Layout layout,
        const PanelStyle& style)
    : WrapperGroup(parent, id, r, layout, style.padding + EdgeSize::all(1))
    , style(style)
  {}

protected:
  void afterUnwrap() final
  {
    layout = Layout::NONE;
    borderedBox(*this, "bg", {0, 0, width(), height()}, style.border);
  }
};

inline Panel
panel(Group& parent,
      std::string_view id,
      const SDL_Rect& r,
      Layout layout = Layout::VERTICAL,
      const PanelStyle& style = style::PANEL)
{
  return {&parent, id, r, layout, style};
}

struct ButtonBoxStyle
{
  SDL_Color center;
  SDL_Color topLeft;
  SDL_Color bottomRight;
};

inline void
buttonBox(Group& target, const SDL_Rect& r, const ButtonBoxStyle& style)
{
  borderedBox(target,
              "background",
              r,
              {style.center,
               style.topLeft,
               style.topLeft,
               style.bottomRight,
               style.bottomRight});
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
  auto action = g.checkMouse(id, r);
  text(g, id, {2, 2}, style::TEXT);
  bool grabbing = action == MouseAction::GRAB;
  SDL_Color base = grabbing ? style::BUTTON_ACTIVE : style::BUTTON;
  if (grabbing != inverted) {
    buttonBox(g, r, {base, style::BUTTON_DARK, style::BUTTON_LIGHT});
  } else {
    buttonBox(g, r, {base, style::BUTTON_LIGHT, style::BUTTON_DARK});
  }
  g.end();
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

struct InputBoxStyle
{
  SDL_Color center;
  SDL_Color border;
};

inline void
inputBox(Group& target,
         std::string_view id,
         const SDL_Rect& r,
         const InputBoxStyle& style)
{
  borderedBox(
    target,
    id,
    r,
    {style.center, style.border, style.border, style.border, style.border});
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
  g.checkMouse(id, r);

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

  SDL_Color bgColor = style::INPUTBOX;
  if (active) {
    // Show cursor
    box(g, {int(value.size()) * 8 + 2, 2, 1, r.h - 4}, {0, 0, 0, 255});

    // Set bg color
    bgColor = style::INPUTBOX_ACTIVE;
  }
  inputBox(g, "background", r, {bgColor, style::INPUTBOX_BORDER});
  g.end();
  return action;
}

inline bool
textBox(Group& target,
        std::string_view id,
        char* value,
        size_t maxSize,
        const SDL_Rect& r = {0})
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
        const SDL_Rect& r = {0})
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

inline bool
textField(Group& target,
          std::string_view id,
          std::string_view labelText,
          char* value,
          size_t maxSize,
          const SDL_Point& p = {0})
{
  SDL_Rect box{p.x, p.y, 0, 0};
  fixInputSize(&box);
  SDL_Rect r{box};
  SDL_Point labelPos = {r.w + 1, 0};
  r.w += measure(labelText).x + 1;

  auto g = group(target, {}, r, Layout::NONE);

  auto changed = textBox(g, id, value, maxSize, box);
  label(g, labelText, labelPos);
  g.end();

  return changed;
}

inline bool
textField(Group& target,
          std::string_view id,
          char* value,
          size_t maxSize,
          const SDL_Point& p = {0})
{
  return textField(target, id, id, value, maxSize, p);
}

inline bool
textField(Group& target,
          std::string_view id,
          std::string_view labelText,
          std::string* value,
          const SDL_Point& p = {0})
{
  SDL_Rect box{p.x, p.y, 0, 0};
  fixInputSize(&box);
  SDL_Rect r{box};
  SDL_Point labelPos = {r.w + 1, 0};
  r.w += measure(labelText).x + 1;

  auto g = group(target, {}, r, Layout::NONE);

  auto changed = textBox(g, id, value, box);
  label(g, labelText, labelPos);
  g.end();

  return changed;
}

inline bool
textField(Group& target,
          std::string_view id,
          std::string* value,
          const SDL_Point& p = {0})
{
  return textField(target, id, id, value, p);
}

// A intBox
inline bool
intBox(Group& target, std::string_view id, int* value, const SDL_Rect& r = {0})
{
  SDL_assert(value != nullptr);
  char textValue[256];
  SDL_itoa(*value, textValue, 10);
  if (textBox(target, id, textValue, 256, r)) {
    auto newValue = SDL_atoi(textValue);
    if (newValue != *value) {
      *value = newValue;
      return true;
    }
  }
  return false;
}
} // namespace dui

#endif // DUI_BASIC_WIDGETS_HPP_
