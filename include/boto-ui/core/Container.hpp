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
                 const SDL_Point& internalOffset = {},
                 const SDL_Point& endPadding = {},
                 Layout layout = Layout::NONE,
                 int elementSpacing = 0)
    : eventTarget(dispatcher.check(RequestEvent::INPUT, r, id))
    , clip(dList.clip(r))
    , offset({r.x + internalOffset.x, r.y + internalOffset.y})
    , endPos(offset)
    , endPadding(endPadding)
    , elementSpacing(elementSpacing)
    , layout(layout)
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

  const SDL_Point& caret() const { return offset; }

  SDL_Point size() const
  {
    auto& r = eventTarget.rect();
    return {
      std::max(0, r.w != Undefined ? r.w : endPos.x - r.x + endPadding.x),
      std::max(0, r.h != Undefined ? r.h : endPos.y - r.y + endPadding.y),
    };
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

class Container : public CookieBase<Frame, Frame::ContainerPopper>
{
public:
  Container() = default;

  const ContainerState& state() const { return get()->containers[index]; }

  Container container(std::string_view id,
                      const SDL_Rect& r,
                      const SDL_Point& offset = {},
                      const SDL_Point& endPadding = {},
                      Layout layout = Layout::NONE,
                      int elementSpacing = 0)
  {
    auto frame = get();
    SDL_assert(index == frame->containers.size() - 1);
    return frame->container(id, r, offset, endPadding, layout, elementSpacing);
  }

  EventTargetState element(std::string_view id,
                           const SDL_Rect& r,
                           RequestEvent req = RequestEvent::GRAB)
  {
    auto frame = get();
    SDL_assert(index == frame->containers.size() - 1);
    return frame->element(id, r, req);
  }

  EventTargetState element(const SDL_Rect& r,
                           RequestEvent req = RequestEvent::GRAB)
  {
    auto frame = get();
    SDL_assert(index == frame->containers.size() - 1);
    return frame->element(r, req);
  }

private:
  Container(Frame* frame, size_t index)
    : CookieBase(frame)
    , index(index)
  {}

  friend class Frame;

  size_t index;
};

inline EventTargetState
Frame::element(std::string_view id, SDL_Rect r, RequestEvent req)
{
  auto state = get();
  if (!containers.empty()) {
    auto& c = containers.back();
    auto caret = c.caret();
    r.x += caret.x;
    r.y += caret.y;
    c.advance({r.w, r.h});
  }
  return state->dispatcher.check(req, r, id).state();
}

inline Container
Frame::container(std::string_view id,
                 SDL_Rect r,
                 const SDL_Point& offset,
                 const SDL_Point& endPadding,
                 Layout layout,
                 int elementSpacing)
{
  auto state = get();
  if (!containers.empty()) {
    auto caret = containers.back().caret();
    r.x += caret.x;
    r.y += caret.y;
  }
  containers.emplace_back(state->dList,
                          state->dispatcher,
                          id,
                          r,
                          offset,
                          endPadding,
                          layout,
                          elementSpacing);
  return {this, containers.size() - 1};
}

inline void
Frame::popContainer()
{
  auto sz = containers.back().size();
  containers.pop_back();
  if (!containers.empty()) {
    containers.back().advance(sz);
  }
}

}

#endif // BOTO_CORE_CONTAINER_HPP_
