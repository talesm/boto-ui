#pragma once

#include "Target.hpp"

namespace boto {

/// A class to make wrapper elements
template<class CONTAINER, class CLIENT>
class Wrapper
{
public:
  Wrapper(CONTAINER&& container, CLIENT&& client)
    : container(std::move(container))
    , client(std::move(client))
  {}
  Wrapper(const Wrapper&) = delete;
  Wrapper(Wrapper&& rhs) = default;
  Wrapper& operator=(const Wrapper& rhs) = delete;
  Wrapper& operator=(Wrapper&& rhs) = default;
  ~Wrapper() { end(); }

  operator Target() & { return client; }
  operator bool() const { return client; }

  const ContainerState& state() const { return client.state(); }

  void end()
  {
    if (container) {
      client.end();
      container.end();
    }
  }

private:
  CONTAINER container;
  CLIENT client;
};

} // namespace boto
