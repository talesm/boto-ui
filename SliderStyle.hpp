#ifndef DUI_SLIDERSTYLE_HPP_
#define DUI_SLIDERSTYLE_HPP_

#include <SDL.h>
#include "LabelStyle.hpp"
#include "ScrollBarStyle.hpp"
#include "theme.hpp"

namespace dui {

struct SliderStyle
{
  ScrollBarStyle scroll;
  ElementStyle label;
};
struct Slider;

namespace style {

template<class Slider, class Theme>
struct FromTheme
{
  constexpr static SliderStyle get()
  {
    return {
      themeFor<ScrollBar, Theme>(),
      themeFor<Label, Theme>(),
    };
  }
};
} // namespace style

} // namespace dui

#endif // DUI_SLIDERSTYLE_HPP_
