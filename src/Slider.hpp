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
template<class T>
inline bool
slider(Target target,
       std::string_view id,
       std::string_view labelText,
       T* value,
       T min,
       T max,
       const SDL_Point& p = {0},
       const SliderStyle& style = themeFor<Slider>())
{
  auto& buttons = style.scroll.buttons;
  auto adv = makeInputSize(
    p, buttons.font, buttons.scale, buttons.padding + buttons.border);
  SDL_Rect box{p.x, p.y, adv.x, adv.y};
  auto g = labeledGroup(target, labelText, box, style.label);
  auto changed = scrollBar(g, id, value, min, max, box, style.scroll);
  g.end();
  return changed;
}
template<class T>
inline bool
slider(Target target,
       std::string_view id,
       T* value,
       T min,
       T max,
       const SDL_Point& p = {0},
       const SliderStyle& style = themeFor<Slider>())
{
  return slider(target, id, id, value, min, max, p, style);
}
/// @]
} // namespace dui

#endif // DUI_SLIDER_HPP_
