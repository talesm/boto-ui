#ifndef DUI_SLIDERFIELD_HPP_
#define DUI_SLIDERFIELD_HPP_

#include <algorithm>
#include <SDL.h>
#include "Group.hpp"
#include "InputBox.hpp"
#include "InputField.hpp"
#include "SliderBox.hpp"
#include "SliderFieldStyle.hpp"
#include "Target.hpp"

namespace dui {

/**
 * @[
 * @brief A sliderField that allows the user to choose a value in a interval
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
sliderField(Target target,
            std::string_view id,
            std::string_view labelText,
            T* value,
            T min,
            T max,
            const SDL_Point& p = {0},
            const SliderFieldStyle& style = themeFor<SliderField>())
{
  auto& buttons = style.scroll.buttons;
  auto adv = makeInputSize(
    p, buttons.font, buttons.scale, buttons.padding + buttons.border);
  SDL_Rect box{p.x, p.y, adv.x, adv.y};
  auto g = labeledGroup(target, labelText, box, style.label);
  return sliderBox(g, id, value, min, max, box, style.scroll);
}
template<class T>
inline bool
sliderField(Target target,
            std::string_view id,
            T* value,
            T min,
            T max,
            const SDL_Point& p = {0},
            const SliderFieldStyle& style = themeFor<SliderField>())
{
  return sliderField(target, id, id, value, min, max, p, style);
}
/// @]
} // namespace dui

#endif // DUI_SLIDERFIELD_HPP_
