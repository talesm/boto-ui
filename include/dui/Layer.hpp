#pragma once
#include "Group.hpp"

namespace dui {

class LayerImpl : public Targetable<LayerImpl>
{
  class LayerGuard
  {
    State* state = nullptr;

  public:
    LayerGuard(State& state)
      : state(&state)
    {
      state.pushLayer();
    }

    LayerGuard(LayerGuard&& rhs)
    {
      state = rhs.state;
      rhs.state = nullptr;
    }

    ~LayerGuard() { reset(); }

    void reset()
    {
      if (state) {
        state->popLayer();
        state = nullptr;
      }
    }
  };
  LayerGuard guard;
  Target parent;
  std::string_view id;
  bool locked = false;
  bool ended = false;
  SDL_Rect rect;
  SDL_Point topLeft;
  SDL_Point bottomRight;

public:
  LayerImpl(Target parent, std::string_view id, const SDL_Rect& rect)
    : guard(parent.getState())
    , parent(parent)
    , id(id)
    , rect(rect)
    , topLeft({rect.x, rect.y})
    , bottomRight({rect.x + rect.w, rect.y + rect.h})
  {
    parent.lock(id, rect);
  }

  LayerImpl(const LayerImpl&) = delete;
  LayerImpl(LayerImpl&& rhs)
    : guard(std::move(rhs.guard))
    , parent(rhs.parent)
    , id(rhs.id)
    , locked(rhs.locked)
    , ended(rhs.ended)
    , rect(rhs.rect)
    , topLeft(rhs.topLeft)
    , bottomRight(rhs.bottomRight)
  {
    rhs.ended = true;
  }

  ~LayerImpl()
  {
    if (!ended) {
      end();
    }
  }

  void end()
  {
    SDL_assert(!ended);
    if (rect.w == 0) {
      rect.w = width();
    }
    if (rect.h == 0) {
      rect.h = height();
    }
    parent.unlock(id, rect);
    ended = true;
    parent = {};
    guard.reset();
  }

  /// Convert to target object
  operator bool() const { return !ended; }
  operator Target() &
  {
    return {
      &parent.getState(),
      id,
      rect,
      topLeft,
      bottomRight,
      locked,
      {0, Layout::NONE},
    };
  }
};

inline LayerImpl
layer(Target target, std::string_view id, const SDL_Rect& r)
{
  return LayerImpl(target, id, r);
}
} // namespace dui
