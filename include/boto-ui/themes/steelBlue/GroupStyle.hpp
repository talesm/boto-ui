#pragma once

#include "Theme.hpp"
#include "core/Layout.hpp"

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

namespace style {
/// Default panel style
template<class Theme>
struct FromTheme<Group, Theme>
{
  constexpr static GroupStyle get()
  {
    return {
      2,                // Element spacing
      Layout::VERTICAL, // Layout,
    };
  }
};
} // namespace style

} // namespace boto
