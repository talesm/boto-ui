#ifndef BOTO_SLIDERFIELDSTYLE_HPP_
#define BOTO_SLIDERFIELDSTYLE_HPP_

#include <SDL.h>
#include "LabelStyle.hpp"
#include "SliderBoxStyle.hpp"
#include "Theme.hpp"

namespace boto {

struct SliderFieldStyle
{
  SliderBoxStyle scroll;
  ControlStyle label;
};
struct SliderField;

namespace style {

template<class Theme>
struct FromTheme<SliderField, Theme>
{
  constexpr static SliderFieldStyle get()
  {
    return {
      themeFor<SliderBox, Theme>(),
      themeFor<Label, Theme>(),
    };
  }
};
} // namespace style

} // namespace boto

#endif // BOTO_SLIDERFIELDSTYLE_HPP_
