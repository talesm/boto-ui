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
  CLIENT client;
  bool valid = false;
  bool autoW;
  bool autoH;

public:
  Wrapper(Target parent, const EdgeSize& padding, ClientInitializer initializer)
    : padding(padding)
    , initializer(std::move(initializer))
    , client(this->initializer(parent, clientRect(padding, parent.getRect())))
    , autoW(parent.getRect().w == 0)
    , autoH(parent.getRect().h == 0)
  {
    valid = true;
  }
  Wrapper(const Wrapper&) = delete;
  Wrapper(Wrapper&&) = delete;

  Wrapper(Wrapper&& rhs, Target parent)
    : padding(rhs.padding)
    , initializer(rhs.initializer)
    , client(initializer(parent, clientRect(padding, parent.getRect())))
    , valid(rhs.valid)
    , autoW(rhs.autoW)
    , autoH(rhs.autoH)
  {
    rhs.valid = false;
  }
  Wrapper& operator=(const Wrapper& rhs) = delete;
  Wrapper& operator=(Wrapper&& rhs) = delete;

  ~Wrapper()
  {
    if (valid) {
      end();
    }
  }

  operator Target() & { return client; }
  operator bool() const { return valid && bool(client); }

  SDL_Point end();
};

template<class CLIENT>
inline SDL_Point
Wrapper<CLIENT>::end()
{
  SDL_assert(valid);
  SDL_Point sz{client.width() + padding.left + padding.right,
               client.height() + padding.top + padding.bottom};
  client.end();
  valid = false;
  return sz;
}

} // namespace dui
