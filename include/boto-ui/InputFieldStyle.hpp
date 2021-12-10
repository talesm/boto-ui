#ifndef DUI_INPUTFIELDSTYLE_HPP_
#define DUI_INPUTFIELDSTYLE_HPP_

#include "InputBoxStyle.hpp"
#include "LabelStyle.hpp"
#include "Theme.hpp"

namespace boto {

struct InputFieldStyle
{
  InputBoxStyle box;
  ElementStyle label;
};

struct TextField;
struct IntField;
struct DoubleField;
struct FloatField;

namespace style {

template<class Box, class Theme>
struct FieldFromThemeBase
{
  constexpr static InputFieldStyle get()
  {
    return {
      themeFor<Box, Theme>(),
      themeFor<Label, Theme>(),
    };
  }
};

template<class Theme>
struct FromTheme<TextField, Theme> : FieldFromThemeBase<TextBox, Theme>
{};

template<class Theme>
struct FromTheme<IntField, Theme> : FieldFromThemeBase<IntBox, Theme>
{};

template<class Theme>
struct FromTheme<DoubleField, Theme> : FieldFromThemeBase<DoubleBox, Theme>
{};

template<class Theme>
struct FromTheme<FloatField, Theme> : FieldFromThemeBase<FloatBox, Theme>
{};
} // namespace style

} // namespace boto

#endif // DUI_INPUTFIELDSTYLE_HPP_
