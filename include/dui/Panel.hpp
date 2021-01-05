#ifndef DUI_PANEL_HPP_
#define DUI_PANEL_HPP_

#include <string_view>
#include "Element.hpp"
#include "Group.hpp"
#include "PanelStyle.hpp"
#include "Wrapper.hpp"

namespace dui {

/// A panel class @see panel()
template<class CLIENT>
class PanelT
{
  PanelStyle style;
  Wrapper<CLIENT> wrapper;

public:
  template<class FUNC>
  PanelT(Target parent,
         std::string_view id,
         const SDL_Rect& r,
         FUNC initializer,
         const PanelStyle& style)
    : style(style)
    , wrapper(parent, id, r, style.padding + style.border, initializer)
  {}
  PanelT(PanelT&&) = default;
  PanelT& operator=(PanelT&&) = default;

  ~PanelT()
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
 * @return PanelT
 */
inline PanelT<Group>
panel(Target target,
      std::string_view id,
      const SDL_Rect& r = {0},
      Layout layout = Layout::VERTICAL,
      const PanelStyle& style = themeFor<Panel>())
{
  return {target,
          id,
          r,
          [&](auto& t, auto r) { return group(t, "client", r, layout); },
          style};
}
} // namespace dui

#endif // DUI_PANEL_HPP_
