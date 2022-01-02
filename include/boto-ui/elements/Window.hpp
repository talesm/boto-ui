#pragma once

#include <string_view>
#include "Panel.hpp"
#include "Wrapper.hpp"

namespace boto {

/// An window class @see window()
// template<class CLIENT>
// class WindowImpl : public Targetable<WindowImpl<CLIENT>>
// {
//   WindowDecorationStyle style;
//   std::string_view title;
//   Decorator<CLIENT> wrapper;

//   constexpr EdgeSize makeWrapperPadding()
//   {
//     EdgeSize padding = style.panel.border + style.panel.padding;
//     padding.top += (8 << style.title.scale) + style.title.padding.top +
//                    style.title.padding.bottom + style.title.border.top +
//                    style.title.border.bottom;
//     return padding;
//   }

// public:
//   /// Window ctor
//   template<class FUNC>
//   WindowImpl(Target parent,
//              std::string_view id,
//              std::string_view title,
//              const SDL_Rect& r,
//              FUNC initializer,
//              const WindowDecorationStyle& style)
//     : style(style)
//     , title(title)
//     , wrapper(parent, id, r, makeWrapperPadding(), initializer)
//   {}
//   /// Move ctor
//   WindowImpl(WindowImpl&& rhs)
//     : style(rhs.style)
//     , title(rhs.title)
//     , wrapper(std::move(rhs.wrapper))
//   {}

//   /// Move assignment operator
//   WindowImpl& operator=(WindowImpl&& rhs)
//   {
//     this->~WindowImpl();
//     new (this) WindowImpl(std::move(rhs));
//     return *this;
//   }

//   ~WindowImpl()
//   {
//     if (wrapper) {
//       end();
//     }
//   }

//   /// Finishes the window
//   void end()
//   {
//     SDL_assert(wrapper);
//     auto sz = wrapper.endClient();
//     centeredLabel(wrapper, title, {0, 0, sz.x, 0}, style);
//     element(wrapper, {0, 0, sz.x, sz.y}, style);
//     wrapper.end();
//   }

//   /// Returns a target object to this
//   operator Target() & { return wrapper; }

//   /// Returns true if it can accept elements
//   operator bool() const { return wrapper; }
// };

/// Makes window size accordingly to parameters
inline SDL_Point
makeWindowSize(SDL_Point defaultSize, Target target)
{
  if (defaultSize.x == 0 && target.layout() == Layout::VERTICAL) {
    defaultSize.x = target.width();
  }
  if (defaultSize.y == 0 && target.layout() == Layout::HORIZONTAL) {
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

using WindowImpl = Wrapper<PanelImpl, Container>;

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
inline WindowImpl
window(Target target,
       std::string_view id,
       std::string_view title,
       SDL_Rect r = {0})
{
  const WindowStyle& style = target.styleFor<Window>();
  r = makeWindowRect(r, target);
  PanelImpl superElement =
    panel(target, id, r, style.panel.withLayout(Layout::VERTICAL));
  auto clRect = clientRectRelative(r, style.panel.decoration.border);
  centeredLabel(superElement, title, {clRect.x, clRect.y, clRect.w, 8}, style);
  clRect.y += 8;
  clRect.h -= 8;
  Container subElement = group(superElement, id, clRect, style);
  return {std::move(superElement), std::move(subElement)};
}

/// @copydoc window()
/// @ingroup groups
inline WindowImpl
window(Target target, std::string_view id, const SDL_Rect& r = {0})
{
  return window(target, id, id, r);
}

using ScrollableWindowImpl = Wrapper<WindowImpl, ScrollableImpl>;

// /**
//  * @brief adds a scrollable window
//  * @ingroup groups
//  *
//  * This is mostly the same than scrollable(), except it accepts more styling
//  * options, like border and background color and has a title
//  * @param target the parent group or frame
//  * @param id the id
//  * @param scrollOffset the scrolling control variable
//  * @param r the relative position and the size. If size is 0 it will use a
//  * default size. Notice that this is different from group() and panel()
//  behavior
//  * @param layout
//  * @param style
//  * @return group
//  */
// inline ScrollableWindowImpl
// scrollableWindow(
//   Target target,
//   std::string_view id,
//   std::string_view title,
//   SDL_Point* scrollOffset,
//   SDL_Rect r = {0},
//   const ScrollableWindowStyle& style = target.styleOf<ScrollableWindow>())
// {
//   r = makeWindowRect(r, target);
//   WindowImpl superElement = window(target, id, r, style);

//   auto clRect = clientRectRelative(r, style.panel.client.decoration.border);
//   clRect.y += 8;
//   clRect.h -= 8;
//   ScrollableImpl subElement =
//     scrollable(superElement, id, scrollOffset, clRect, style.panel);
//   return {std::move(superElement), std::move(subElement)};
// }

// /// @copydoc scrollableWindow()
// /// @ingroup groups
// inline ScrollableWindowImpl
// scrollableWindow(
//   Target target,
//   std::string_view id,
//   SDL_Point* scrollOffset,
//   const SDL_Rect& r = {0},
//   const ScrollableWindowStyle& style = target.styleOf<ScrollableWindow>())
// {
//   return scrollableWindow(target, id, id, scrollOffset, r, style);
// }

// /// @copydoc scrollableWindow()
// /// @ingroup groups
// inline ScrollableWindowImpl
// scrollableWindow(
//   Target target,
//   std::string_view id,
//   std::string_view title,
//   SDL_Point* scrollOffset,
//   const SDL_Rect& r,
//   Layout layout,
//   const ScrollableWindowStyle& style = target.styleOf<ScrollableWindow>())
// {
//   return scrollableWindow(
//     target, id, title, scrollOffset, r, style.withLayout(layout));
// }

// /// @copydoc scrollableWindow()
// /// @ingroup groups
// inline ScrollableWindowImpl
// scrollableWindow(
//   Target target,
//   std::string_view id,
//   SDL_Point* scrollOffset,
//   const SDL_Rect& r,
//   Layout layout,
//   const ScrollableWindowStyle& style = target.styleOf<ScrollableWindow>())
// {
//   return scrollableWindow(target, id, id, scrollOffset, r, layout);
// }
} // namespace boto
