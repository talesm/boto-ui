#pragma once

#include <string_view>
#include "Element.hpp"
#include "Group.hpp"
#include "Panel.hpp"
#include "WindowStyle.hpp"
#include "Wrapper.hpp"

namespace dui {

/// A panel class @see panel()
template<class CLIENT>
class WindowImpl : public Targetable<WindowImpl<CLIENT>>
{
  WindowDecorationStyle style;
  std::string_view title;
  Wrapper<CLIENT> wrapper;

  constexpr EdgeSize makeWrapperPadding()
  {
    EdgeSize padding = style.panel.border + style.panel.padding;
    padding.top += (8 << style.title.scale) + style.title.padding.top +
                   style.title.padding.bottom + style.title.border.top +
                   style.title.border.bottom;
    return padding;
  }

public:
  template<class FUNC>
  WindowImpl(Target parent,
             std::string_view id,
             std::string_view title,
             const SDL_Rect& r,
             FUNC initializer,
             const WindowDecorationStyle& style)
    : style(style)
    , title(title)
    , wrapper(parent, id, r, makeWrapperPadding(), initializer)
  {}
  WindowImpl(WindowImpl&& rhs)
    : style(rhs.style)
    , title(rhs.title)
    , wrapper(std::move(rhs.wrapper))
  {}

  WindowImpl& operator=(WindowImpl&& rhs)
  {
    this->~WindowImpl();
    new (this) WindowImpl(std::move(rhs));
    return *this;
  }

  ~WindowImpl()
  {
    if (wrapper) {
      end();
    }
  }

  void end()
  {
    SDL_assert(wrapper);
    auto sz = wrapper.endClient();
    centeredLabel(wrapper, title, {0, 0, sz.x, 0}, style);
    box(wrapper, {0, 0, sz.x, sz.y}, style);
    wrapper.end();
  }

  operator Target() & { return wrapper; }

  operator bool() const { return wrapper; }
};

inline SDL_Point
makeWindowSize(SDL_Point defaultSize, Target target)
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
makeWindowRect(const SDL_Rect& r, Target target)
{
  auto sz = makeWindowSize({r.w, r.h}, target);
  return {r.x, r.y, sz.x, sz.y};
}

/**
 * @[
 * @brief adds a panel element
 *
 * @param target the parent group or frame
 * @param id the panel id
 * @param r the panel relative postion and size
 * @param layout the layout
 * @param style the style
 * @return WindowImpl
 */
inline WindowImpl<Group>
window(Target target,
       std::string_view id,
       const SDL_Rect& r = {0},
       const WindowStyle& style = themeFor<Window>())
{
  return {
    target,
    id,
    id,
    makeWindowRect(r, target),
    [style](auto t, auto r) { return group(t, "client", r, style); },
    style,
  };
}
inline WindowImpl<Group>
window(Target target,
       std::string_view id,
       const SDL_Rect& r,
       Layout layout,
       const WindowStyle& style = themeFor<Window>())
{
  return window(target, id, r, style.withLayout(layout));
}
/// @]
} // namespace dui
