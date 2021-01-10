#pragma once

#include <string_view>
#include "Panel.hpp"
#include "ScrollableStyle.hpp"
#include "SliderBox.hpp"
#include "Wrapper.hpp"

namespace dui {

constexpr EdgeSize
evalPadding(const ScrollableStyle& style)
{
  EdgeSize padding{0, 0};
  auto buttonStyle = style.slider.buttons;
  if (!style.fixHorizontal) {
    padding.right = buttonStyle.padding.left + buttonStyle.padding.right +
                    buttonStyle.border.left + buttonStyle.border.right + 8;
  }
  if (!style.fixVertical) {
    padding.bottom = buttonStyle.padding.top + buttonStyle.padding.bottom +
                     buttonStyle.border.top + buttonStyle.border.bottom + 8;
  }
  return padding;
}

/// Scrollable class. @see scrollable() and scrollablePanel()
class Scrollable : public Targetable<Scrollable>
{
  ScrollableStyle style;
  Wrapper<Group> wrapper;
  SDL_Point* scrollOffset;

public:
  Scrollable(Target parent,
             std::string_view id,
             SDL_Point* scrollOffset,
             const SDL_Rect& r,
             const ScrollableStyle& style)
    : style(style)
    , wrapper(parent,
              id,
              r,
              evalPadding(style),
              [=](auto t, auto r) {
                return offsetGroup(t, "client", *scrollOffset, r, style);
              })
    , scrollOffset(scrollOffset)
  {}

  Scrollable(Scrollable&&) = default;
  Scrollable& operator=(Scrollable&&) = default;

  ~Scrollable()
  {
    if (wrapper) {
      end();
    }
  }

  void end()
  {
    SDL_Point clientSize{0}; //{wrapper.width(), wrapper.height()};
    {
      Target client{wrapper};
      clientSize = {client.contentWidth(), client.contentHeight()};
    }
    SDL_Point wrapperSize = wrapper.endClient();
    auto padding = evalPadding(style);
    if (padding.right > 0) {
      sliderBoxV(wrapper,
                 "vertical",
                 &scrollOffset->y,
                 0,
                 clientSize.y,
                 {
                   wrapperSize.x - padding.right,
                   0,
                   padding.right,
                   wrapperSize.y,
                 });
    }
    if (padding.bottom > 0) {
      sliderBox(wrapper,
                "horizontal",
                &scrollOffset->x,
                0,
                clientSize.x,
                {
                  0,
                  wrapperSize.y - padding.bottom,
                  wrapperSize.x - padding.right,
                  padding.bottom,
                });
    }
    wrapper.end();
  }

  operator bool() const { return wrapper; }

  operator Target() { return wrapper; }
};

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

inline SDL_Rect
makeScrollableRect(const SDL_Rect& r, Target target)
{
  auto sz = makeScrollableSize({r.w, r.h}, target);
  return {r.x, r.y, sz.x, sz.y};
}

/**
 * @[
 * @brief add a scrollable group
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
inline Scrollable
scrollable(Target target,
           std::string_view id,
           SDL_Point* scrollOffset,
           const SDL_Rect& r,
           Layout layout = Layout::VERTICAL,
           const ScrollableStyle& style = themeFor<Scrollable>())
{
  return {target, id, scrollOffset, makeScrollableRect(r, target), style};
}
///@]

/**
 * @[
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
///@]
} // namespace dui
