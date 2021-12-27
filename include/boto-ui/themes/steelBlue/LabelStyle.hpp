#ifndef BOTO_LABELSTYLE_HPP_
#define BOTO_LABELSTYLE_HPP_

#include "ControlStyle.hpp"
#include "Theme.hpp"

namespace boto {

struct Label;

namespace style {
template<class Theme>
struct FromTheme<Label, Theme>
{
  constexpr static ControlStyle get()
  {
    return themeFor<Control, Theme>()
      .withBorderSize(EdgeSize::all(0))
      .withPadding(EdgeSize::all(2))
      .withBackgroundColor({});
  }
};
} // namespace style

} // namespace boto

#endif // BOTO_LABELSTYLE_HPP_
