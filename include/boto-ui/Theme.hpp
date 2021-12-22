#ifndef BOTO_THEME_HPP_
#define BOTO_THEME_HPP_

namespace boto {

namespace style {

struct SteelBlue;

#ifndef BOTO_THEME
#define BOTO_THEME boto::style::SteelBlue
#endif

template<class Element, class Theme>
struct FromTheme;

template<class Element, class BaseTheme>
struct DerivedTheme
{
  static constexpr auto get() { return FromTheme<Element, BaseTheme>::get(); }
};

} // namespace style

template<class Element, class Theme = BOTO_THEME>
constexpr auto
themeFor()
{
  return style::FromTheme<Element, Theme>::get();
}

} // namespace boto

#include "themes/steelBlue/steelBlue.hpp"
#endif // BOTO_THEME_HPP_
