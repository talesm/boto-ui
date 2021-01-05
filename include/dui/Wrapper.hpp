#pragma once

#include "EdgeSize.hpp"
#include "Group.hpp"

namespace dui {

/// A class to make wrapper elements
template<class CLIENT>
class Wrapper
{
  EdgeSize padding;
  Group wrapper;
  CLIENT client;
  bool onClient = false;
  bool autoW;
  bool autoH;

  static constexpr SDL_Rect paddedSize(const SDL_Rect& rect,
                                       const EdgeSize& padding)
  {
    int pw = padding.left + padding.right;
    int dw = rect.w > pw ? pw : 0;
    int ph = padding.top + padding.bottom;
    int dh = rect.h > ph ? ph : 0;
    return {padding.left, padding.top, rect.w - dw, rect.h - dh};
  }

public:
  template<class FUNC>
  Wrapper(Target parent,
          std::string_view id,
          const SDL_Rect& rect,
          const EdgeSize& padding,
          FUNC initializer)
    : padding(padding)
    , wrapper(parent, id, {0}, rect, Layout::NONE)
    , client(initializer(wrapper, paddedSize(rect, padding)))
    , autoW(rect.w == 0)
    , autoH(rect.h == 0)
  {
    onClient = true;
  }
  Wrapper(Wrapper&&) = default;
  Wrapper& operator=(Wrapper&&) = default;

  ~Wrapper()
  {
    SDL_assert(!onClient);
    if (wrapper) {
      end();
    }
  }

  operator Target() & { return onClient ? Target{client} : Target{wrapper}; }
  operator bool() const { return onClient || bool(wrapper); }

  SDL_Point endClient();

  void end()
  {
    SDL_assert(!onClient);
    wrapper.end();
  }
};

template<class CLIENT>
inline SDL_Point
Wrapper<CLIENT>::endClient()
{
  SDL_assert(onClient);
  if (autoW) {
    wrapper.width(client.width() + padding.left + padding.right);
  }
  if (autoH) {
    wrapper.height(client.height() + padding.top + padding.bottom);
  }
  client.end();
  onClient = false;
  return {wrapper.width(), wrapper.height()};
}

} // namespace dui
