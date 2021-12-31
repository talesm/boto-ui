#ifndef BOTO_PANEL_HPP_
#define BOTO_PANEL_HPP_

#include <string_view>
#include "Decorator.hpp"
#include "Group.hpp"
#include "core/Container.hpp"
#include "elements/presenters/ElementPresenter.hpp"

namespace boto {

struct PanelPresenter
{
  ElementStyle style;
  DisplayList::Clip clip;
  void operator()(Container& c)
  {
    auto& state = c.state();
    presentElement(c.get()->displayList(), state.rect(), style);
  }
};

using PanelImpl = Decorator<Container, PanelPresenter>;

/// Return the adjusted size for a given panel
inline SDL_Point
makePanelSize(SDL_Point defaultSize, Target target)
{
  if (defaultSize.x == 0) {
    defaultSize.x =
      target.getLayout() == Layout::VERTICAL ? target.width() : Undefined;
  }
  if (defaultSize.y == 0) {
    defaultSize.y =
      target.getLayout() == Layout::HORIZONTAL ? target.height() : Undefined;
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
inline PanelImpl
panel(Target target,
      std::string_view id,
      const SDL_Rect& r,
      const PanelStyle& style)
{
  auto g = group(target,
                 id,
                 makePanelRect(r, target),
                 style.padding + style.decoration.border,
                 style);
  return PanelImpl{
    std::move(g),
    PanelPresenter{style},
  };
}
inline PanelImpl
panel(Target target, std::string_view id, const SDL_Rect& r = {0})
{
  return panel(target, id, r, target.styleFor<Panel>());
}
/// @copydoc panel
/// @ingroup groups
inline PanelImpl
panel(Target target, std::string_view id, const SDL_Rect& r, Layout layout)
{
  return panel(target, id, r, target.styleFor<Panel>().withLayout(layout));
}
} // namespace boto

#endif // BOTO_PANEL_HPP_
