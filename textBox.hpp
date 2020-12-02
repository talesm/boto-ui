#ifndef DUI_TEXTBOX_HPP
#define DUI_TEXTBOX_HPP

#include <string_view>
#include "Group.hpp"
#include "element.hpp"

namespace dui {

namespace style {

constexpr SDL_Color INPUTBOX{240, 240, 240, 255};
constexpr SDL_Color INPUTBOX_ACTIVE{255, 255, 255, 255};
constexpr SDL_Color INPUTBOX_BORDER{0, 0, 0, 255};

}

struct InputBoxStyle
{
  SDL_Color center;
  SDL_Color border;
};

inline void
inputBox(Group& target, const SDL_Rect& r, const InputBoxStyle& style)
{
  borderedBox(
    target,
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
  inputBox(g, r, {bgColor, style::INPUTBOX_BORDER});
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

#endif // DUI_TEXTBOX_HPP
