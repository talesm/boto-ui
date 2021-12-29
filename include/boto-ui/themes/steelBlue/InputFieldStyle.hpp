#ifndef BOTO_INPUTFIELDSTYLE_HPP_
#define BOTO_INPUTFIELDSTYLE_HPP_

#include "InputBoxStyle.hpp"
#include "LabelStyle.hpp"
#include "core/Theme.hpp"

namespace boto {

struct InputFieldStyle
{
  InputBoxStyle box;
  ControlStyle label;
};

struct TextField;
struct IntField;
struct DoubleField;
struct FloatField;

template<class Box>
struct FieldFromThemeBase
{
  static InputFieldStyle get(Theme& theme)
  {
    return {theme.of<Box>(), theme.of<Label>()};
  }
};

template<>
struct StyleFor<SteelBlue, TextField> : FieldFromThemeBase<TextBox>
{};

template<>
struct StyleFor<SteelBlue, IntField> : FieldFromThemeBase<IntBox>
{};

template<>
struct StyleFor<SteelBlue, DoubleField> : FieldFromThemeBase<DoubleBox>
{};

template<>
struct StyleFor<SteelBlue, FloatField> : FieldFromThemeBase<FloatBox>
{};

} // namespace boto

#endif // BOTO_INPUTFIELDSTYLE_HPP_
