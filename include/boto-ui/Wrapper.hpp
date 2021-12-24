#pragma once

#include <functional>
#include "Target.hpp"

namespace boto {

/// A class to make wrapper elements
template<class CONTAINER, class FINALIZER>
class Wrapper
{
public:
  Wrapper(CONTAINER&& c, FINALIZER&& finalizer)
    : container(std::move(c))
    , finalizer(std::forward<FINALIZER>(finalizer))
  {}
  Wrapper(const Wrapper&) = delete;
  Wrapper(Wrapper&& rhs) = default;
  Wrapper& operator=(const Wrapper& rhs) = delete;
  Wrapper& operator=(Wrapper&& rhs) = default;
  ~Wrapper() { end(); }

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
