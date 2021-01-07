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
  GroupStyle client;

  constexpr ScrollableStyle withFixHorizontal(bool fixHorizontal) const
  {
    return {fixHorizontal, fixVertical, slider, client};
  }

  constexpr ScrollableStyle withFixVertical(bool fixVertical) const
  {
    return {fixHorizontal, fixVertical, slider, client};
  }

  constexpr ScrollableStyle withSlider(SliderBoxStyle slider) const
  {
    return {fixHorizontal, fixVertical, slider, client};
  }

  constexpr ScrollableStyle withClient(const GroupStyle& client) const
  {
    return {fixHorizontal, fixVertical, slider, client};
  }

  constexpr ScrollableStyle withElementSpacing(int elementSpacing) const
  {
    return withClient(client.withElementSpacing(elementSpacing));
  }

  constexpr ScrollableStyle withLayout(Layout layout) const
  {
    return withClient(client.withLayout(layout));
  }

  constexpr operator GroupStyle() const { return client; }
};

struct ScrollablePanelStyle
{
  PanelDecorationStyle decoration;
  ScrollableStyle scrollable;
  constexpr operator PanelDecorationStyle() const { return decoration; }
  constexpr operator ScrollableStyle() const { return scrollable; }

  constexpr ScrollablePanelStyle withDecoration(
    const PanelDecorationStyle& decoration) const
  {
    return {decoration, scrollable};
  }

  constexpr ScrollablePanelStyle withScrollable(
    const ScrollableStyle& scrollable) const
  {
    return {decoration, scrollable};
  }

  constexpr ScrollablePanelStyle withClient(const GroupStyle& client) const
  {
    return withScrollable(scrollable.withClient(client));
  }

  constexpr ScrollablePanelStyle withLayout(Layout layout) const
  {
    return withScrollable(scrollable.withLayout(layout));
  }
};

struct Scrollable;
struct ScrollablePanel;

namespace style {

template<class Theme>
struct FromTheme<Scrollable, Theme>
{
  constexpr static ScrollableStyle get()
  {
    return {
      false,                        // Fix horizontal
      false,                        // Fix vertical
      themeFor<SliderBox, Theme>(), // scrollable
      themeFor<Group, Theme>(),     // group
    };
  }
};

template<class Theme>
struct FromTheme<ScrollablePanel, Theme>
{
  constexpr static ScrollablePanelStyle get()
  {
    return {
      themeFor<PanelDecoration, Theme>().withPadding({0, 0, 255, 255}),
      themeFor<Scrollable, Theme>(),
    };
  }
};
} // namespace style
} // namespace dui
