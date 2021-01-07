#pragma once

#include "TargetStyle.hpp"
#include "Theme.hpp"

namespace dui {

using GroupStyle = TargetStyle;

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

} // namespace dui
