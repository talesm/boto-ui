#pragma once

#include <string_view>
#include "Panel.hpp"
#include "ScrollableStyle.hpp"
#include "SliderBox.hpp"
#include "Wrapper.hpp"

namespace boto {
SDL_Rect
decorateBars(Target target,
             SDL_Rect r,
             SDL_Point* scrollOffset,
             const SliderBoxStyle& style,
             bool fixedHorizontal,
             bool fixedVertical)
{
  SDL_Point scrollbarsPadding =
    evalScrollbarSpace(style, fixedHorizontal, fixedVertical);
  if (scrollbarsPadding.x > 0) {
    r.w -= scrollbarsPadding.x;
    sliderBoxV(target,
               "vertical",
               &scrollOffset->y,
               0,
               r.h,
               {
                 r.w,
                 0,
                 scrollbarsPadding.x,
                 r.h,
               });
  }
  if (scrollbarsPadding.y > 0) {
    r.h -= scrollbarsPadding.y;
    sliderBox(target,
              "horizontal",
              &scrollOffset->x,
              0,
              r.w,
              {
                0,
                r.h,
                r.w,
                scrollbarsPadding.y,
              });
  }
  return r;
}

/// Eval the scrollable size according with parameters
inline SDL_Point
makeScrollableSize(const SDL_Point& defaultSize, Target target)
{
  auto size = makePanelSize(defaultSize, target);
  if (size.x == 0) {
    size.x = 150;
  }
  if (size.y == 0) {
    size.y = 80;
  }
  return size;
}

/// Eval the scrollable rect according with parameters
inline SDL_Rect
makeScrollableRect(const SDL_Rect& r, Target target)
{
  auto sz = makeScrollableSize({r.w, r.h}, target);
  return {r.x, r.y, sz.x, sz.y};
}

using ScrollableImpl = Wrapper<Container, Container>;

/**
 * @brief add a scrollable group
 * @ingroup groups
 *
 * @param target the parent group or frame
 * @param id the id
 * @param scrollOffset the scrolling control variable
 * @param r the relative position and the size. If size is 0 it will use a
 * default size. Notice that this is different from group() and panel()
 * behavior
 * @param layout
 * @param style
 * @return group
 */
inline ScrollableImpl
scrollable(Target target,
           std::string_view id,
           SDL_Point* scrollOffset,
           SDL_Rect r = {0},
           const ScrollableStyle& style = themeFor<Scrollable>())
{
  r = makeScrollableRect(r, target);
  auto superElement = target.container(id, r, {}, {}, Layout::NONE, 0);
  r = decorateBars(superElement,
                   r,
                   scrollOffset,
                   style.slider,
                   style.fixHorizontal,
                   style.fixVertical);
  auto subElement =
    group(superElement, "client", r, *scrollOffset, {}, style.client);
  return {std::move(superElement), std::move(subElement)};
}
/// @copydoc scrollable()
/// @ingroup groups
inline ScrollableImpl
scrollable(Target target,
           std::string_view id,
           SDL_Point* scrollOffset,
           const SDL_Rect& r,
           Layout layout,
           const ScrollableStyle& style = themeFor<Scrollable>())
{
  return scrollable(target, id, scrollOffset, r, style.withLayout(layout));
}

using ScrollablePanelImpl = Wrapper<PanelImpl, Container>;
/**
 * @{
 * @brief add a scrollable panel
 *
 * This is mostly the same than scrollable(), except it accepts more styling
 * options, like border and background color.
 * @param target the parent group or frame
 * @param id the id
 * @param scrollOffset the scrolling control variable
 * @param r the relative position and the size. If size is 0 it will use a
 * default size. Notice that this is different from group() and panel() behavior
 * @param layout
 * @param style
 * @return group
 */
inline ScrollablePanelImpl
scrollablePanel(Target target,
                std::string_view id,
                SDL_Point* scrollOffset,
                SDL_Rect r = {0},
                const ScrollablePanelStyle& style = themeFor<ScrollablePanel>())
{
  r = makeScrollableRect(r, target);
  auto& client = style.client;
  auto superElement = panel(target,
                            id,
                            r,
                            client.withElementSpacing(0)
                              .withLayout(Layout::NONE)
                              .withPadding(EdgeSize::all(0)));
  r = decorateBars(superElement,
                   clientRect(client.decoration.border, r),
                   scrollOffset,
                   style.slider,
                   style.fixHorizontal,
                   style.fixVertical);
  auto subElement = group(superElement,
                          "client",
                          r,
                          {
                            client.padding.left + scrollOffset->x,
                            client.padding.top + scrollOffset->y,
                          },
                          {
                            client.padding.right,
                            client.padding.bottom,
                          },
                          client);
  return {std::move(superElement), std::move(subElement)};
}
/// @copydoc scrollablePanel()
/// @ingroup groups
inline ScrollablePanelImpl
scrollablePanel(Target target,
                std::string_view id,
                SDL_Point* scrollOffset,
                const SDL_Rect& r,
                Layout layout,
                const ScrollablePanelStyle& style = themeFor<ScrollablePanel>())
{
  return scrollablePanel(target, id, scrollOffset, r, style.withLayout(layout));
}
///@}
} // namespace boto
