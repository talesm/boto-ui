#ifndef DUI_PANELSTYLE_HPP_
#define DUI_PANELSTYLE_HPP_

#include "BoxStyle.hpp"
#include "EdgeSize.hpp"
#include "GroupStyle.hpp"
#include "Theme.hpp"

namespace dui {

// Style for panels
struct PanelDecorationStyle
{
  EdgeSize padding;
  EdgeSize border;
  BoxPaintStyle paint;

  constexpr PanelDecorationStyle withPadding(const EdgeSize& padding) const
  {
    return {padding, border, paint};
  }
  constexpr PanelDecorationStyle withBorderSize(const EdgeSize& border) const
  {
    return {padding, border, paint};
  }
  constexpr PanelDecorationStyle withPaint(const BoxPaintStyle& paint) const
  {
    return {padding, border, paint};
  }
  constexpr PanelDecorationStyle withBackgroundColor(SDL_Color background) const
  {
    return withPaint(paint.withBackground(background));
  }
  constexpr PanelDecorationStyle withBorderColor(
    const BorderColorStyle& border) const
  {
    return withPaint(paint.withBorder(border));
  }

  constexpr operator BoxStyle() const { return {border, paint}; }
};
struct PanelStyle
{
  PanelDecorationStyle decoration;
  GroupStyle client;

  constexpr operator GroupStyle() const { return client; }
  constexpr operator PanelDecorationStyle() const { return decoration; }

  constexpr PanelStyle withDecoration(
    const PanelDecorationStyle& decoration) const
  {
    return {decoration, client};
  }

  constexpr PanelStyle withClient(const GroupStyle& client) const
  {
    return {decoration, client};
  }

  constexpr PanelStyle withPadding(const EdgeSize& padding) const
  {
    return withDecoration(decoration.withPadding(padding));
  }
  constexpr PanelStyle withBorder(const EdgeSize& border) const
  {
    return withDecoration(decoration.withBorderSize(border));
  }
  constexpr PanelStyle withPaint(const BoxPaintStyle& paint) const
  {
    return withDecoration(decoration.withPaint(paint));
  }
  constexpr PanelStyle withBackgroundColor(SDL_Color background) const
  {
    return withDecoration(decoration.withBackgroundColor(background));
  }
  constexpr PanelStyle withBorderColor(const BorderColorStyle& border) const
  {
    return withDecoration(decoration.withBorderColor(border));
  }

  constexpr PanelStyle withElementSpacing(int elementSpacing) const
  {
    return withClient(client.withElementSpacing(elementSpacing));
  }

  constexpr PanelStyle withLayout(Layout layout) const
  {
    return withClient(client.withLayout(layout));
  }
};

struct PanelDecoration;
struct Panel;

namespace style {
/// Default panel style
template<class Theme>
struct FromTheme<PanelDecoration, Theme>
{
  constexpr static PanelDecorationStyle get()
  {
    auto boxStyle = themeFor<Box, Theme>();
    return {EdgeSize::all(2), boxStyle.border, boxStyle.paint};
  }
};
template<class Theme>
struct FromTheme<Panel, Theme>
{
  constexpr static PanelStyle get()
  {
    return {themeFor<PanelDecoration, Theme>(), themeFor<Group, Theme>()};
  }
};
}

} // namespace dui

#endif // DUI_PANELSTYLE_HPP_
