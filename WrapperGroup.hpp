#ifndef DUI_WRAPPERGROUP_HPP
#define DUI_WRAPPERGROUP_HPP

#include "EdgeSize.hpp"
#include "Group.hpp"

namespace dui {

/// A class to make wrapper elements
class WrapperGroup
{
  Group wrapper;
  EdgeSize padding;
  Group client;
  bool onClient = false;
  bool ended = false;
  bool autoW;
  bool autoH;

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
    : wrapper(parent, id, rect, Layout::NONE)
    , padding(padding)
    , client(wrapper, {}, paddedSize(rect, padding), layout)
    , autoW(rect.w == 0)
    , autoH(rect.h == 0)
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

inline void
WrapperGroup::endWrapper()
{
  SDL_assert(!ended && !onClient);
  wrapper.end();
  ended = true;
}

} // namespace dui

#endif // DUI_WRAPPERGROUP_HPP
