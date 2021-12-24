#ifndef BOTO_PANELSTYLE_HPP_
#define BOTO_PANELSTYLE_HPP_

#include "EdgeSize.hpp"
#include "GroupStyle.hpp"
#include "Theme.hpp"

namespace boto {

// Style for panels
struct PanelStyle
{
  EdgeSize padding;
  EdgeSize border;
  ElementPaintStyle paint;
  GroupStyle client;

  constexpr PanelStyle withPadding(const EdgeSize& padding) const
  {
    return {padding, border, paint, client};
  }
  constexpr PanelStyle withBorderSize(const EdgeSize& border) const
  {
    return {padding, border, paint, client};
  }
  constexpr PanelStyle withPaint(const ElementPaintStyle& paint) const
  {
    return {padding, border, paint, client};
  }
  constexpr PanelStyle withBackgroundColor(SDL_Color background) const
  {
    return withPaint(paint.withBackground(background));
  }
  constexpr PanelStyle withBorderColor(const BorderColorStyle& border) const
  {
    return withPaint(paint.withBorder(border));
  }

  constexpr PanelStyle withClient(const GroupStyle& client) const
  {
    return {padding, border, paint, client};
  }

  constexpr PanelStyle withElementSpacing(int elementSpacing) const
  {
    return withClient(client.withElementSpacing(elementSpacing));
  }

  constexpr PanelStyle withLayout(Layout layout) const
  {
    return withClient(client.withLayout(layout));
  }

  constexpr operator ElementStyle() const { return {border, paint}; }

  constexpr operator GroupStyle() const { return client; }
};

struct Panel;

namespace style {
/// Default panel style
template<class Theme>
struct FromTheme<Panel, Theme>
{
  constexpr static PanelStyle get()
  {
    auto boxStyle = themeFor<Element, Theme>();
    return {EdgeSize::all(2),
            boxStyle.border,
            boxStyle.paint,
            themeFor<Group, Theme>()};
  }
};
}

} // namespace boto

#endif // BOTO_PANELSTYLE_HPP_
