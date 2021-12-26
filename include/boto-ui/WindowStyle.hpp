#pragma once

#include "ButtonStyle.hpp"
#include "ControlStyle.hpp"
#include "PanelStyle.hpp"
#include "ScrollableStyle.hpp"
#include "Theme.hpp"

namespace boto {

struct WindowStyle
{
  PanelStyle panel;
  ControlStyle title;
  GroupStyle client;

  constexpr operator GroupStyle() const { return client; }

  constexpr operator ElementStyle() const { return panel; }

  constexpr operator ControlStyle() const { return title; }

  constexpr WindowStyle withPanel(const PanelStyle& panel) const
  {
    return {panel, title, client};
  }

  constexpr WindowStyle withTitle(const ControlStyle& title) const
  {
    return {panel, title, client};
  }

  constexpr WindowStyle withClient(const GroupStyle& client) const
  {
    return {panel, title, client};
  }

  constexpr WindowStyle withPadding(const EdgeSize& padding) const
  {
    return withPanel(panel.withPadding(padding));
  }
  constexpr WindowStyle withBorderSize(const EdgeSize& border) const
  {
    return withPanel(panel.withBorderSize(border));
  }
  constexpr WindowStyle withPaint(const ElementPaintStyle& paint) const
  {
    return withPanel(panel.withPaint(paint));
  }
  constexpr WindowStyle withBackgroundColor(SDL_Color background) const
  {
    return withPanel(panel.withBackgroundColor(background));
  }
  constexpr WindowStyle withBorderColor(const BorderColorStyle& border) const
  {
    return withPanel(panel.withBorderColor(border));
  }

  constexpr WindowStyle withElementSpacing(int elementSpacing) const
  {
    return withClient(client.withElementSpacing(elementSpacing));
  }

  constexpr WindowStyle withLayout(Layout layout) const
  {
    return withClient(client.withLayout(layout));
  }
};

struct ScrollableWindowStyle
{
  WindowStyle decoration;
  ScrollableStyle scrollable;
  constexpr operator WindowStyle() const { return decoration; }
  constexpr operator ScrollableStyle() const { return scrollable; }

  constexpr ScrollableWindowStyle withDecoration(
    const WindowStyle& decoration) const
  {
    return {decoration, scrollable};
  }

  constexpr ScrollableWindowStyle withScrollable(
    const ScrollableStyle& scrollable) const
  {
    return {decoration, scrollable};
  }

  constexpr ScrollableWindowStyle withClient(const GroupStyle& client) const
  {
    return withScrollable(scrollable.withClient(client));
  }

  constexpr ScrollableWindowStyle withLayout(Layout layout) const
  {
    return withScrollable(scrollable.withLayout(layout));
  }
};

struct WindowDecoration;
struct Window;
struct ScrollableWindow;

namespace style {

/// Default panel style
template<class Theme>
struct FromTheme<Window, Theme>
{
  constexpr static WindowStyle get()
  {
    auto buttomStyle = themeFor<Button, Theme>();
    auto labelStyle = themeFor<Label, Theme>();
    return {
      themeFor<Panel, Theme>(),
      labelStyle.withBorder(EdgeSize::all(1))
        .withBorderColor(BorderColorStyle::all(labelStyle.paint.text))
        .withBackgroundColor(buttomStyle.normal.background),
      themeFor<Group, Theme>(),
    };
  }
};

template<class Theme>
struct FromTheme<ScrollableWindow, Theme>
{
  constexpr static ScrollableWindowStyle get()
  {
    return {
      themeFor<Window, Theme>().withPadding({0, 0, 255, 255}),
      themeFor<Scrollable, Theme>(),
    };
  }
};

} // namespace style

} // namespace boto
