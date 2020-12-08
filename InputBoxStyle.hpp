#ifndef DUI_INPUTBOXSTYLE_HPP_
#define DUI_INPUTBOXSTYLE_HPP_

#include <SDL.h>
#include "ElementStyle.hpp"
#include "theme.hpp"

namespace dui {

/// Input box style
struct InputBoxStyle
{
  EdgeSize padding;
  EdgeSize border;
  Font font;
  int scale;
  ElementPaintStyle normal;
  ElementPaintStyle active;
};

struct InputBoxBase;
struct TextBox;
struct NumberBox;
struct IntBox;
struct DoubleBox;

namespace style {

template<class Theme>
struct FromTheme<InputBoxBase, Theme>
{
  constexpr static InputBoxStyle get()
  {
    auto element = themeFor<Element, Theme>();
    return {
      EdgeSize::all(2),
      EdgeSize::all(1),
      element.font,
      element.scale,
      {element.paint.text,
       {240, 240, 240, 255},
       BorderColorStyle::all(element.paint.text)},
      {element.paint.text,
       {255, 255, 255, 255},
       BorderColorStyle::all(element.paint.text)},
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
}
} // namespace dui

#endif // DUI_INPUTBOXSTYLE_HPP_
