#pragma once

#include "ButtonStyle.hpp"
#include "ElementStyle.hpp"
#include "PanelStyle.hpp"
#include "Theme.hpp"

namespace dui {

struct WindowDecorationStyle
{
  PanelDecorationStyle panel;
  ElementStyle title;

  constexpr operator BoxStyle() const { return panel; }
  constexpr operator ElementStyle() const { return title; }

  constexpr WindowDecorationStyle withPanel(
    const PanelDecorationStyle& panel) const
  {
    return {panel, title};
  }

  constexpr WindowDecorationStyle withTitle(const ElementStyle& title) const
  {
    return {panel, title};
  }

  constexpr WindowDecorationStyle withPadding(const EdgeSize& padding) const
  {
    return withPanel(panel.withPadding(padding));
  }
  constexpr WindowDecorationStyle withBorderSize(const EdgeSize& border) const
  {
    return withPanel(panel.withBorderSize(border));
  }
  constexpr WindowDecorationStyle withPaint(const BoxPaintStyle& paint) const
  {
    return withPanel(panel.withPaint(paint));
  }
  constexpr WindowDecorationStyle withBackgroundColor(
    SDL_Color background) const
  {
    return withPanel(panel.withBackgroundColor(background));
  }
  constexpr WindowDecorationStyle withBorderColor(
    const BorderColorStyle& border) const
  {
    return withPanel(panel.withBorderColor(border));
  }
};

struct WindowStyle
{
  WindowDecorationStyle decoration;
  GroupStyle client;

  constexpr operator GroupStyle() const { return client; }
  constexpr operator WindowDecorationStyle() const { return decoration; }

  constexpr WindowStyle withDecoration(
    const WindowDecorationStyle& decoration) const
  {
    return {decoration, client};
  }

  constexpr WindowStyle withClient(const GroupStyle& client) const
  {
    return {decoration, client};
  }

  constexpr WindowStyle withPadding(const EdgeSize& padding) const
  {
    return withDecoration(decoration.withPadding(padding));
  }
  constexpr WindowStyle withBorder(const EdgeSize& border) const
  {
    return withDecoration(decoration.withBorderSize(border));
  }
  constexpr WindowStyle withPaint(const BoxPaintStyle& paint) const
  {
    return withDecoration(decoration.withPaint(paint));
  }
  constexpr WindowStyle withBackgroundColor(SDL_Color background) const
  {
    return withDecoration(decoration.withBackgroundColor(background));
  }
  constexpr WindowStyle withBorderColor(const BorderColorStyle& border) const
  {
    return withDecoration(decoration.withBorderColor(border));
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

struct WindowDecoration;
struct Window;

namespace style {

/// Default panel style
template<class Theme>
struct FromTheme<WindowDecoration, Theme>
{
  constexpr static WindowDecorationStyle get()
  {
    auto buttomStyle = themeFor<Button, Theme>();
    auto labelStyle = themeFor<Label, Theme>();
    return {
      themeFor<PanelDecoration, Theme>(),
      labelStyle.withBorder(EdgeSize::all(1))
        .withBorderColor(BorderColorStyle::all(labelStyle.paint.text))
        .withBackgroundColor(buttomStyle.normal.background),
    };
  }
};

template<class Theme>
struct FromTheme<Window, Theme>
{
  constexpr static WindowStyle get()
  {
    return {themeFor<WindowDecoration, Theme>(), themeFor<Group, Theme>()};
  }
};

} // namespace style

} // namespace dui
