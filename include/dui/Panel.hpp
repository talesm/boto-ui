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
class PanelImpl : public Targetable<PanelImpl<CLIENT>>
{
  PanelDecorationStyle style;
  Wrapper<CLIENT> wrapper;

public:
  template<class FUNC>
  PanelImpl(Target parent,
            std::string_view id,
            const SDL_Rect& r,
            FUNC initializer,
            const PanelDecorationStyle& style)
    : style(style)
    , wrapper(parent, id, r, style.padding + style.border, initializer)
  {}
  PanelImpl(PanelImpl&& rhs)
    : style(rhs.style)
    , wrapper(std::move(rhs.wrapper))
  {}

  PanelImpl& operator=(PanelImpl&& rhs)
  {
    this->~PanelImpl();
    new (this) PanelImpl(std::move(rhs));
    return *this;
  }

  ~PanelImpl()
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

inline SDL_Point
makePanelSize(SDL_Point defaultSize, Target target)
{
  if (defaultSize.x == 0 && target.getLayout() == Layout::VERTICAL) {
    defaultSize.x = target.width();
  }
  if (defaultSize.y == 0 && target.getLayout() == Layout::HORIZONTAL) {
    defaultSize.y = target.height();
  }
  return defaultSize;
}

inline SDL_Rect
makePanelRect(const SDL_Rect& r, Target target)
{
  auto sz = makePanelSize({r.w, r.h}, target);
  return {r.x, r.y, sz.x, sz.y};
}

/**
 * @{
 * @brief adds a panel element
 *
 * @param target the parent group or frame
 * @param id the panel id
 * @param r the panel relative postion and size
 * @param layout the layout
 * @param style the style
 * @return PanelT
 */
inline PanelImpl<Group>
panel(Target target,
      std::string_view id,
      const SDL_Rect& r = {0},
      const PanelStyle& style = themeFor<Panel>())
{
  return {
    target,
    id,
    makePanelRect(r, target),
    [style](auto t, auto r) { return group(t, "client", r, style); },
    style,
  };
}
inline PanelImpl<Group>
panel(Target target,
      std::string_view id,
      const SDL_Rect& r,
      Layout layout,
      const PanelStyle& style = themeFor<Panel>())
{
  return panel(target, id, r, style.withLayout(layout));
}
/// @}
} // namespace dui

#endif // DUI_PANEL_HPP_
