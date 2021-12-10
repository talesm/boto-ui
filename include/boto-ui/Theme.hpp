#ifndef DUI_THEME_HPP_
#define DUI_THEME_HPP_

namespace boto {

namespace style {

// Default theme
struct SteelBlue;

#ifndef DUI_THEME
#define DUI_THEME boto::style::SteelBlue
#endif

template<class Element, class Theme>
struct FromTheme;

template<class Element, class BaseTheme>
struct DerivedTheme
{
  static constexpr auto get() { return FromTheme<Element, BaseTheme>::get(); }
};

} // namespace style

template<class Element, class Theme = DUI_THEME>
constexpr auto
themeFor()
{
  return style::FromTheme<Element, Theme>::get();
}

} // namespace boto

#endif // DUI_THEME_HPP_
