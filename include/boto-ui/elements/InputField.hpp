#ifndef BOTO_INPUTFIELD_HPP
#define BOTO_INPUTFIELD_HPP

#include <string_view>
#include "Group.hpp"
#include "InputBox.hpp"
#include "Label.hpp"

namespace boto {

/// Create a group that labels its items
/// @ingroup groups
inline auto
labeledGroup(Target target,
             std::string_view id,
             std::string_view labelText,
             SDL_Rect r,
             const ControlStyle& style)
{
  SDL_Point labelPos = {r.w + 1, 0};
  r.w += measure(labelText, style.text.font, style.text.scale).x + 1;

  auto g = group(target, id, r, Layout::NONE);
  label(g, labelText, labelPos, style);
  return g;
}
inline auto
labeledGroup(Target target,
             std::string_view id,
             std::string_view labelText,
             const SDL_Rect& r)
{
  return labeledGroup(target, id, labelText, r, target.styleFor<Label>());
}

/// A text field element
/// @ingroup elements
inline bool
textField(Target target,
          std::string_view id,
          std::string_view labelText,
          char* value,
          size_t maxSize,
          const SDL_Point& p = {0})
{
  auto& style = target.styleFor<TextField>();
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, style.box.normal)};
  auto g = labeledGroup(target, id, labelText, box, style.label);
  return textBox(g, id, value, maxSize, box);
}

/// A text field element
/// @ingroup elements
inline bool
textField(Target target,
          std::string_view id,
          char* value,
          size_t maxSize,
          const SDL_Point& p = {0})
{
  return textField(target, id, id, value, maxSize, p);
}

/// A text field element
/// @ingroup elements
inline bool
textField(Target target,
          std::string_view id,
          std::string_view labelText,
          std::string* value,
          const SDL_Point& p = {0})
{
  auto& style = target.styleFor<TextField>();
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, style.box.normal)};
  auto g = labeledGroup(target, id, labelText, box, style.label);
  return textBox(g, id, value, box);
}

/// A text field element
/// @ingroup elements
inline bool
textField(Target target,
          std::string_view id,
          std::string* value,
          const SDL_Point& p = {0})
{
  return textField(target, id, id, value, p);
}

/// An integer field element
/// @ingroup elements
inline bool
numberField(Target target,
            std::string_view id,
            std::string_view labelText,
            int* value,
            const SDL_Point& p = {0})
{
  auto& style = target.styleFor<IntField>();
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, style.box.normal)};
  auto g = labeledGroup(target, id, labelText, box, style.label);
  return numberBox(g, id, value, box);
}

/// An integer field element
/// @ingroup elements
inline bool
numberField(Target target,
            std::string_view id,
            int* value,
            const SDL_Point& p = {0})
{
  return numberField(target, id, id, value, p);
}

/// A double field element
/// @ingroup elements
inline bool
numberField(Target target,
            std::string_view id,
            std::string_view labelText,
            double* value,
            const SDL_Point& p = {0})
{
  auto& style = target.styleFor<DoubleField>();
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, style.box.normal)};
  auto g = labeledGroup(target, id, labelText, box, style.label);
  return numberBox(g, id, value, box);
}

/// A double field element
/// @ingroup elements
inline bool
numberField(Target target,
            std::string_view id,
            double* value,
            const SDL_Point& p = {0})
{
  return numberField(target, id, id, value, p);
}

/// A float field element
/// @ingroup elements
inline bool
numberField(Target target,
            std::string_view id,
            std::string_view labelText,
            float* value,
            const SDL_Point& p = {0})
{
  auto& style = target.styleFor<FloatField>();
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, style.box.normal)};
  auto g = labeledGroup(target, id, labelText, box, style.label);
  return numberBox(g, id, value, box);
}

/// A float field element
/// @ingroup elements
inline bool
numberField(Target target,
            std::string_view id,
            float* value,
            const SDL_Point& p = {0})
{
  return numberField(target, id, id, value, p);
}
} // namespace boto

#endif // BOTO_INPUTFIELD_HPP
