#ifndef DUI_INPUTBOX_HPP
#define DUI_INPUTBOX_HPP

#include <string_view>
#include "Group.hpp"
#include "Panel.hpp"
#include "element.hpp"

namespace dui {

/// Input box style
struct InputBoxStyle
{
  SDL_Color text;
  PanelStyle box;
  PanelStyle boxActive;
};

namespace style {

/// Default input box style
constexpr InputBoxStyle INPUTBOX{
  TEXT,
  {{{240, 240, 240, 255}, TEXT, TEXT, TEXT, TEXT}, EdgeSize::all(2)},
  {{{255, 255, 255, 255}, TEXT, TEXT, TEXT, TEXT}, EdgeSize::all(2)},
};

}

inline SDL_Rect
makeInputSize(SDL_Rect r)
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
            SDL_Rect r,
            const InputBoxStyle& style = style::INPUTBOX)
{
  r = makeInputSize(r);
  target.checkMouse(id, r);

  auto action = target.checkText(id);
  bool active = false;
  switch (action) {
    case TextAction::NONE:
      active = target.isActive(id);
      break;
    case TextAction::INPUT:
    case TextAction::BACKSPACE:
      active = true;
      break;
  }
  auto& currentStyle = active ? style.boxActive : style.box;
  auto g = panel(target, id, r, Layout::NONE, currentStyle);
  text(g, value, {0}, style.text);

  if (active) {
    // Show cursor
    auto cursorHeight =
      r.h - currentStyle.padding.top - currentStyle.padding.bottom;
    box(g, {int(value.size()) * 8, 0, 1, cursorHeight}, style.text);
  }
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

// A doubleBox
inline bool
doubleBox(Group& target, std::string_view id, double* value, SDL_Rect r = {0})
{
  SDL_assert(value != nullptr);
  r = makeInputSize(r);

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
} // namespace dui

#endif // DUI_INPUTBOX_HPP
