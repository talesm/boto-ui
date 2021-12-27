#ifndef BOTO_PANELSTYLE_HPP_
#define BOTO_PANELSTYLE_HPP_

#include "EdgeSize.hpp"
#include "GroupStyle.hpp"
#include "Theme.hpp"

namespace boto {

// Style for panels
struct PanelStyle
{
  ElementStyle decoration;
  EdgeSize padding;
  GroupStyle client;

  constexpr PanelStyle withDecoration(const ElementStyle& decoration) const
  {
    return {decoration, padding, client};
  }

  constexpr PanelStyle withPadding(const EdgeSize& padding) const
  {
    return {decoration, padding, client};
  }
  constexpr PanelStyle withBorderSize(const EdgeSize& border) const
  {
    return withDecoration(decoration.withBorderSize(border));
  }
  constexpr PanelStyle withBackgroundColor(SDL_Color background) const
  {
    return withDecoration(decoration.withBackgroundColor(background));
  }
  constexpr PanelStyle withBorderColor(const BorderColorStyle& border) const
  {
    return withDecoration(decoration.withBorderColor(border));
  }

  constexpr PanelStyle withClient(const GroupStyle& client) const
  {
    return {decoration, padding, client};
  }

  constexpr PanelStyle withElementSpacing(int elementSpacing) const
  {
    return withClient(client.withElementSpacing(elementSpacing));
  }

  constexpr PanelStyle withLayout(Layout layout) const
  {
    return withClient(client.withLayout(layout));
  }

  constexpr operator ElementStyle() const { return decoration; }

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
    return {
      boxStyle,
      EdgeSize::all(2),
      themeFor<Group, Theme>(),
    };
  }
};
}

} // namespace boto

#endif // BOTO_PANELSTYLE_HPP_
