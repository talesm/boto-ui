#pragma once
#include "core/Layout.hpp"

namespace boto {

struct TargetStyle
{
  int elementSpacing;
  Layout layout;

  constexpr TargetStyle withElementSpacing(int elementSpacing) const
  {
    return {elementSpacing, layout};
  }
  constexpr TargetStyle withLayout(Layout layout) const
  {
    return {elementSpacing, layout};
  }
};

} // namespace boto
