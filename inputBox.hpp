#ifndef DUI_INPUTBOX_HPP
#define DUI_INPUTBOX_HPP

#include <string_view>
#include "Group.hpp"
#include "InputBoxStyle.hpp"
#include "Panel.hpp"
#include "element.hpp"

namespace dui {

inline SDL_Rect
makeInputSize(SDL_Rect r, const InputBoxStyle& style)
{
  if (r.w == 0 || r.h == 0) {
    auto clientSz = measure(
      'm', style.font, style.scale); // TODO allow customization for this
    clientSz.x *= 16;

    auto elementSz = elementSize(style.padding + style.border, clientSz);

    if (r.w == 0) {
      r.w = elementSz.x;
    }
    if (r.h == 0) {
      r.h = elementSz.y;
    }
  }
  return r;
}

struct TextChange
{
  std::string_view insert;
  size_t index;
  size_t erase;
};

inline TextChange
textBoxBase(Group& target,
            std::string_view id,
            std::string_view value,
            SDL_Rect r,
            const InputBoxStyle& style = themeFor<InputBoxBase>())
{
  static size_t cursorPos = 0;
  static size_t maxPos = 0;
  r = makeInputSize(r, style);
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
  if (deltaX > 0) {
    int deltaChar = deltaX / 8 + 1;
    value.remove_prefix(deltaChar);
  }

  text(g, value, {0}, {style.font, currentColors.text, style.scale});

  if (active && (g.getState().ticks() / 512) % 2) {
    // Show cursor
    colorBox(g, {int(cursorPos) * 8, 0, 1, clientSz.y}, currentColors.text);
  }
  g.end();
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
textBox(Group& target,
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
textBox(Group& target,
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
  Group& target;
  std::string_view id;
  SDL_Rect rect;
  const InputBoxStyle& style;

  bool active;
  bool refillBuffer;

public:
  static constexpr int BUF_SZ = 256;
  char buffer[BUF_SZ];

  BufferedInputBox(Group& target,
                   std::string_view id,
                   SDL_Rect r,
                   const InputBoxStyle& style)
    : target(target)
    , id(id)
    , rect(makeInputSize(r, style))
    , style(style)
  {
    bool clicked = target.checkMouse(id, rect) == MouseAction::GRAB;
    active = target.isActive(id);
    refillBuffer = !active || clicked;
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
      return false;
    }
    SDL_strlcpy(buffer, editBuffer, BUF_SZ);
    return true;
  }
};

// A intBox
inline bool
intBox(Group& target,
       std::string_view id,
       int* value,
       SDL_Rect r = {0},
       const InputBoxStyle& style = themeFor<IntBox>())
{
  SDL_assert(value != nullptr);
  BufferedInputBox bufferedBox{target, id, r, style};
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
doubleBox(Group& target,
          std::string_view id,
          double* value,
          SDL_Rect r = {0},
          const InputBoxStyle& style = themeFor<DoubleBox>())
{
  SDL_assert(value != nullptr);
  BufferedInputBox bufferedBox{target, id, r, style};
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
