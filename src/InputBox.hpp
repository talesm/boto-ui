#ifndef DUI_INPUTBOX_HPP
#define DUI_INPUTBOX_HPP

#include <string_view>
#include "Element.hpp"
#include "Group.hpp"
#include "InputBoxStyle.hpp"
#include "Panel.hpp"

namespace dui {

inline SDL_Point
makeInputSize(SDL_Point defaultSz,
              const Font& font,
              int scale,
              const EdgeSize& padding)
{
  if (defaultSz.x != 0 && defaultSz.y != 0) {
    return defaultSz;
  }
  // TODO allow customization for this
  auto clientSz = measure('m', font, scale);
  clientSz.x *= 16;

  auto elementSz = elementSize(padding, clientSz);

  if (defaultSz.x == 0) {
    defaultSz.x = elementSz.x;
  }
  if (defaultSz.y == 0) {
    defaultSz.y = elementSz.y;
  }
  return defaultSz;
}

inline SDL_Rect
makeInputRect(SDL_Rect r, const InputBoxStyle& style)
{
  auto sz = makeInputSize(
    {r.w, r.h}, style.font, style.scale, style.padding + style.border);
  return {r.x, r.y, sz.x, sz.y};
}

struct TextChange
{
  std::string_view insert;
  size_t index;
  size_t erase;
};

inline TextChange
textBoxBase(Target target,
            std::string_view id,
            std::string_view value,
            SDL_Rect r,
            const InputBoxStyle& style = themeFor<InputBoxBase>())
{
  static size_t cursorPos = 0;
  static size_t maxPos = 0;
  r = makeInputRect(r, style);
  if (target.checkMouse(id, r) == MouseAction::GRAB) {
    maxPos = cursorPos = value.size();
  }

  auto action = target.checkText(id);
  bool active = action == TextAction::NONE ? target.isActive(id) : true;
  if (active && cursorPos > value.size()) {
    maxPos = cursorPos = value.size();
  }
  auto& currentColors = active ? style.active : style.normal;
  auto g = panel(
    target, id, r, Layout::NONE, {style.padding, style.border, currentColors});

  // This creates an auto scroll effect if value text don't fit in the box;
  auto clientSz = clientSize(style.padding + EdgeSize::all(1), {r.w, r.h});
  auto contentSz = measure(value, style.font, style.scale);
  int deltaX = contentSz.x - clientSz.x;
  if (deltaX < 0) {
    deltaX = 0;
  } else if (active && deltaX + 8 > int(cursorPos) * 8) {
    // TODO Use proper scrolling here
    deltaX = cursorPos * 8;
    if (deltaX > 8) {
      deltaX -= 8;
    } else {
      deltaX = 0;
    }
  }
  text(g, value, {-deltaX, 0}, {style.font, currentColors.text, style.scale});

  if (active && (target.getState().ticks() / 512) % 2) {
    // Show cursor
    colorBox(
      g, {int(cursorPos) * 8 - deltaX, 0, 1, clientSz.y}, currentColors.text);
  }
  if (action == TextAction::INPUT) {
    auto insert = target.lastText();
    auto index = cursorPos;
    cursorPos += insert.size();
    maxPos += insert.size();
    return {insert, index, 0};
  }
  if (action == TextAction::KEYDOWN) {
    SDL_Keysym keysym = target.lastKeyDown();
    switch (keysym.sym) {
      case SDLK_BACKSPACE:
        if (!value.empty()) {
          cursorPos -= 1;
          maxPos -= 1;
          return {{}, cursorPos, 1};
        }
        break;
      case SDLK_LEFT:
        if (cursorPos > 0) {
          cursorPos -= 1;
        }
        break;
      case SDLK_RIGHT:
        if (cursorPos < maxPos) {
          cursorPos += 1;
        }
        break;
      default:
        break;
    }
  }
  return {};
}

inline bool
textBox(Target target,
        std::string_view id,
        char* value,
        size_t maxSize,
        const SDL_Rect& r = {0},
        const InputBoxStyle& style = themeFor<TextBox>())
{
  auto len = strlen(value);
  auto change = textBoxBase(target, id, {value, len}, r, style);
  if (change.erase == 0 && change.insert.empty()) {
    return false;
  }
  int offset = int(change.insert.size()) - int(change.erase);
  if (offset != 0) {
    size_t target = change.index + change.insert.size();
    int maxCount = maxSize - target;
    if (maxCount > 0) {
      size_t source = target - offset;
      SDL_memmove(&value[target],
                  &value[source],
                  std::min(len - source + 1, size_t(maxCount)));
    }
  }
  if (!change.insert.empty()) {
    int maxCount = maxSize - 1 - change.index;
    if (maxCount > 0) {
      SDL_memcpy(&value[change.index],
                 change.insert.data(),
                 std::min(change.insert.size(), size_t(maxCount)));
    }
  }
  return true;
}

inline bool
textBox(Target target,
        std::string_view id,
        std::string* value,
        const SDL_Rect& r = {0},
        const InputBoxStyle& style = themeFor<TextBox>())
{
  auto change = textBoxBase(target, id, *value, r, style);
  if (change.erase == 0 && change.insert.empty()) {
    return false;
  }
  value->replace(change.index, change.erase, change.insert);
  return true;
}

// TODO delete copy ctor, other safety nets
class BufferedInputBox
{
  Target target;
  std::string_view id;
  SDL_Rect rect;
  const InputBoxStyle& style;

  bool active;
  bool refillBuffer;

public:
  int incAmount = 0;
  static constexpr int BUF_SZ = 256;
  char buffer[BUF_SZ];

  BufferedInputBox(Target target,
                   std::string_view id,
                   SDL_Rect r,
                   const InputBoxStyle& style)
    : target(target)
    , id(id)
    , rect(makeInputRect(r, style))
    , style(style)
  {
    bool clicked = target.checkMouse(id, rect) == MouseAction::GRAB;
    active = target.isActive(id);
    refillBuffer = !active || clicked;
    if (active && target.checkText(id) == TextAction::KEYDOWN) {
      auto keysym = target.lastKeyDown();
      if (keysym.sym == SDLK_UP) {
        incAmount = 1;
        refillBuffer = true;
      } else if (keysym.sym == SDLK_DOWN) {
        incAmount = -1;
        refillBuffer = true;
      }
    }
  }

  bool wantsRefill() const { return refillBuffer; }

  bool end()
  {
    if (!active) {
      textBox(target, id, buffer, BUF_SZ, rect, style);
      return false;
    }
    static char editBuffer[BUF_SZ];
    if (refillBuffer) {
      SDL_strlcpy(editBuffer, buffer, BUF_SZ);
    }
    if (!textBox(target, id, editBuffer, BUF_SZ, rect, style)) {
      return incAmount != 0;
    }
    SDL_strlcpy(buffer, editBuffer, BUF_SZ);
    return true;
  }
};

// An int Box
inline bool
numberBox(Target target,
          std::string_view id,
          int* value,
          SDL_Rect r = {0},
          const InputBoxStyle& style = themeFor<IntBox>())
{
  SDL_assert(value != nullptr);
  BufferedInputBox bufferedBox{target, id, r, style};
  if (bufferedBox.wantsRefill()) {
    if (bufferedBox.incAmount != 0) {
      *value += bufferedBox.incAmount;
    }
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

// A double box
inline bool
numberBox(Target target,
          std::string_view id,
          double* value,
          SDL_Rect r = {0},
          const InputBoxStyle& style = themeFor<DoubleBox>())
{
  SDL_assert(value != nullptr);
  BufferedInputBox bufferedBox{target, id, r, style};
  if (bufferedBox.wantsRefill()) {
    if (bufferedBox.incAmount != 0) {
      *value += bufferedBox.incAmount;
    }
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
// A float box
inline bool
numberBox(Target target,
          std::string_view id,
          float* value,
          SDL_Rect r = {0},
          const InputBoxStyle& style = themeFor<FloatBox>())
{
  SDL_assert(value != nullptr);
  BufferedInputBox bufferedBox{target, id, r, style};
  if (bufferedBox.wantsRefill()) {
    if (bufferedBox.incAmount != 0) {
      *value += bufferedBox.incAmount;
    }
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
