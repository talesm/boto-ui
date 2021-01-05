#ifndef DUI_WRAPPERGROUP_HPP
#define DUI_WRAPPERGROUP_HPP

#include "EdgeSize.hpp"
#include "Group.hpp"

namespace dui {

/// A class to make wrapper elements
class Wrapper
{
  Group wrapper;
  EdgeSize padding;
  Group client;
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
  Wrapper(Target parent,
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

  void end();

  void scroll(const SDL_Point& offset)
  {
    if (onClient) {
      client.scroll(offset);
    } else {
      wrapper.scroll(offset);
    }
  }
};

inline SDL_Point
Wrapper::endClient()
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
Wrapper::end()
{
  SDL_assert(!onClient);
  wrapper.end();
}

} // namespace dui

#endif // DUI_WRAPPERGROUP_HPP
