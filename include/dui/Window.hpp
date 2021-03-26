#pragma once

#include <string_view>
#include "Element.hpp"
#include "Group.hpp"
#include "InputBox.hpp"
#include "Panel.hpp"
#include "Scrollable.hpp"
#include "WindowStyle.hpp"
#include "Wrapper.hpp"

namespace dui {

/// An window class @see window()
template<class CLIENT>
class WindowImpl : public Targetable<WindowImpl<CLIENT>>
{
  WindowDecorationStyle style;
  std::string_view title;
  Group decoration;
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
  /// Window ctor
  template<class FUNC>
  WindowImpl(Target parent,
             std::string_view id,
             std::string_view title,
             const SDL_Rect& r,
             FUNC initializer,
             const WindowDecorationStyle& style)
    : style(style)
    , title(title)
    , decoration(group(parent, id, r, Layout::NONE))
    , wrapper(decoration, makeWrapperPadding(), initializer)
  {}
  /// Move ctor
  WindowImpl(WindowImpl&& rhs)
    : style(rhs.style)
    , title(rhs.title)
    , decoration(std::move(rhs.decoration))
    , wrapper(std::move(rhs.wrapper), decoration)
  {}

  /// Move assignment operator
  WindowImpl& operator=(const WindowImpl& rhs) = delete;
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

  /// Finishes the window
  void end()
  {
    SDL_assert(wrapper);
    auto sz = wrapper.end();
    auto rect = decoration.getRect();
    if (rect.w > 0) {
      sz.x = rect.w;
    }
    if (rect.h > 0) {
      sz.y = rect.h;
    }
    centeredLabel(decoration, title, {0, 0, sz.x, 0}, style);
    box(decoration, {0, 0, sz.x, sz.y}, style);
    decoration.end();
  }

  /// Returns a target object to this
  operator Target() & { return wrapper; }

  /// Returns true if it can accept elements
  operator bool() const { return wrapper; }
};

/// Makes window size accordingly to parameters
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

/// Makes window rect accordingly to parameters
inline SDL_Rect
makeWindowRect(const SDL_Rect& r, Target target)
{
  auto sz = makeWindowSize({r.w, r.h}, target);
  return {r.x, r.y, sz.x, sz.y};
}

/**
 * @brief adds an window element
 * @ingroup groups
 *
 * @param target the parent group or frame
 * @param id the window id
 * @param title the window id
 * @param r the window relative postion and size
 * @param layout the layout
 * @param style the style
 * @return group
 */
inline WindowImpl<Group>
window(Target target,
       std::string_view id,
       std::string_view title,
       const SDL_Rect& r = {0},
       const WindowStyle& style = themeFor<Window>())
{
  return {
    target,
    id,
    title,
    makeWindowRect(r, target),
    [style](auto t, auto r) { return group(t, "client", r, style); },
    style,
  };
}

/// @copydoc window()
/// @ingroup groups
inline WindowImpl<Group>
window(Target target,
       std::string_view id,
       const SDL_Rect& r = {0},
       const WindowStyle& style = themeFor<Window>())
{
  return window(target, id, id, r, style);
}

/// @copydoc window()
/// @ingroup groups
inline WindowImpl<Group>
window(Target target,
       std::string_view id,
       std::string_view title,
       const SDL_Rect& r,
       Layout layout,
       const WindowStyle& style = themeFor<Window>())
{
  return window(target, id, title, r, style.withLayout(layout));
}

/// @copydoc window()
/// @ingroup groups
inline WindowImpl<Group>
window(Target target,
       std::string_view id,
       const SDL_Rect& r,
       Layout layout,
       const WindowStyle& style = themeFor<Window>())
{
  return window(target, id, id, r, layout);
}

/**
 * @brief adds a scrollable window
 * @ingroup groups
 *
 * This is mostly the same than scrollable(), except it accepts more styling
 * options, like border and background color and has a title
 * @param target the parent group or frame
 * @param id the id
 * @param scrollOffset the scrolling control variable
 * @param r the relative position and the size. If size is 0 it will use a
 * default size. Notice that this is different from group() and panel() behavior
 * @param layout
 * @param style
 * @return group
 */
inline WindowImpl<Scrollable>
scrollableWindow(
  Target target,
  std::string_view id,
  std::string_view title,
  SDL_Point* scrollOffset,
  const SDL_Rect& r = {0},
  const ScrollableWindowStyle& style = themeFor<ScrollableWindow>())
{
  return {target,
          id,
          title,
          makeScrollableRect(r, target),
          [=](auto t, auto r) {
            return scrollable(t, "client", scrollOffset, r, style);
          },
          style};
}

/// @copydoc scrollableWindow()
/// @ingroup groups
inline WindowImpl<Scrollable>
scrollableWindow(
  Target target,
  std::string_view id,
  SDL_Point* scrollOffset,
  const SDL_Rect& r = {0},
  const ScrollableWindowStyle& style = themeFor<ScrollableWindow>())
{
  return scrollableWindow(target, id, id, scrollOffset, r, style);
}

/// @copydoc scrollableWindow()
/// @ingroup groups
inline WindowImpl<Scrollable>
scrollableWindow(
  Target target,
  std::string_view id,
  std::string_view title,
  SDL_Point* scrollOffset,
  const SDL_Rect& r,
  Layout layout,
  const ScrollableWindowStyle& style = themeFor<ScrollableWindow>())
{
  return scrollableWindow(
    target, id, title, scrollOffset, r, style.withLayout(layout));
}

/// @copydoc scrollableWindow()
/// @ingroup groups
inline WindowImpl<Scrollable>
scrollableWindow(
  Target target,
  std::string_view id,
  SDL_Point* scrollOffset,
  const SDL_Rect& r,
  Layout layout,
  const ScrollableWindowStyle& style = themeFor<ScrollableWindow>())
{
  return scrollableWindow(target, id, id, scrollOffset, r, layout);
}
} // namespace dui
