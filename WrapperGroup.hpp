#ifndef DUI_WRAPPERGROUP_HPP
#define DUI_WRAPPERGROUP_HPP

#include "EdgeSize.hpp"
#include "Group.hpp"

namespace dui {

/// A class to make wrapper elements
class WrapperGroup
{
  struct Wrapper
  {
    Target parent;
    std::string_view id;
    SDL_Rect rect;
    SDL_Point topLeft;
    SDL_Point bottomRight;
    bool locked = false;

    Wrapper(Target parent, std::string_view id, const SDL_Rect& rect)
      : parent(parent)
      , id(id)
      , rect(rect)
      , topLeft(makeCaret(parent.getCaret(), rect.x, rect.y))
      , bottomRight(topLeft)
    {
      parent.lock(id, rect);
    }

    operator Target() &
    {
      auto s = &parent.getState();
      return {s, id, rect, topLeft, bottomRight, Layout::NONE, locked};
    }
    void end()
    {
      parent.unlock(id, rect);
      parent.advance({rect.x + rect.w, rect.y + rect.h});
    }
  };

  Wrapper wrapper;
  EdgeSize padding;
  Group client;
  bool onClient = false;
  bool ended = false;

  static constexpr SDL_Rect paddedSize(SDL_Rect rect, const EdgeSize& padding)
  {
    rect.x += padding.left;
    rect.y += padding.top;
    if (rect.w > padding.left + padding.right) {
      rect.w -= padding.left + padding.right;
    }
    if (rect.h > padding.top + padding.bottom) {
      rect.h -= padding.top + padding.bottom;
    }
    return rect;
  }

public:
  WrapperGroup(Target parent,
               std::string_view id,
               const SDL_Rect& rect,
               Layout layout,
               const EdgeSize& padding)
    : wrapper(parent, id, rect)
    , padding(padding)
    , client(wrapper, {}, paddedSize(rect, padding), layout)
  {
    onClient = true;
  }

  operator Target() & { return onClient ? Target{client} : Target{wrapper}; }
  operator bool() const { return !ended; }

  SDL_Point endClient();

  void endWrapper();
};

inline SDL_Point
WrapperGroup::endClient()
{
  SDL_assert(onClient);
  if (wrapper.rect.w == 0) {
    wrapper.rect.w = client.width() + padding.left + padding.right;
  }
  if (wrapper.rect.h == 0) {
    wrapper.rect.h = client.height() + padding.top + padding.bottom;
  }
  client.end();
  onClient = false;
  return {wrapper.rect.w, wrapper.rect.h};
}

inline void
WrapperGroup::endWrapper()
{
  SDL_assert(!ended && !onClient);
  wrapper.end();
  ended = true;
}

} // namespace dui

#endif // DUI_WRAPPERGROUP_HPP
