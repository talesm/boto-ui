#ifndef BOTO_CORE_CONTAINER_HPP_
#define BOTO_CORE_CONTAINER_HPP_

#include <climits>
#include <cmath>
#include <string_view>
#include "DisplayList.hpp"
#include "EventDispatcher.hpp"
#include "Frame.hpp"
#include "Layout.hpp"

namespace boto {

constexpr int Undefined = INT_MAX / 2;

struct ContainerState
{
  ContainerState(DisplayList& dList,
                 EventDispatcher& dispatcher,
                 std::string_view id,
                 const SDL_Rect& r,
                 const SDL_Point& internalOffset,
                 Layout layout,
                 int elementSpacing = 0)
    : target(dispatcher.check(RequestEvent::INPUT, r, id))
    , clip(dList.clip(r))
    , offset({r.x + internalOffset.x, r.y + internalOffset.y})
    , endPos(offset)
    , elementSpacing(elementSpacing)
    , layout(layout)
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

  const SDL_Point& caret() const { return offset; }

  SDL_Point size() const
  {
    auto& r = target.rect();
    return {
      std::max(0, r.w != Undefined ? r.w : endPos.x - r.x + endPadding.x),
      std::max(0, r.h != Undefined ? r.h : endPos.y - r.y + endPadding.y),
    };
  }

  bool wasUndefined() const
  {
    auto& r = target.rect();
    return r.w == Undefined || r.h == Undefined;
  }

  EventDispatcher::EventTarget target;
  DisplayList::Clip clip;
  SDL_Point offset;
  SDL_Point endPos;
  SDL_Point endPadding{0};
  int elementSpacing;
  Layout layout;
};

}

#endif // BOTO_CORE_CONTAINER_HPP_
