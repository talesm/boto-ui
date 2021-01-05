#pragma once
#include "PanelStyle.hpp"
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

struct ScrollablePanelStyle
{
  PanelStyle panel;
  ScrollableStyle scrollable;
};

struct Scrollable;
struct ScrollablePanel;

namespace style {

template<class Theme>
struct FromTheme<Scrollable, Theme>
{
  constexpr static ScrollableStyle get()
  {
    return {false, false, themeFor<SliderBox, Theme>()};
  }
};

template<class Theme>
struct FromTheme<ScrollablePanel, Theme>
{
  constexpr static ScrollablePanelStyle get()
  {
    return {
      themeFor<Panel, Theme>().withPadding({0, 0, 255, 255}),
      themeFor<Scrollable, Theme>(),
    };
  }
};
} // namespace style
} // namespace dui
