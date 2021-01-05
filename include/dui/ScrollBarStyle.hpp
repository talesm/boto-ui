#ifndef DUI_SCROLLBARSTYLE_HPP_
#define DUI_SCROLLBARSTYLE_HPP_

#include <SDL.h>
#include "BoxStyle.hpp"
#include "ButtonStyle.hpp"
#include "InputBoxStyle.hpp"
#include "PanelStyle.hpp"
#include "Theme.hpp"

namespace dui {

// Style for button
struct ScrollBarSliderStyle
{
  PanelStyle panel;
  BoxStyle cursor;
  int minCursor;
};

struct ScrollBarStyle
{
  ButtonStyle buttons;
  ScrollBarSliderStyle bar;
};

struct ScrollBarSlider;
struct ScrollBar;

namespace style {

template<class Theme>
struct FromTheme<ScrollBarSlider, Theme>
{
  constexpr static ScrollBarSliderStyle get()
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
struct FromTheme<ScrollBar, Theme>
{
  constexpr static ScrollBarStyle get()
  {
    return {themeFor<Button, Theme>(), themeFor<ScrollBarSlider, Theme>()};
  }
};
} // namespace style

} // namespace dui

#endif // DUI_SCROLLBARSTYLE_HPP_
