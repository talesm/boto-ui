#pragma once

#include "ControlStyle.hpp"
#include "PanelStyle.hpp"
#include "ScrollableStyle.hpp"
#include "core/Theme.hpp"

namespace boto {

struct WindowStyle
{
  PanelStyle panel;
  ControlStyle title;

  constexpr operator GroupStyle() const { return panel.client; }

  constexpr operator ElementStyle() const { return panel; }

  constexpr operator ControlStyle() const { return title; }

  constexpr WindowStyle withPanel(const PanelStyle& panel) const
  {
    return {panel, title};
  }

  constexpr WindowStyle withTitle(const ControlStyle& title) const
  {
    return {panel, title};
  }

  constexpr WindowStyle withPadding(const EdgeSize& padding) const
  {
    return withPanel(panel.withPadding(padding));
  }
  constexpr WindowStyle withBorderSize(const EdgeSize& border) const
  {
    return withPanel(panel.withBorderSize(border));
  }
  constexpr WindowStyle withDecoration(const ElementStyle& decoration) const
  {
    return withPanel(panel.withDecoration(decoration));
  }
  constexpr WindowStyle withBackground(SDL_Color background) const
  {
    return withPanel(panel.withBackground(background));
  }
  constexpr WindowStyle withBorder(SDL_Color border) const
  {
    return withPanel(panel.withBorder(border));
  }

  constexpr WindowStyle withElementSpacing(int elementSpacing) const
  {
    return withPanel(panel.withElementSpacing(elementSpacing));
  }

  constexpr WindowStyle withLayout(Layout layout) const
  {
    return withPanel(panel.withLayout(layout));
  }
};

struct Window;

template<>
struct StyleFor<SteelBlue, Window>
{
  static WindowStyle get(Theme& theme)
  {
    auto labelStyle = theme.of<Label>();
    auto buttonStyle = theme.of<Button>();
    return {
      theme.of<Panel>(),
      labelStyle.withBorder(EdgeSize::all(1))
        .withBorder(labelStyle.text.color)
        .withBackground(buttonStyle.normal.decoration.background),
    };
  }
};

} // namespace boto
