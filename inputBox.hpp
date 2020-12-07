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
  EdgeSize padding;
  EdgeSize border;
  ElementColorStyle normal;
  ElementColorStyle active;
};

struct InputBoxBase;

namespace style {

template<>
struct FromTheme<InputBoxBase, SteelBlue>
{
  constexpr static InputBoxStyle get()
  {
    return {
      EdgeSize::all(2),
      EdgeSize::all(1),
      {TEXT, {240, 240, 240, 255}, BorderColorStyle::all(TEXT)},
      {TEXT, {255, 255, 255, 255}, BorderColorStyle::all(TEXT)},
    };
  }
};
}

inline SDL_Rect
makeInputSize(SDL_Rect r,
              const EdgeSize& padding = themeFor<InputBoxBase>().padding,
              const EdgeSize& border = themeFor<InputBoxBase>().border)
{
  if (r.w == 0 || r.h == 0) {
    auto clientSz = measure('m'); // TODO allow customization for this
    clientSz.x *= 16;

    auto elementSz = elementSize(padding + border, clientSz);

    if (r.w == 0) {
      r.w = elementSz.x;
    }
    if (r.h == 0) {
      r.h = elementSz.y;
    }
  }
  return r;
}

inline TextAction
textBoxBase(Group& target,
            std::string_view id,
            std::string_view value,
            SDL_Rect r,
            const InputBoxStyle& style = themeFor<InputBoxBase>())
{
  r = makeInputSize(r, style.padding);
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
  auto& currentColors = active ? style.active : style.normal;
  auto g = panel(target,
                 id,
                 r,
                 Layout::NONE,
                 {
                   currentColors.background,
                   currentColors.border,
                   style.border,
                   style.padding,
                 });

  // This creates an auto scroll effect if value text don't fit in the box;
  auto clientSz = clientSize(style.padding + EdgeSize::all(1), {r.w, r.h});
  auto contentSz = measure(value);
  int deltaX = contentSz.x - clientSz.x;
  if (deltaX > 0) {
    int deltaChar = deltaX / 8 + 1;
    value.remove_prefix(deltaChar);
  }

  text(g, value, {0}, currentColors.text);

  if (active && (g.getState().ticks() / 512) % 2) {
    // Show cursor
    box(g, {int(value.size()) * 8, 0, 1, clientSz.y}, currentColors.text);
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

// TODO delete copy ctor, other safety nets
class BufferedInputBox
{
  Group& target;
  std::string_view id;
  SDL_Rect rect;

  bool active;
  bool refillBuffer;

public:
  static constexpr int BUF_SZ = 256;
  char buffer[BUF_SZ];

  BufferedInputBox(Group& target, std::string_view id, SDL_Rect r)
    : target(target)
    , id(id)
    , rect(makeInputSize(r))
  {
    bool clicked = target.checkMouse(id, rect) == MouseAction::GRAB;
    active = target.isActive(id);
    refillBuffer = !active || clicked;
  }

  bool wantsRefill() const { return refillBuffer; }

  bool end()
  {
    if (!active) {
      textBox(target, id, buffer, BUF_SZ, rect);
      return false;
    }
    static char editBuffer[BUF_SZ];
    if (refillBuffer) {
      SDL_strlcpy(editBuffer, buffer, BUF_SZ);
    }
    if (!textBox(target, id, editBuffer, BUF_SZ, rect)) {
      return false;
    }
    SDL_strlcpy(buffer, editBuffer, BUF_SZ);
    return true;
  }
};

// A intBox
inline bool
intBox(Group& target, std::string_view id, int* value, SDL_Rect r = {0})
{
  SDL_assert(value != nullptr);
  BufferedInputBox bufferedBox{target, id, r};
  if (bufferedBox.wantsRefill()) {
    SDL_itoa(*value, bufferedBox.buffer, 10);
  }
  if (bufferedBox.end()) {
    auto newValue = SDL_atoi(bufferedBox.buffer);
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
  BufferedInputBox bufferedBox{target, id, r};
  if (bufferedBox.wantsRefill()) {
    auto text = bufferedBox.buffer;
    int n = SDL_snprintf(text, bufferedBox.BUF_SZ, "%f", *value);
    for (int i = n - 1; i > 0; --i) {
      if (text[i] != '0' && text[i] != '.') {
        text[i + 1] = 0;
        break;
      }
    }
  }
  if (bufferedBox.end()) {
    auto newValue = SDL_strtod(bufferedBox.buffer, nullptr);
    if (newValue != *value) {
      *value = newValue;
      return true;
    }
  }
  return false;
}
} // namespace dui

#endif // DUI_INPUTBOX_HPP
