#ifndef DUI_SLIDERBOXSTYLE_HPP_
#define DUI_SLIDERBOXSTYLE_HPP_

#include <SDL.h>
#include "BoxStyle.hpp"
#include "ButtonStyle.hpp"
#include "InputBoxStyle.hpp"
#include "PanelStyle.hpp"
#include "Theme.hpp"

namespace boto {

// Style for button
struct SliderBoxBarStyle
{
  PanelStyle panel;
  BoxStyle cursor;
  int minCursor;
};

struct SliderBoxStyle
{
  ButtonStyle buttons;
  SliderBoxBarStyle bar;
};

struct SliderBoxBar;
struct SliderBox;

namespace style {

template<class Theme>
struct FromTheme<SliderBoxBar, Theme>
{
  constexpr static SliderBoxBarStyle get()
  {
    auto buttonStyle = themeFor<Button, Theme>();
    return {
      themeFor<Panel, Theme>().withPadding(EdgeSize::all(0)),
      themeFor<Box, Theme>()
        .withBackgroundColor(buttonStyle.normal.background)
        .withBorderSize(buttonStyle.border)
        .withBorderColor(buttonStyle.normal.border),
      buttonStyle.border.left + buttonStyle.border.right + 2,
    };
  }
};
template<class Theme>
struct FromTheme<SliderBox, Theme>
{
  constexpr static SliderBoxStyle get()
  {
    return {themeFor<Button, Theme>(), themeFor<SliderBoxBar, Theme>()};
  }
};
} // namespace style

} // namespace boto

#endif // DUI_SLIDERBOXSTYLE_HPP_
