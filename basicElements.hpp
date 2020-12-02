#ifndef DUI_BASIC_WIDGETS_HPP_
#define DUI_BASIC_WIDGETS_HPP_

#include <string_view>
#include "Group.hpp"
#include "WrapperGroup.hpp"
#include "label.hpp"

namespace dui {

namespace style {

constexpr SDL_Color BUTTON{224, 224, 224, 255};
constexpr SDL_Color BUTTON_ACTIVE{208, 208, 208, 255};
constexpr SDL_Color BUTTON_LIGHT{255, 255, 255, 255};
constexpr SDL_Color BUTTON_DARK{0, 0, 0, 255};
constexpr SDL_Color INPUTBOX{240, 240, 240, 255};
constexpr SDL_Color INPUTBOX_ACTIVE{255, 255, 255, 255};
constexpr SDL_Color INPUTBOX_BORDER{0, 0, 0, 255};

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
buttonBase(Group& target,
           std::string_view id,
           std::string_view label,
           bool inverted,
           const SDL_Point& p = {0})
{
  auto g = group(target, {}, {p.x, p.y}, Layout::NONE);
  auto adv = measure(label);
  SDL_Rect r{0, 0, adv.x + 4, adv.y + 4};
  auto action = g.checkMouse(id, r);
  text(g, label, {2, 2}, style::TEXT);
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
button(Group& target,
       std::string_view id,
       std::string_view label,
       const SDL_Point& p = {0})
{
  return buttonBase(target, id, label, false, p);
}

inline bool
button(Group& target, std::string_view id, const SDL_Point& p = {0})
{
  return button(target, id, id, p);
}

inline bool
toggleButton(Group& target,
             std::string_view id,
             std::string_view label,
             bool* value,
             const SDL_Point& p = {0})
{
  if (buttonBase(target, id, label, *value, p)) {
    *value = !*value;
    return true;
  }
  return false;
}

inline bool
toggleButton(Group& target,
             std::string_view id,
             bool* value,
             const SDL_Point& p = {0})
{
  return toggleButton(target, id, id, value, p);
}

template<class T, class U>
inline bool
choiceButton(Group& target,
             std::string_view id,
             std::string_view label,
             T* value,
             U option,
             const SDL_Point& p = {0})
{
  bool selected = *value == option;
  if (buttonBase(target, id, label, selected, p) && !selected) {
    *value = option;
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
  return choiceButton(target, id, id, value, option, p);
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

inline SDL_Rect
fixInputSize(SDL_Rect r)
{
  if (r.w == 0 || r.h == 0) {
    auto sz = measure('m'); // TODO allow customization for this
    if (r.w == 0) {
      r.w = sz.x * 16 + 4;
    }
    if (r.h == 0) {
      r.h = sz.y + 4;
    }
  }
  return r;
}

inline TextAction
textBoxBase(Group& target,
            std::string_view id,
            std::string_view value,
            SDL_Rect r)
{
  r = fixInputSize(r);
  auto g = group(target, {}, r, Layout::NONE);
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

inline Group
labeledGroup(Group& target,
             std::string_view labelText,
             const SDL_Rect& clientRect)
{
  SDL_Rect r{clientRect};
  SDL_Point labelPos = {r.w + 1, 0};
  r.w += measure(labelText).x + 1;

  auto g = group(target, {}, r, Layout::NONE);
  label(g, labelText, labelPos);
  return g;
}

inline bool
textField(Group& target,
          std::string_view id,
          std::string_view labelText,
          char* value,
          size_t maxSize,
          const SDL_Point& p = {0})
{
  SDL_Rect box{fixInputSize({p.x, p.y, 0, 0})};
  auto g = labeledGroup(target, labelText, box);
  auto changed = textBox(g, id, value, maxSize, box);
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
  SDL_Rect box{fixInputSize({p.x, p.y, 0, 0})};
  auto g = labeledGroup(target, labelText, box);
  auto changed = textBox(g, id, value, box);
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
  constexpr int BUF_SZ = 256;
  char textValue[BUF_SZ];
  static char activeTextValue[BUF_SZ];

  bool active = target.isActive(id);
  char* text = active ? activeTextValue : textValue;
  bool clicked = target.checkMouse(id, r) == MouseAction::ACTION;

  if (!active || clicked) {
    SDL_itoa(*value, text, 10);
  }
  if (textBox(target, id, text, BUF_SZ, r)) {
    auto newValue = SDL_atoi(text);
    if (newValue != *value) {
      *value = newValue;
      return true;
    }
  }
  return false;
}

inline bool
intField(Group& target,
         std::string_view id,
         std::string_view labelText,
         int* value,
         const SDL_Point& p = {0})
{
  SDL_Rect box{fixInputSize({p.x, p.y, 0, 0})};
  auto g = labeledGroup(target, labelText, box);
  auto changed = intBox(g, id, value, box);
  g.end();
  return changed;
}

inline bool
intField(Group& target,
         std::string_view id,
         int* value,
         const SDL_Point& p = {0})
{
  return intField(target, id, id, value, p);
}

// A doubleBox
inline bool
doubleBox(Group& target, std::string_view id, double* value, SDL_Rect r = {0})
{
  SDL_assert(value != nullptr);
  r = fixInputSize(r);

  constexpr int BUF_SZ = 256;
  char textValue[BUF_SZ];
  static char activeTextValue[BUF_SZ];

  bool active = target.isActive(id);
  char* text = active ? activeTextValue : textValue;
  bool clicked = target.checkMouse(id, r) == MouseAction::ACTION;
  if (!active || clicked) {
    int n = SDL_snprintf(text, BUF_SZ, "%f", *value);
    for (int i = n - 1; i > 0; --i) {
      if (text[i] != '0' && text[i] != '.') {
        text[i + 1] = 0;
        break;
      }
    }
  }
  if (textBox(target, id, text, BUF_SZ, r)) {
    auto newValue = SDL_strtod(text, nullptr);
    if (newValue != *value) {
      *value = newValue;
      return true;
    }
  }
  return false;
}

inline bool
doubleField(Group& target,
            std::string_view id,
            std::string_view labelText,
            double* value,
            const SDL_Point& p = {0})
{
  SDL_Rect box{fixInputSize({p.x, p.y, 0, 0})};
  auto g = labeledGroup(target, labelText, box);
  auto changed = doubleBox(g, id, value, box);
  g.end();
  return changed;
}

inline bool
doubleField(Group& target,
            std::string_view id,
            double* value,
            const SDL_Point& p = {0})
{
  return doubleField(target, id, id, value, p);
}
} // namespace dui

#endif // DUI_BASIC_WIDGETS_HPP_
