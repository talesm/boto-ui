#pragma once

#include <string_view>
#include "Panel.hpp"
#include "ScrollableStyle.hpp"
#include "SliderBox.hpp"
#include "Wrapper.hpp"

namespace boto {
struct ScrollablePresenter
{
  SliderBoxStyle sliderStyle;
  SDL_Point scrollBarsPadding;
  SDL_Point* scrollOffset;

  /// Finished the group
  void operator()(Target target)
  {
    SDL_Point sz{target.size()}; //{wrapper.width(), wrapper.height()};
    if (scrollBarsPadding.x > 0) {
      sliderBoxV(target,
                 "vertical",
                 &scrollOffset->y,
                 0,
                 sz.y,
                 {
                   sz.x - scrollBarsPadding.x,
                   0,
                   scrollBarsPadding.x,
                   sz.y,
                 });
    }
    if (scrollBarsPadding.y > 0) {
      sliderBox(target,
                "horizontal",
                &scrollOffset->x,
                0,
                sz.x,
                {
                  0,
                  sz.y - scrollBarsPadding.y,
                  sz.x - scrollBarsPadding.x,
                  scrollBarsPadding.y,
                });
    }
  }
};

class Scrollable
{
public:
  operator Target() { return client; }

  void end()
  {
    client.end();
    container.end();
  }

private:
  Wrapper<CONTAINER, ScrollablePresenter> container;
  DisplayList::Clip client;
};

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

/**
 * @brief add a scrollable group
 * @ingroup groups
 *
 * @param target the parent group or frame
 * @param id the id
 * @param scrollOffset the scrolling control variable
 * @param r the relative position and the size. If size is 0 it will use a
 * default size. Notice that this is different from group() and panel() behavior
 * @param layout
 * @param style
 * @return group
 */
inline Scrollable
scrollable(Target target,
           std::string_view id,
           SDL_Point* scrollOffset,
           const SDL_Rect& r = {0},
           const ScrollableStyle& style = themeFor<Scrollable>())
{
  return {target, id, scrollOffset, makeScrollableRect(r, target), style};
}
/// @copydoc scrollable()
/// @ingroup groups
inline Scrollable
scrollable(Target target,
           std::string_view id,
           SDL_Point* scrollOffset,
           const SDL_Rect& r,
           Layout layout,
           const ScrollableStyle& style = themeFor<Scrollable>())
{
  return {target, id, scrollOffset, makeScrollableRect(r, target), style};
}

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
inline PanelImpl<Scrollable>
scrollablePanel(Target target,
                std::string_view id,
                SDL_Point* scrollOffset,
                const SDL_Rect& r = {0},
                const ScrollablePanelStyle& style = themeFor<ScrollablePanel>())
{
  return {target,
          id,
          makeScrollableRect(r, target),
          [=](auto t, auto r) {
            return scrollable(t, "client", scrollOffset, r, style);
          },
          style};
}
/// @copydoc scrollablePanel()
/// @ingroup groups
inline PanelImpl<Scrollable>
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
