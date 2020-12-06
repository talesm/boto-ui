#ifndef DUI_THEME_HPP_
#define DUI_THEME_HPP_

namespace dui {

namespace style {

// Default theme
struct SteelBlue
{};

#ifndef DUI_THEME
#define DUI_THEME dui::style::SteelBlue
#endif

template<class StyleType, class Theme = DUI_THEME>
struct FromTheme;

template<class StyleType, class BaseTheme = DUI_THEME>
struct DerivedTheme
{
  static constexpr StyleType get()
  {
    return FromTheme<StyleType, BaseTheme>::get();
  }
};

} // namespace style

template<class StyleType, class BaseTheme = DUI_THEME>
constexpr StyleType
themeFor()
{
  return style::FromTheme<StyleType>::get();
}

} // namespace dui

#endif // DUI_THEME_HPP_
