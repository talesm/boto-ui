#pragma once

namespace dui {

enum class Layout : Uint8
{
  NONE,
  VERTICAL,
  HORIZONTAL,
};

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

} // namespace dui
