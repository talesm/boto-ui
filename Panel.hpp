#ifndef DUI_PANEL_HPP_
#define DUI_PANEL_HPP_

#include <string_view>
#include "Group.hpp"
#include "WrapperGroup.hpp"
#include "element.hpp"

namespace dui {

// Style for panels
struct PanelStyle : BorderedBoxStyle
{
  EdgeSize padding;

  PanelStyle() = default;

  constexpr PanelStyle(SDL_Color background,
                       const BorderColorStyle& borderColor,
                       const EdgeSize& border,
                       const EdgeSize& padding)
    : BorderedBoxStyle{background, borderColor, border}
    , padding(padding)
  {}
};

struct Panel;

namespace style {
/// Default panel style
template<>
struct FromTheme<Panel, SteelBlue>
{
  constexpr static PanelStyle get()
  {
    return {
      {219, 228, 240, 240},
      BorderColorStyle::all(themeFor<Text>().color),
      EdgeSize::all(1),
      EdgeSize::all(2),
    };
  }
};
}

/// A panel class @see panel()
class Panel final : public WrapperGroup
{
  PanelStyle style;

public:
  Panel(Group* parent,
        std::string_view id,
        const SDL_Rect& r,
        Layout layout,
        const PanelStyle& style)
    : WrapperGroup(parent, id, r, layout, style.padding + EdgeSize::all(1))
    , style(style)
  {}

protected:
  void afterUnwrap() final
  {
    layout = Layout::NONE;
    borderedBox(*this, {0, 0, width(), height()}, style);
  }
};

/**
 * @brief adds a panel element
 *
 * @param target the parent group or frame
 * @param id the panel id
 * @param r the panel relative postion and size
 * @param layout the layout
 * @param style the style
 * @return Panel
 */
inline Panel
panel(Group& target,
      std::string_view id,
      const SDL_Rect& r = {0},
      Layout layout = Layout::VERTICAL,
      const PanelStyle& style = themeFor<Panel>())
{
  return {&target, id, r, layout, style};
}

} // namespace dui

#endif // DUI_PANEL_HPP_
