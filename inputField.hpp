#ifndef DUI_INPUTFIELD_HPP
#define DUI_INPUTFIELD_HPP

#include <string_view>
#include "Group.hpp"
#include "InputFieldStyle.hpp"
#include "inputBox.hpp"
#include "label.hpp"

namespace dui {

inline Group
labeledGroup(Target target,
             std::string_view labelText,
             const SDL_Rect& clientRect,
             const ElementStyle& style = themeFor<Label>())
{
  SDL_Rect r{clientRect};
  SDL_Point labelPos = {r.w + 1, 0};
  r.w += measure(labelText, style.font, style.scale).x + 1;

  auto g = group(target, {}, r, Layout::NONE);
  label(g, labelText, labelPos, style);
  return g;
}

inline bool
textField(Target target,
          std::string_view id,
          std::string_view labelText,
          char* value,
          size_t maxSize,
          const SDL_Point& p = {0},
          const InputFieldStyle& style = themeFor<TextField>())
{
  SDL_Rect box{makeInputSize({p.x, p.y, 0, 0}, themeFor<TextBox>())};
  auto g = labeledGroup(target, labelText, box, style.label);
  auto changed = textBox(g, id, value, maxSize, box, style.box);
  g.end();
  return changed;
}

inline bool
textField(Target target,
          std::string_view id,
          char* value,
          size_t maxSize,
          const SDL_Point& p = {0},
          const InputFieldStyle& style = themeFor<TextField>())
{
  return textField(target, id, id, value, maxSize, p, style);
}

inline bool
textField(Target target,
          std::string_view id,
          std::string_view labelText,
          std::string* value,
          const SDL_Point& p = {0},
          const InputFieldStyle& style = themeFor<TextField>())
{
  SDL_Rect box{makeInputSize({p.x, p.y, 0, 0}, themeFor<TextBox>())};
  auto g = labeledGroup(target, labelText, box, style.label);
  auto changed = textBox(g, id, value, box, style.box);
  g.end();
  return changed;
}

inline bool
textField(Target target,
          std::string_view id,
          std::string* value,
          const SDL_Point& p = {0},
          const InputFieldStyle& style = themeFor<TextField>())
{
  return textField(target, id, id, value, p, style);
}

inline bool
intField(Target target,
         std::string_view id,
         std::string_view labelText,
         int* value,
         const SDL_Point& p = {0},
         const InputFieldStyle& style = themeFor<IntField>())
{
  SDL_Rect box{makeInputSize({p.x, p.y, 0, 0}, themeFor<IntBox>())};
  auto g = labeledGroup(target, labelText, box, style.label);
  auto changed = intBox(g, id, value, box, style.box);
  g.end();
  return changed;
}

inline bool
intField(Target target,
         std::string_view id,
         int* value,
         const SDL_Point& p = {0},
         const InputFieldStyle& style = themeFor<IntField>())
{
  return intField(target, id, id, value, p, style);
}

inline bool
doubleField(Target target,
            std::string_view id,
            std::string_view labelText,
            double* value,
            const SDL_Point& p = {0},
            const InputFieldStyle& style = themeFor<DoubleField>())
{
  SDL_Rect box{makeInputSize({p.x, p.y, 0, 0}, themeFor<DoubleBox>())};
  auto g = labeledGroup(target, labelText, box, style.label);
  auto changed = doubleBox(g, id, value, box, style.box);
  g.end();
  return changed;
}

inline bool
doubleField(Target target,
            std::string_view id,
            double* value,
            const SDL_Point& p = {0},
            const InputFieldStyle& style = themeFor<DoubleField>())
{
  return doubleField(target, id, id, value, p, style);
}
} // namespace dui

#endif // DUI_INPUTFIELD_HPP
