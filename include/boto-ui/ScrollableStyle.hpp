#pragma once
#include "PanelStyle.hpp"
#include "SliderBoxStyle.hpp"
#include "Theme.hpp"

namespace boto {

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
  bool fixHorizontal;
  bool fixVertical;
  SliderBoxStyle slider;
  PanelStyle client;
  constexpr operator PanelStyle() const { return client; }

  constexpr ScrollablePanelStyle withFixHorizontal(bool fixHorizontal) const
  {
    return {fixHorizontal, fixVertical, slider, client};
  }

  constexpr ScrollablePanelStyle withFixVertical(bool fixVertical) const
  {
    return {fixHorizontal, fixVertical, slider, client};
  }

  constexpr ScrollablePanelStyle withSlider(SliderBoxStyle slider) const
  {
    return {fixHorizontal, fixVertical, slider, client};
  }

  constexpr ScrollablePanelStyle withClient(const PanelStyle& client) const
  {
    return {fixHorizontal, fixVertical, slider, client};
  }

  constexpr ScrollablePanelStyle withElementSpacing(int elementSpacing) const
  {
    return withClient(client.withElementSpacing(elementSpacing));
  }

  constexpr ScrollablePanelStyle withLayout(Layout layout) const
  {
    return withClient(client.withLayout(layout));
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
      false,                        // Fix horizontal
      false,                        // Fix vertical
      themeFor<SliderBox, Theme>(), // scrollable
      themeFor<Panel, Theme>().withPadding({0, 0, 255, 255}),
    };
  }
};
} // namespace style

constexpr SDL_Point
evalScrollbarSpace(const SliderBoxStyle& style,
                   bool fixedHorizontal,
                   bool fixedVertical)
{
  SDL_Point p{0};
  auto buttonStyle = style.buttons;
  if (!fixedHorizontal) {
    p.x = buttonStyle.padding.left + buttonStyle.padding.right +
          buttonStyle.border.left + buttonStyle.border.right + 8;
  }
  if (!fixedVertical) {
    p.y = buttonStyle.padding.top + buttonStyle.padding.bottom +
          buttonStyle.border.top + buttonStyle.border.bottom + 8;
  }
  return p;
}

} // namespace boto
