#pragma once

#include "Target.hpp"

namespace boto {

/// A class to make wrapper elements
template<class CONTAINER, class FINALIZER>
class Decorator
{
public:
  Decorator(CONTAINER&& c, FINALIZER&& finalizer)
    : container(std::move(c))
    , finalizer(std::forward<FINALIZER>(finalizer))
  {}
  Decorator(const Decorator&) = delete;
  Decorator(Decorator&& rhs) = default;
  Decorator& operator=(const Decorator& rhs) = delete;
  Decorator& operator=(Decorator&& rhs) = default;
  ~Decorator() { end(); }

  operator Target() & { return container; }
  operator bool() const { return container; }

  const ContainerState& state() const { return container.state(); }

  void end()
  {
    if (container) {
      finalizer(container);
      container.end();
    }
  }

private:
  CONTAINER container;
  FINALIZER finalizer;
};

} // namespace boto
