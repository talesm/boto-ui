#ifndef DUI_INPUTFIELD_HPP
#define DUI_INPUTFIELD_HPP

#include <string_view>
#include "Group.hpp"
#include "inputBox.hpp"
#include "label.hpp"

namespace dui {

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
  SDL_Rect box{makeInputSize({p.x, p.y, 0, 0}, themeFor<TextBox>())};
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
  SDL_Rect box{makeInputSize({p.x, p.y, 0, 0}, themeFor<TextBox>())};
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

inline bool
intField(Group& target,
         std::string_view id,
         std::string_view labelText,
         int* value,
         const SDL_Point& p = {0})
{
  SDL_Rect box{makeInputSize({p.x, p.y, 0, 0}, themeFor<IntBox>())};
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

inline bool
doubleField(Group& target,
            std::string_view id,
            std::string_view labelText,
            double* value,
            const SDL_Point& p = {0})
{
  SDL_Rect box{makeInputSize({p.x, p.y, 0, 0}, themeFor<DoubleBox>())};
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

#endif // DUI_INPUTFIELD_HPP
