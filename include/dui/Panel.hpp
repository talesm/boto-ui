#ifndef DUI_PANEL_HPP_
#define DUI_PANEL_HPP_

#include <string_view>
#include "Element.hpp"
#include "Group.hpp"
#include "PanelStyle.hpp"
#include "Wrapper.hpp"

namespace dui {

/// A panel class @see panel()
class Panel
{
  PanelStyle style;
  Wrapper wrapper;

public:
  Panel(Target parent,
        std::string_view id,
        const SDL_Rect& r,
        Layout layout,
        const PanelStyle& style)
    : style(style)
    , wrapper(parent, id, {0}, r, layout, style.padding + style.border)
  {}
  Panel(Panel&&) = default;
  Panel& operator=(Panel&&) = default;

  ~Panel()
  {
    if (wrapper) {
      end();
    }
  }

  void end()
  {
    SDL_assert(wrapper);
    auto sz = wrapper.endClient();
    box(*this, {0, 0, sz.x, sz.y}, style);
    wrapper.end();
  }

  operator Target() & { return wrapper; }

  operator bool() const { return wrapper; }
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
panel(Target target,
      std::string_view id,
      const SDL_Rect& r = {0},
      Layout layout = Layout::VERTICAL,
      const PanelStyle& style = themeFor<Panel>())
{
  return {target, id, r, layout, style};
}

} // namespace dui

#endif // DUI_PANEL_HPP_
