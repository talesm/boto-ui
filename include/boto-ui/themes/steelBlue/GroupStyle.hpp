#pragma once

#include "core/Layout.hpp"
#include "core/Theme.hpp"

namespace boto {

struct GroupStyle
{
  int elementSpacing;
  Layout layout;

  constexpr GroupStyle withElementSpacing(int elementSpacing) const
  {
    return {elementSpacing, layout};
  }
  constexpr GroupStyle withLayout(Layout layout) const
  {
    return {elementSpacing, layout};
  }
};

class Group;

/// Default panel style
template<>
struct StyleFor<SteelBlue, Group>
{
  constexpr static GroupStyle get(Theme&)
  {
    return {
      2,                // Element spacing
      Layout::VERTICAL, // Layout,
    };
  }
};

} // namespace boto
