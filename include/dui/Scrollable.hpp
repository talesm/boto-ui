#pragma once

#include <string_view>
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

struct Scrollable
{
  ScrollableStyle style;
  Wrapper wrapper;
  SDL_Point* scrollOffset;

  Scrollable(Target parent,
             std::string_view id,
             SDL_Point* scrollOffset,
             const SDL_Rect& r,
             Layout layout,
             const ScrollableStyle& style)
    : style(style)
    , wrapper(parent, id, r, layout, evalPadding(style))
    , scrollOffset(scrollOffset)
  {
    wrapper.scroll(*scrollOffset);
  }

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
                   wrapperSize.y - padding.bottom,
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

inline Scrollable
scrollable(Target parent,
           std::string_view id,
           SDL_Point* scrollOffset,
           const SDL_Rect& r,
           Layout layout = Layout::VERTICAL,
           const ScrollableStyle& style = themeFor<Scrollable>())
{
  return {parent, id, scrollOffset, r, layout, style};
}
} // namespace dui
