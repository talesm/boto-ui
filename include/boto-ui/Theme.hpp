#ifndef BOTO_THEME_HPP_
#define BOTO_THEME_HPP_

#include "core/Theme.hpp"

namespace boto {

namespace style {

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

template<class T>
struct StyleFor<SteelBlue, T>
{
  static auto get(ThemeT<SteelBlue>& theme)
  {
    return style::FromTheme<T, SteelBlue>::get();
  }
};

} // namespace boto

#include "themes/steelBlue/steelBlue.hpp"
#endif // BOTO_THEME_HPP_
