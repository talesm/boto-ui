#pragma once
#include "SliderBoxStyle.hpp"
#include "Theme.hpp"

namespace dui {

struct ScrollableStyle
{
  bool fixHorizontal;
  bool fixVertical;
  SliderBoxStyle slider;

  constexpr ScrollableStyle withFixHorizontal(bool fixHorizontal)
  {
    return {fixHorizontal, fixVertical, slider};
  }

  constexpr ScrollableStyle withFixVertical(bool fixVertical)
  {
    return {fixHorizontal, fixVertical, slider};
  }

  constexpr ScrollableStyle withSlider(SliderBoxStyle slider)
  {
    return {fixHorizontal, fixVertical, slider};
  }
};

struct Scrollable;

namespace style {

template<class Theme>
struct FromTheme<Scrollable, Theme>
{
  constexpr static ScrollableStyle get()
  {
    return {false, false, themeFor<SliderBox, Theme>()};
  }
};
} // namespace style
} // namespace dui
