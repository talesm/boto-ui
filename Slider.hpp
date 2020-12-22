#ifndef DUI_SLIDER_HPP_
#define DUI_SLIDER_HPP_

#include <algorithm>
#include <SDL.h>
#include "Group.hpp"
#include "InputBox.hpp"
#include "InputField.hpp"
#include "ScrollBar.hpp"
#include "SliderStyle.hpp"
#include "Target.hpp"

namespace dui {

/**
 * @[
 * @brief A slider that allows the user to choose a value in a interval
 *
 * @param target
 * @param id
 * @param labelText
 * @param value
 * @param min
 * @param max
 * @param p
 * @param style
 * @return true
 * @return false
 */
inline bool
slider(Target target,
       std::string_view id,
       std::string_view labelText,
       int* value,
       int min,
       int max,
       const SDL_Point& p = {0},
       const SliderStyle& style = themeFor<Slider>())
{
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, themeFor<TextBox>())};
  auto g = labeledGroup(target, labelText, box, style.label);
  auto changed = scrollBar(g, id, value, min, max, box, style.scroll);
  g.end();
  return changed;
}

inline bool
slider(Target target,
       std::string_view id,
       int* value,
       int min,
       int max,
       const SDL_Point& p = {0},
       const SliderStyle& style = themeFor<Slider>())
{
  return slider(target, id, id, value, min, max, p, style);
}
/// @]
} // namespace dui

#endif // DUI_SLIDER_HPP_
