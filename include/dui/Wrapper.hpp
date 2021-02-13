#pragma once

#include <functional>
#include "EdgeSize.hpp"
#include "Group.hpp"

namespace dui {

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

public:
  Wrapper(Target parent,
          std::string_view id,
          const SDL_Rect& rect,
          const EdgeSize& padding,
          ClientInitializer initializer)
    : padding(padding)
    , initializer(initializer)
    , decoration(parent, id, {0}, rect, {0, Layout::NONE})
    , client(this->initializer(decoration, clientRect(padding, rect)))
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
    , client(initializer(decoration, clientRect(padding, decoration.getRect())))
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

} // namespace dui
