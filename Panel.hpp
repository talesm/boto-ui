#ifndef DUI_PANEL_HPP_
#define DUI_PANEL_HPP_

#include <string_view>
#include "Group.hpp"
#include "PanelStyle.hpp"
#include "WrapperGroup.hpp"
#include "element.hpp"

namespace dui {

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
    box(*this, {0, 0, width(), height()}, style);
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
