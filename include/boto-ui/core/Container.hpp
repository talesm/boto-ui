#ifndef BOTO_CORE_CONTAINER_HPP_
#define BOTO_CORE_CONTAINER_HPP_

#include <climits>
#include <cmath>
#include <string_view>
#include "DisplayList.hpp"
#include "EventDispatcher.hpp"
#include "Layout.hpp"
#include "State.hpp"

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

class Container
{
public:
  Container() = default;

  Container(CookieBase<State, State::ContainerGuard> cookie, size_t index)
    : cookie(std::move(cookie))
    , index(index)
  {}

  auto get() const { return cookie.get(); }

  const ContainerState& state() const { return get()->containers[index]; }

  Container container(std::string_view id,
                      const SDL_Rect& r,
                      const SDL_Point& offset = {},
                      const SDL_Point& endPadding = {},
                      Layout layout = Layout::NONE,
                      int elementSpacing = 0)
  {
    auto s = get();
    SDL_assert(index == s->containers.size() - 1);
    return s->container(id, r, offset, endPadding, layout, elementSpacing);
  }

  EventTargetState element(std::string_view id,
                           const SDL_Rect& r,
                           RequestEvent req = RequestEvent::GRAB)
  {
    auto s = get();
    SDL_assert(index == s->containers.size() - 1);
    return s->element(id, r, req);
  }

  EventTargetState element(const SDL_Rect& r,
                           RequestEvent req = RequestEvent::GRAB)
  {
    auto s = get();
    SDL_assert(index == s->containers.size() - 1);
    return s->element(r, req);
  }

private:
  CookieBase<State, State::ContainerGuard> cookie;
  size_t index;
};

inline EventTargetState
State::element(std::string_view id, SDL_Rect r, RequestEvent req)
{
  if (!containers.empty()) {
    auto& c = containers.back();
    auto caret = c.caret();
    r.x += caret.x;
    r.y += caret.y;
    c.advance({r.w, r.h});
  }
  return dispatcher.check(req, r, id).state();
}

inline Container
State::container(std::string_view id,
                 SDL_Rect r,
                 const SDL_Point& offset,
                 const SDL_Point& endPadding,
                 Layout layout,
                 int elementSpacing)
{
  if (!containers.empty()) {
    auto caret = containers.back().caret();
    r.x += caret.x;
    r.y += caret.y;
  }
  containers.emplace_back(
    dList, dispatcher, id, r, offset, endPadding, layout, elementSpacing);
  return {this, containers.size() - 1};
}

inline void
State::popContainer()
{
  auto& c = containers.back();
  auto sz = c.size();
  if (c.wasUndefined()) {
    dispatcher.shrink(sz.x, sz.y);
  }
  containers.pop_back();
  if (!containers.empty()) {
    containers.back().advance(sz);
  }
}

}

#endif // BOTO_CORE_CONTAINER_HPP_
