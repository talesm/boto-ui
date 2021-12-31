#ifndef BOTO_LABELSTYLE_HPP_
#define BOTO_LABELSTYLE_HPP_

#include "ControlStyle.hpp"
#include "core/Theme.hpp"

namespace boto {

struct Label;

template<>
struct StyleFor<SteelBlue, Label>
{
  static ControlStyle get(Theme& theme)
  {
    return theme.of<Control>()
      .withBorderSize(EdgeSize::all(0))
      .withPadding(EdgeSize::all(2))
      .withBackground({});
  }
};

} // namespace boto

#endif // BOTO_LABELSTYLE_HPP_
