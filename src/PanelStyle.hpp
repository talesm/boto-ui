#ifndef DUI_PANELSTYLE_HPP_
#define DUI_PANELSTYLE_HPP_

#include "BoxStyle.hpp"
#include "EdgeSize.hpp"
#include "Theme.hpp"

namespace dui {

// Style for panels
struct PanelStyle
{
  EdgeSize padding;
  EdgeSize border;
  BoxPaintStyle paint;

  constexpr PanelStyle withPadding(const EdgeSize& padding) const
  {
    return {padding, border, paint};
  }
  constexpr PanelStyle withBorderSize(const EdgeSize& border) const
  {
    return {padding, border, paint};
  }
  constexpr PanelStyle withPaint(const BoxPaintStyle& paint) const
  {
    return {padding, border, paint};
  }
  constexpr PanelStyle withBackgroundColor(SDL_Color background) const
  {
    return withPaint(paint.withBackground(background));
  }
  constexpr PanelStyle withBorderColor(const BorderColorStyle& border) const
  {
    return withPaint(paint.withBorder(border));
  }
  constexpr operator BoxStyle() const { return {border, paint}; }
};

struct Panel;

namespace style {
/// Default panel style
template<class Theme>
struct FromTheme<Panel, Theme>
{
  constexpr static PanelStyle get()
  {
    auto boxStyle = themeFor<Box, Theme>();
    return {
      EdgeSize::all(2),
      boxStyle.border,
      boxStyle.paint,
    };
  }
};
}

} // namespace dui

#endif // DUI_PANELSTYLE_HPP_
