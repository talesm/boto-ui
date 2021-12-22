#ifndef BOTO_INPUTBOXSTYLE_HPP_
#define BOTO_INPUTBOXSTYLE_HPP_

#include <SDL.h>
#include "ControlStyle.hpp"
#include "Theme.hpp"

namespace boto {

/// Input box style
struct InputBoxStyle
{
  EdgeSize padding;
  EdgeSize border;
  Font font;
  int scale;
  ControlPaintStyle normal;
  ControlPaintStyle active;
};

struct InputBoxBase;
struct TextBox;
struct NumberBox;
struct IntBox;
struct DoubleBox;
struct FloatBox;

namespace style {

template<class Theme>
struct FromTheme<InputBoxBase, Theme>
{
  constexpr static InputBoxStyle get()
  {
    auto control = themeFor<Control, Theme>();
    return {
      EdgeSize::all(2),
      EdgeSize::all(1),
      control.font,
      control.scale,
      {control.paint.text,
       {240, 240, 240, 255},
       BorderColorStyle::all(control.paint.text)},
      {control.paint.text,
       {255, 255, 255, 255},
       BorderColorStyle::all(control.paint.text)},
    };
  }
};

template<class Theme>
struct FromTheme<TextBox, Theme> : FromTheme<InputBoxBase, Theme>
{};
template<class Theme>
struct FromTheme<NumberBox, Theme> : FromTheme<InputBoxBase, Theme>
{};
template<class Theme>
struct FromTheme<IntBox, Theme> : FromTheme<NumberBox, Theme>
{};
template<class Theme>
struct FromTheme<DoubleBox, Theme> : FromTheme<NumberBox, Theme>
{};
template<class Theme>
struct FromTheme<FloatBox, Theme> : FromTheme<NumberBox, Theme>
{};
}
} // namespace boto

#endif // BOTO_INPUTBOXSTYLE_HPP_
