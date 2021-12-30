#ifndef BOTO_CORE_CONTAINERSTATE_HPP_
#define BOTO_CORE_CONTAINERSTATE_HPP_

#include <climits>
#include <cmath>
#include <string_view>
#include "DisplayList.hpp"
#include "EventDispatcher.hpp"
#include "Layout.hpp"

namespace boto {

constexpr int Undefined = INT_MAX / 2;

struct ContainerState
{
  ContainerState(EventDispatcher::EventTarget&& eventTarget,
                 DisplayList::Clip&& clip,
                 const SDL_Rect& r,
                 const SDL_Point& internalOffset = {},
                 const SDL_Point& endPadding = {},
                 int elementSpacing = 0,
                 Layout layout = Layout::NONE)
    : eventTarget(std::move(eventTarget))
    , clip(std::move(clip))
    , offset({r.x + internalOffset.x, r.y + internalOffset.y})
    , endPos(offset)
    , endPadding(endPadding)
    , elementSpacing(elementSpacing)
    , layout(layout)
  {}
  ContainerState(DisplayList& dList,
                 EventDispatcher& dispatcher,
                 std::string_view id,
                 const SDL_Rect& r,
                 const SDL_Point& internalOffset = {},
                 const SDL_Point& endPadding = {},
                 Layout layout = Layout::NONE,
                 int elementSpacing = 0)
    : ContainerState(dispatcher.check(RequestEvent::INPUT, r, id),
                     dList.clip(r),
                     r,
                     internalOffset,
                     endPadding,
                     elementSpacing,
                     layout)
  {}

  ContainerState()
    : offset({0})
    , endPos({0})
    , layout(Layout::NONE)
  {}

  void advance(const SDL_Point& p)
  {
    if (layout == Layout::HORIZONTAL) {
      endPos.x += p.x + elementSpacing;
    } else {
      endPos.x = std::max(p.x + offset.x, endPos.x);
    }
    if (layout == Layout::VERTICAL) {
      endPos.y += p.y + elementSpacing;
    } else {
      endPos.y = std::max(p.y + offset.y, endPos.y);
    }
  }

  SDL_Point caret() const
  {
    return {
      layout == Layout::HORIZONTAL ? endPos.x : offset.x,
      layout == Layout::VERTICAL ? endPos.y : offset.y,
    };
  }

  SDL_Point size() const
  {
    auto& r = eventTarget.rect();
    return {
      std::max(0, r.w != Undefined ? r.w : endPos.x - r.x + endPadding.x),
      std::max(0, r.h != Undefined ? r.h : endPos.y - r.y + endPadding.y),
    };
  }

  SDL_Rect rect() const
  {
    auto& r = eventTarget.rect();
    auto sz = size();
    return {r.x, r.y, sz.x, sz.y};
  }

  bool wasUndefined() const
  {
    auto& r = eventTarget.rect();
    return r.w == Undefined || r.h == Undefined;
  }

  EventDispatcher::EventTarget eventTarget;
  DisplayList::Clip clip;
  SDL_Point offset;
  SDL_Point endPos;
  SDL_Point endPadding;
  int elementSpacing;
  Layout layout;
};

}

#endif // BOTO_CORE_CONTAINERSTATE_HPP_
