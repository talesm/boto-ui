#ifndef BOTO_INPUTBOXSTYLE_HPP_
#define BOTO_INPUTBOXSTYLE_HPP_

#include <SDL.h>
#include "ControlStyle.hpp"
#include "Theme.hpp"

namespace boto {

/// Input box style
struct InputBoxStyle
{
  ControlStyle normal;
  ControlStyle active;
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
    return {
      themeFor<Control, Theme>().withBackgroundColor({240, 240, 240, 255}),
      themeFor<Control, Theme>().withBackgroundColor({255, 255, 255, 255}),
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
