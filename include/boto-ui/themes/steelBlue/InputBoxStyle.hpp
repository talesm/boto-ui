#ifndef BOTO_INPUTBOXSTYLE_HPP_
#define BOTO_INPUTBOXSTYLE_HPP_

#include <SDL.h>
#include "ControlStyle.hpp"
#include "core/Theme.hpp"

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

template<>
struct StyleFor<SteelBlue, InputBoxBase>
{
  static InputBoxStyle get(Theme& theme)
  {
    return {
      theme.of<Control>().withBackground({240, 240, 240, 255}),
      theme.of<Control>().withBackground({255, 255, 255, 255}),
    };
  }
};

template<>
struct StyleFor<SteelBlue, TextBox> : StyleFor<SteelBlue, InputBoxBase>
{};
template<>
struct StyleFor<SteelBlue, NumberBox> : StyleFor<SteelBlue, InputBoxBase>
{};
template<>
struct StyleFor<SteelBlue, IntBox> : StyleFor<SteelBlue, NumberBox>
{};
template<>
struct StyleFor<SteelBlue, DoubleBox> : StyleFor<SteelBlue, NumberBox>
{};
template<>
struct StyleFor<SteelBlue, FloatBox> : StyleFor<SteelBlue, NumberBox>
{};
} // namespace boto

#endif // BOTO_INPUTBOXSTYLE_HPP_
