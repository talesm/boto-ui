#pragma once
#include <ostream>
#include "core/Status.hpp"

namespace std {
std::ostream&
operator<<(std::ostream& out, boto::StatusFlags flag)
{
  if (!flag) {
    out << "[]";
    return out;
  }
  out.put('[');
  if (flag.test(boto::Status::HOVERED)) {
    out.put('h');
  }
  if (flag.test(boto::Status::GRABBED)) {
    out.put('g');
  }
  if (flag.test(boto::Status::FOCUSED)) {
    out.put('f');
  }
  out.put(']');
  return out;
}
}
