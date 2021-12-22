#ifndef BOTO_INPUTFIELD_HPP
#define BOTO_INPUTFIELD_HPP

#include <string_view>
#include "Group.hpp"
#include "InputBox.hpp"
#include "InputFieldStyle.hpp"
#include "Label.hpp"

namespace boto {

/// Create a group that labels its items
/// @ingroup groups
inline Group
labeledGroup(Target target,
             std::string_view id,
             std::string_view labelText,
             const SDL_Rect& clientRect,
             const ControlStyle& style = themeFor<Label>())
{
  SDL_Rect r{clientRect};
  SDL_Point labelPos = {r.w + 1, 0};
  r.w += measure(labelText, style.font, style.scale).x + 1;

  auto g = group(target, id, r, Layout::NONE);
  label(g, labelText, labelPos, style);
  return g;
}

/// A text field element
/// @ingroup elements
inline bool
textField(Target target,
          std::string_view id,
          std::string_view labelText,
          char* value,
          size_t maxSize,
          const SDL_Point& p = {0},
          const InputFieldStyle& style = themeFor<TextField>())
{
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, style.box)};
  auto g = labeledGroup(target, id, labelText, box, style.label);
  return textBox(g, id, value, maxSize, box, style.box);
}

/// A text field element
/// @ingroup elements
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

/// A text field element
/// @ingroup elements
inline bool
textField(Target target,
          std::string_view id,
          std::string_view labelText,
          std::string* value,
          const SDL_Point& p = {0},
          const InputFieldStyle& style = themeFor<TextField>())
{
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, style.box)};
  auto g = labeledGroup(target, id, labelText, box, style.label);
  return textBox(g, id, value, box, style.box);
}

/// A text field element
/// @ingroup elements
inline bool
textField(Target target,
          std::string_view id,
          std::string* value,
          const SDL_Point& p = {0},
          const InputFieldStyle& style = themeFor<TextField>())
{
  return textField(target, id, id, value, p, style);
}

/// An integer field element
/// @ingroup elements
inline bool
numberField(Target target,
            std::string_view id,
            std::string_view labelText,
            int* value,
            const SDL_Point& p = {0},
            const InputFieldStyle& style = themeFor<IntField>())
{
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, style.box)};
  auto g = labeledGroup(target, id, labelText, box, style.label);
  return numberBox(g, id, value, box, style.box);
}

/// An integer field element
/// @ingroup elements
inline bool
numberField(Target target,
            std::string_view id,
            int* value,
            const SDL_Point& p = {0},
            const InputFieldStyle& style = themeFor<IntField>())
{
  return numberField(target, id, id, value, p, style);
}

/// A double field element
/// @ingroup elements
inline bool
numberField(Target target,
            std::string_view id,
            std::string_view labelText,
            double* value,
            const SDL_Point& p = {0},
            const InputFieldStyle& style = themeFor<DoubleField>())
{
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, style.box)};
  auto g = labeledGroup(target, id, labelText, box, style.label);
  return numberBox(g, id, value, box, style.box);
}

/// A double field element
/// @ingroup elements
inline bool
numberField(Target target,
            std::string_view id,
            double* value,
            const SDL_Point& p = {0},
            const InputFieldStyle& style = themeFor<DoubleField>())
{
  return numberField(target, id, id, value, p, style);
}

/// A float field element
/// @ingroup elements
inline bool
numberField(Target target,
            std::string_view id,
            std::string_view labelText,
            float* value,
            const SDL_Point& p = {0},
            const InputFieldStyle& style = themeFor<FloatField>())
{
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, style.box)};
  auto g = labeledGroup(target, id, labelText, box, style.label);
  return numberBox(g, id, value, box, style.box);
}

/// A float field element
/// @ingroup elements
inline bool
numberField(Target target,
            std::string_view id,
            float* value,
            const SDL_Point& p = {0},
            const InputFieldStyle& style = themeFor<FloatField>())
{
  return numberField(target, id, id, value, p, style);
}
} // namespace boto

#endif // BOTO_INPUTFIELD_HPP
