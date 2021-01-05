#ifndef DUI_SLIDERFIELDSTYLE_HPP_
#define DUI_SLIDERFIELDSTYLE_HPP_

#include <SDL.h>
#include "LabelStyle.hpp"
#include "SliderBoxStyle.hpp"
#include "Theme.hpp"

namespace dui {

struct SliderFieldStyle
{
  SliderBoxStyle scroll;
  ElementStyle label;
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

} // namespace dui

#endif // DUI_SLIDERFIELDSTYLE_HPP_
