#ifndef BOTO_SLIDERBOXSTYLE_HPP_
#define BOTO_SLIDERBOXSTYLE_HPP_

#include <SDL.h>
#include "ButtonStyle.hpp"
#include "InputBoxStyle.hpp"
#include "PanelStyle.hpp"
#include "core/Theme.hpp"

namespace boto {

// Style for button
struct SliderBoxBarStyle
{
  PanelStyle panel;
  ElementStyle cursor;
  int minCursor;
};

struct SliderBoxStyle
{
  ButtonStyle buttons;
  SliderBoxBarStyle bar;
};

struct SliderBoxBar;
struct SliderBox;

template<>
struct StyleFor<SteelBlue, SliderBoxBar>
{
  static SliderBoxBarStyle get(Theme& theme)
  {
    auto buttonStyle = theme.of<Button>().normal.decoration;
    return {
      theme.of<Panel>().withPadding(EdgeSize::all(0)),
      buttonStyle,
      buttonStyle.border.left + buttonStyle.border.right + 2,
    };
  }
};
template<>
struct StyleFor<SteelBlue, SliderBox>
{
  static SliderBoxStyle get(Theme& theme)
  {
    return {theme.of<Button>(), theme.of<SliderBoxBar>()};
  }
};

} // namespace boto

#endif // BOTO_SLIDERBOXSTYLE_HPP_
