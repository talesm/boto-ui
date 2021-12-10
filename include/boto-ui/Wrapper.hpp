#pragma once

#include <functional>
#include "EdgeSize.hpp"
#include "Group.hpp"

namespace boto {

/// A class to make wrapper elements
template<class CLIENT>
class Wrapper : public Targetable<Wrapper<CLIENT>>
{
  using ClientInitializer = std::function<CLIENT(Target, const SDL_Rect&)>;
  EdgeSize padding;
  ClientInitializer initializer;
  Group decoration;
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
  Wrapper(Target parent,
          std::string_view id,
          const SDL_Rect& rect,
          const EdgeSize& padding,
          ClientInitializer initializer)
    : padding(padding)
    , initializer(initializer)
    , decoration(parent, id, {0}, rect, {0, Layout::NONE})
    , client(this->initializer(decoration, paddedSize(rect, padding)))
    , autoW(rect.w == 0)
    , autoH(rect.h == 0)
  {
    onClient = true;
  }
  Wrapper(const Wrapper&) = delete;
  Wrapper(Wrapper&& rhs)
    : padding(rhs.padding)
    , initializer(rhs.initializer)
    , decoration(std::move(rhs.decoration))
    , client(initializer(decoration, paddedSize(decoration.getRect(), padding)))
    , onClient(rhs.onClient)
    , autoW(rhs.autoW)
    , autoH(rhs.autoH)
  {
    rhs.onClient = false;
  }
  Wrapper& operator=(const Wrapper& rhs) = delete;
  Wrapper& operator=(Wrapper&& rhs)
  {
    this->~Wrapper();
    new (this) Wrapper(std::move(rhs));
    return *this;
  }

  ~Wrapper()
  {
    SDL_assert(!onClient);
    if (decoration) {
      end();
    }
  }

  operator Target() & { return onClient ? Target{client} : Target{decoration}; }
  operator bool() const { return onClient || bool(decoration); }

  SDL_Point endClient();

  void end()
  {
    SDL_assert(!onClient);
    decoration.end();
  }
};

template<class CLIENT>
inline SDL_Point
Wrapper<CLIENT>::endClient()
{
  SDL_assert(onClient);
  if (autoW) {
    decoration.setWidth(client.width() + padding.left + padding.right);
  }
  if (autoH) {
    decoration.setHeight(client.height() + padding.top + padding.bottom);
  }
  client.end();
  onClient = false;
  return {decoration.width(), decoration.height()};
}

} // namespace boto
