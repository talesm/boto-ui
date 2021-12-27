#ifndef BOTO_CORE_CONTAINER_HPP_
#define BOTO_CORE_CONTAINER_HPP_

#include <string_view>
#include "ContainerState.hpp"
#include "Layout.hpp"
#include "State.hpp"

namespace boto {

/**
 * @brief A container
 *
 */
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
