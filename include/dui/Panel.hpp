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
  Group decoration;
  Wrapper<CLIENT> wrapper;

public:
  /// Ctor
  template<class FUNC>
  PanelImpl(Target parent,
            std::string_view id,
            const SDL_Rect& r,
            FUNC initializer,
            const PanelDecorationStyle& style)
    : style(style)
    , decoration(group(parent, id, r, Layout::NONE))
    , wrapper(decoration, style.padding + style.border, initializer)
  {}
  /// Move ctor
  PanelImpl(PanelImpl&& rhs)
    : style(rhs.style)
    , decoration(std::move(rhs.decoration))
    , wrapper(std::move(rhs.wrapper), decoration)
  {}

  /// Move copy operator
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

  /// Finished the group
  void end()
  {
    SDL_assert(wrapper);
    auto sz = wrapper.end();
    auto rect = decoration.getRect();
    auto w = rect.w;
    auto h = rect.h;
    if (w == 0) {
      w = sz.x;
      decoration.setWidth(w);
    }
    if (h == 0) {
      h = sz.y;
      decoration.setHeight(h);
    }
    box(decoration, {0, 0, w, h}, style);
    decoration.end();
  }

  /// Return a target element for this
  operator Target() & { return wrapper; }

  /// return true if it can accept elements
  operator bool() const { return wrapper; }
};

/// Return the adjusted size for a given panel
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

/// Return the adjusted rect for a given panel
inline SDL_Rect
makePanelRect(const SDL_Rect& r, Target target)
{
  auto sz = makePanelSize({r.w, r.h}, target);
  return {r.x, r.y, sz.x, sz.y};
}

/**
 * @brief adds a panel element
 * @ingroup groups
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
/// @copydoc panel
/// @ingroup groups
inline PanelImpl<Group>
panel(Target target,
      std::string_view id,
      const SDL_Rect& r,
      Layout layout,
      const PanelStyle& style = themeFor<Panel>())
{
  return panel(target, id, r, style.withLayout(layout));
}
} // namespace dui

#endif // DUI_PANEL_HPP_
