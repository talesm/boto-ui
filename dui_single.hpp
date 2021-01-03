/*
 * MIT License
 * 
 * Copyright (c) 2020 Tales Miranda
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */
#ifndef DUI_SINGLE_HPP
#define DUI_SINGLE_HPP

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>
#include <SDL.h>

namespace dui {

#ifndef DUI_THEME
#define DUI_THEME dui::style::SteelBlue
#endif

// begin EdgeSize.hpp
struct EdgeSize
{
  Uint8 left;
  Uint8 top;
  Uint8 right;
  Uint8 bottom;

  constexpr static EdgeSize all(Uint8 sz) { return {sz, sz, sz, sz}; }
  constexpr EdgeSize withLeft(Uint8 sz) const
  {
    return {sz, top, right, bottom};
  }
  constexpr EdgeSize withTop(Uint8 sz) const
  {
    return {left, sz, right, bottom};
  }
  constexpr EdgeSize withRight(Uint8 sz) const { return {sz, top, sz, bottom}; }
  constexpr EdgeSize withBottom(Uint8 sz) const { return {sz, top, right, sz}; }

  constexpr EdgeSize operator+(const EdgeSize& rhs) const
  {
    return {Uint8(left + rhs.left),
            Uint8(top + rhs.top),
            Uint8(right + rhs.right),
            Uint8(bottom + rhs.bottom)};
  }
  constexpr EdgeSize operator-(const EdgeSize& rhs) const
  {
    return {Uint8(left - rhs.left),
            Uint8(top - rhs.top),
            Uint8(right - rhs.right),
            Uint8(bottom - rhs.bottom)};
  }
};

constexpr SDL_Point
elementSize(const EdgeSize& edge, const SDL_Point& client = {0})
{
  return {edge.left + edge.right + client.x, edge.top + edge.bottom + client.y};
}
constexpr SDL_Point
clientSize(const EdgeSize& edge, const SDL_Point& element)
{
  return {element.x - edge.left - edge.right,
          element.y - edge.top - edge.bottom};
}

// begin Theme.hpp
namespace style {

// Default theme
struct SteelBlue;





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

// begin BoxStyle.hpp
struct BorderColorStyle
{
  SDL_Color left;
  SDL_Color top;
  SDL_Color right;
  SDL_Color bottom;

  constexpr BorderColorStyle withLeft(SDL_Color c) const
  {
    return {c, top, right, bottom};
  }
  constexpr BorderColorStyle withTop(SDL_Color c) const
  {
    return {left, c, right, bottom};
  }
  constexpr BorderColorStyle withRight(SDL_Color c) const
  {
    return {left, top, c, bottom};
  }
  constexpr BorderColorStyle withBotton(SDL_Color c) const
  {
    return {left, top, right, c};
  }
  constexpr BorderColorStyle invert() const
  {
    return {right, bottom, left, top};
  }

  static constexpr BorderColorStyle all(SDL_Color c) { return {c, c, c, c}; }
};

// Style for element state
struct BoxPaintStyle
{
  SDL_Color background;
  BorderColorStyle border;

  constexpr BoxPaintStyle withBackground(SDL_Color background) const
  {
    return {background, border};
  }
  constexpr BoxPaintStyle withBorder(const BorderColorStyle& border) const
  {
    return {background, border};
  }
};

// Border style
struct BoxStyle
{
  EdgeSize border;
  BoxPaintStyle paint;

  constexpr BoxStyle withBorderSize(const EdgeSize& border) const
  {
    return {border, paint};
  }
  constexpr BoxStyle withPaint(const BoxPaintStyle& paint) const
  {
    return {border, paint};
  }
  constexpr BoxStyle withBackgroundColor(SDL_Color background) const
  {
    return withPaint(paint.withBackground(background));
  }
  constexpr BoxStyle withBorderColor(const BorderColorStyle& border) const
  {
    return withPaint(paint.withBorder(border));
  }
};

struct Box;

namespace style {
/// Default panel style
template<>
struct FromTheme<Box, SteelBlue>
{
  constexpr static BoxStyle get()
  {
    return {
      EdgeSize::all(1),
      {
        {219, 228, 240, 240},
        BorderColorStyle::all({45, 72, 106, 255}),
      },
    };
  }
};
}

// begin defaultFont.h
unsigned char font_bmp[] = {
  0x42, 0x4d, 0x3e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00,
  0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00,
  0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0x00, 0x18, 0x08, 0x18, 0x10, 0x00, 0x00, 0x10, 0x08, 0x38, 0x38,
  0x38, 0x38, 0x00, 0x00, 0x00, 0x3c, 0x18, 0x0c, 0x3c, 0x30, 0x18, 0x00,
  0x18, 0x18, 0x44, 0x7c, 0x44, 0x44, 0xdb, 0x10, 0x66, 0x18, 0x18, 0x0e,
  0x7e, 0x70, 0x3c, 0x7e, 0x1c, 0x38, 0xaa, 0xaa, 0xaa, 0x92, 0x8a, 0x30,
  0x66, 0x81, 0x18, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x1c, 0x38, 0xba, 0xb2,
  0x92, 0xba, 0xda, 0x7e, 0x18, 0xc3, 0xff, 0xff, 0x18, 0xff, 0x00, 0x18,
  0x18, 0x18, 0xaa, 0xaa, 0xaa, 0xaa, 0x92, 0x32, 0x18, 0xc3, 0x7e, 0x0e,
  0x18, 0x70, 0x00, 0x00, 0x10, 0x08, 0x92, 0xb2, 0x82, 0x82, 0xdb, 0x12,
  0x66, 0x81, 0x3c, 0x0c, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44,
  0x44, 0x44, 0x00, 0x06, 0x66, 0x18, 0x18, 0x08, 0x18, 0x10, 0x00, 0x00,
  0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x3f, 0xff, 0xfe, 0x3f, 0xff, 0xfe, 0x3c, 0x3c, 0x7e, 0x7e,
  0x20, 0x66, 0xfe, 0x40, 0x00, 0x00, 0x41, 0x01, 0x01, 0x7f, 0xff, 0xff,
  0x42, 0x42, 0x42, 0x42, 0x70, 0x3c, 0xee, 0x20, 0x00, 0x00, 0x40, 0x00,
  0x01, 0x7f, 0xff, 0xff, 0x42, 0x5a, 0x42, 0x5a, 0xd8, 0x18, 0xfe, 0x1c,
  0x00, 0x00, 0x40, 0x00, 0x01, 0x7f, 0xff, 0xff, 0x42, 0x5a, 0x42, 0x5a,
  0x0c, 0x3c, 0xee, 0x12, 0x00, 0x00, 0x41, 0x01, 0x01, 0x7f, 0xff, 0xff,
  0x42, 0x42, 0x42, 0x42, 0x06, 0x66, 0x6c, 0x12, 0x00, 0x00, 0x3f, 0xff,
  0xfe, 0x3f, 0xff, 0xfe, 0x3c, 0x3c, 0x7e, 0x7e, 0x00, 0x00, 0x38, 0x0c,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x7e, 0x24, 0xfc,
  0x3c, 0x14, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c,
  0x42, 0x7e, 0x3c, 0xfe, 0x3c, 0x14, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xf9, 0x00, 0x42, 0x7a, 0x24, 0x91, 0x3c, 0x08, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0x3c, 0x42, 0x7a, 0x3c, 0x10,
  0x3c, 0x08, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0x3c,
  0x42, 0x7e, 0x24, 0xfc, 0x3c, 0x14, 0x7e, 0x42, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xf9, 0x00, 0x42, 0x7e, 0x3c, 0x7e, 0x18, 0x14, 0x7e, 0x42,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x24, 0x3c, 0x24, 0x7e,
  0x00, 0x08, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x18, 0x18, 0x3c, 0xfc, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x88, 0xf0, 0x80, 0x80, 0x90, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x8c, 0x46, 0x41, 0x60, 0x40,
  0x3c, 0x24, 0x00, 0x00, 0x00, 0x24, 0x00, 0x08, 0x00, 0x00, 0x30, 0x46,
  0x27, 0x21, 0x50, 0x20, 0xbd, 0x24, 0x24, 0x66, 0x00, 0x3c, 0x18, 0x1c,
  0x00, 0x00, 0x68, 0x22, 0x17, 0x13, 0xa8, 0x10, 0xbd, 0x24, 0x24, 0x24,
  0x24, 0x3c, 0x24, 0x1c, 0x00, 0x00, 0x94, 0x11, 0x0f, 0x0e, 0x18, 0x08,
  0x7e, 0x3c, 0x18, 0x00, 0x24, 0x3c, 0x24, 0x3e, 0x00, 0x00, 0x0a, 0x09,
  0x04, 0x0c, 0x00, 0x06, 0x3c, 0x3c, 0x00, 0x00, 0x00, 0x18, 0x18, 0x1c,
  0x00, 0x00, 0x05, 0x05, 0x00, 0x1a, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x70, 0x00, 0x03,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x4a,
  0x00, 0x00, 0x80, 0x00, 0x24, 0x4a, 0x14, 0x14, 0x24, 0x24, 0x00, 0x00,
  0x00, 0x00, 0xbd, 0x70, 0x42, 0x00, 0x7d, 0x2c, 0x99, 0x48, 0x5d, 0x5d,
  0xbd, 0x81, 0x00, 0x00, 0x00, 0x00, 0x42, 0x75, 0x5a, 0x00, 0x36, 0x3e,
  0x42, 0x72, 0x3e, 0x08, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x72,
  0x24, 0x00, 0x04, 0x7e, 0x28, 0x77, 0x1c, 0x1c, 0x3c, 0x5a, 0x00, 0x00,
  0x00, 0x00, 0x7e, 0x87, 0x00, 0x18, 0x00, 0x7e, 0x7e, 0x72, 0x2a, 0x2a,
  0x66, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x5a, 0x05, 0x00, 0x5a, 0x00, 0x6a,
  0x5a, 0x87, 0x2a, 0x7f, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00,
  0x00, 0x24, 0x00, 0x3c, 0x7e, 0x05, 0x7f, 0x3e, 0x7e, 0x7e, 0x00, 0x00,
  0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x00, 0x41, 0x1c,
  0x3c, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
  0x66, 0x66, 0x66, 0x66, 0x2c, 0x00, 0x2c, 0x2c, 0x00, 0x00, 0xff, 0xe7,
  0xe7, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0x24, 0x00, 0x24, 0x24,
  0x00, 0x00, 0xff, 0xe7, 0xe3, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff,
  0x34, 0xff, 0x37, 0xf4, 0x1f, 0xf8, 0x00, 0x66, 0x60, 0x06, 0x70, 0x0e,
  0x60, 0x06, 0x66, 0x66, 0x24, 0x88, 0x20, 0x84, 0x28, 0x44, 0x00, 0x66,
  0x70, 0x0e, 0x60, 0x06, 0x60, 0x06, 0x66, 0x66, 0x2c, 0x22, 0x22, 0x24,
  0x22, 0x0c, 0xff, 0xe7, 0xff, 0xff, 0xe3, 0xc7, 0xff, 0xff, 0xff, 0xe7,
  0x24, 0xff, 0x1f, 0xf8, 0x27, 0xe4, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xe7,
  0xff, 0xff, 0xff, 0xe7, 0x34, 0x00, 0x00, 0x00, 0x34, 0x34, 0x66, 0x66,
  0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x24, 0x00, 0x00, 0x00,
  0x24, 0x24, 0x00, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x66, 0x3c,
  0x66, 0x66, 0x66, 0x00, 0x66, 0x00, 0xff, 0x66, 0x67, 0xe6, 0x3f, 0xfc,
  0x7f, 0xfe, 0x66, 0x7e, 0xe7, 0xe6, 0x67, 0xff, 0xe7, 0x3c, 0xff, 0x66,
  0x63, 0xc6, 0x7f, 0xfe, 0xff, 0xff, 0x66, 0x66, 0xc3, 0xc6, 0x63, 0xff,
  0xc3, 0x7e, 0x00, 0x66, 0x60, 0x06, 0x70, 0x0e, 0xc0, 0x03, 0x66, 0x66,
  0x00, 0x06, 0x60, 0x00, 0x00, 0x66, 0x00, 0x66, 0x70, 0x0e, 0x60, 0x06,
  0xc0, 0x03, 0x66, 0x66, 0x00, 0x06, 0x60, 0x00, 0x00, 0x66, 0xff, 0x66,
  0x7f, 0xfe, 0x63, 0xc6, 0xff, 0xff, 0x66, 0x66, 0xc3, 0xc6, 0x63, 0xc3,
  0xff, 0x7e, 0xff, 0x66, 0x3f, 0xfc, 0x67, 0xe6, 0x7f, 0xfe, 0x7e, 0x66,
  0xe7, 0xe6, 0x67, 0xe7, 0xff, 0x3c, 0x00, 0x66, 0x00, 0x00, 0x66, 0x66,
  0x00, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x18,
  0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x18, 0x18, 0x18, 0x00,
  0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c,
  0x18, 0x18, 0x18, 0x00, 0x18, 0x00, 0x00, 0x18, 0x1c, 0x38, 0x00, 0x00,
  0x60, 0x06, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0xff, 0x18,
  0x1f, 0xf8, 0x0f, 0xf0, 0x7f, 0xfe, 0x18, 0x18, 0xff, 0xf8, 0x1f, 0xff,
  0xff, 0x3c, 0xff, 0x18, 0x0f, 0xf0, 0x1f, 0xf8, 0x7f, 0xfe, 0x18, 0x18,
  0xff, 0xf8, 0x1f, 0xff, 0xff, 0x3c, 0x00, 0x18, 0x00, 0x00, 0x1c, 0x38,
  0x60, 0x06, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00, 0x18,
  0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18,
  0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
  0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x78, 0x07, 0x78, 0x7c, 0x1c, 0x1c,
  0x18, 0x36, 0x36, 0x3c, 0x7e, 0x0c, 0x18, 0x30, 0x00, 0x00, 0x30, 0x3e,
  0x30, 0x06, 0x30, 0x36, 0x3c, 0x6b, 0x1c, 0x06, 0x32, 0x18, 0x18, 0x18,
  0x00, 0x00, 0x3e, 0x66, 0x30, 0x7e, 0x30, 0x36, 0x24, 0x6b, 0x08, 0x3e,
  0x18, 0x10, 0x18, 0x08, 0x00, 0x00, 0x33, 0x66, 0x3b, 0x60, 0x30, 0x36,
  0x66, 0x6b, 0x1c, 0x66, 0x4c, 0x30, 0x18, 0x0c, 0x4c, 0x00, 0x7e, 0x3e,
  0x36, 0x3e, 0x78, 0x77, 0x66, 0x63, 0x36, 0x66, 0x7c, 0x10, 0x18, 0x08,
  0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x18, 0x18, 0x18, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3e, 0x7c, 0x3c, 0x3e, 0x3c, 0x3c, 0x78, 0xe7, 0x3c,
  0x70, 0x73, 0x3c, 0xeb, 0xe7, 0x3c, 0x00, 0x66, 0x66, 0x66, 0x66, 0x60,
  0x18, 0x0c, 0x66, 0x18, 0x18, 0x36, 0x18, 0x6a, 0x66, 0x66, 0x00, 0x3e,
  0x66, 0x60, 0x66, 0x7e, 0x18, 0x7c, 0x66, 0x18, 0x18, 0x3c, 0x18, 0x6a,
  0x66, 0x66, 0x00, 0x06, 0x66, 0x66, 0x66, 0x66, 0x7e, 0xcc, 0x66, 0x18,
  0x18, 0x36, 0x18, 0x6a, 0x66, 0x66, 0x00, 0x3c, 0x7c, 0x3c, 0x3e, 0x3c,
  0x18, 0x7e, 0x7c, 0x38, 0x38, 0x33, 0x18, 0x54, 0x5c, 0x3c, 0x08, 0x00,
  0x60, 0x00, 0x06, 0x00, 0x18, 0x00, 0x60, 0x00, 0x00, 0x30, 0x18, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x60, 0x00, 0x06, 0x00, 0x0e, 0x00, 0xe0, 0x18,
  0x18, 0x70, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x3f,
  0xe7, 0x3c, 0x1e, 0x3c, 0x18, 0x24, 0xc3, 0x70, 0x7e, 0x1c, 0x00, 0x38,
  0x00, 0x7e, 0x30, 0x6e, 0x66, 0x26, 0x0c, 0x7e, 0x18, 0x3c, 0x66, 0x18,
  0x62, 0x18, 0x06, 0x18, 0x00, 0x00, 0x30, 0x66, 0x66, 0x06, 0x0c, 0x66,
  0x3c, 0x5a, 0x3c, 0x0c, 0x30, 0x18, 0x0c, 0x18, 0x00, 0x00, 0x3e, 0x66,
  0x7c, 0x1c, 0x0c, 0x66, 0x24, 0x5a, 0x18, 0x3e, 0x18, 0x18, 0x18, 0x18,
  0x00, 0x00, 0x33, 0x66, 0x66, 0x30, 0x0c, 0x66, 0x66, 0x42, 0x3c, 0x66,
  0x0c, 0x18, 0x30, 0x18, 0x66, 0x00, 0x33, 0x66, 0x66, 0x32, 0x2d, 0x66,
  0x66, 0x66, 0x66, 0x66, 0x46, 0x18, 0x60, 0x18, 0x3c, 0x00, 0x7e, 0x3c,
  0xfc, 0x1e, 0x3f, 0xe7, 0xe7, 0xe7, 0xc3, 0xe7, 0x7e, 0x1c, 0x40, 0x38,
  0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0xe7, 0x7e, 0x3c, 0x7c, 0x7f,
  0x78, 0x3f, 0xe7, 0x3c, 0x38, 0x73, 0x7f, 0x63, 0xe6, 0x3c, 0x60, 0x66,
  0x33, 0x76, 0x36, 0x31, 0x30, 0x73, 0x66, 0x18, 0x0c, 0x36, 0x31, 0x63,
  0x6e, 0x66, 0x6e, 0x66, 0x33, 0x60, 0x33, 0x30, 0x30, 0x67, 0x66, 0x18,
  0x0c, 0x3c, 0x30, 0x63, 0x6e, 0x66, 0x6e, 0x7e, 0x3e, 0x60, 0x33, 0x3c,
  0x3c, 0x60, 0x7e, 0x18, 0x0c, 0x38, 0x30, 0x6b, 0x7e, 0x66, 0x66, 0x66,
  0x33, 0x60, 0x33, 0x30, 0x30, 0x60, 0x66, 0x18, 0x0c, 0x3c, 0x30, 0x7f,
  0x76, 0x66, 0x66, 0x3c, 0x33, 0x76, 0x36, 0x31, 0x31, 0x73, 0x66, 0x18,
  0x0c, 0x36, 0x30, 0x77, 0x76, 0x66, 0x3c, 0x18, 0x7e, 0x3c, 0x7c, 0x7f,
  0x7f, 0x3e, 0xe7, 0x3c, 0x3f, 0x73, 0x70, 0x63, 0x67, 0x3c, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x3c, 0x3c, 0x7e, 0x3c, 0x06, 0x3c, 0x3c, 0x38, 0x3c, 0x38,
  0x18, 0x08, 0x06, 0x00, 0x60, 0x18, 0x66, 0x18, 0x62, 0x66, 0x7f, 0x06,
  0x66, 0x18, 0x66, 0x0c, 0x18, 0x18, 0x0c, 0x00, 0x30, 0x00, 0x76, 0x18,
  0x30, 0x06, 0x67, 0x06, 0x66, 0x18, 0x66, 0x06, 0x00, 0x00, 0x18, 0x3c,
  0x18, 0x18, 0x6e, 0x18, 0x18, 0x0c, 0x66, 0x3c, 0x66, 0x0c, 0x3c, 0x3e,
  0x00, 0x00, 0x30, 0x00, 0x0c, 0x1c, 0x66, 0x18, 0x0c, 0x06, 0x36, 0x30,
  0x7c, 0x06, 0x66, 0x66, 0x18, 0x18, 0x18, 0x3c, 0x18, 0x06, 0x66, 0x38,
  0x66, 0x66, 0x1e, 0x32, 0x30, 0x06, 0x66, 0x66, 0x18, 0x18, 0x0c, 0x00,
  0x30, 0x66, 0x3c, 0x18, 0x3c, 0x3c, 0x0e, 0x3e, 0x1e, 0x7e, 0x3c, 0x3c,
  0x00, 0x00, 0x06, 0x00, 0x60, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
  0x00, 0x00, 0x08, 0x00, 0x3d, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x20, 0x00,
  0x18, 0x00, 0x00, 0x00, 0x00, 0x24, 0x3e, 0x4c, 0x66, 0x00, 0x18, 0x18,
  0x24, 0x18, 0x10, 0x00, 0x18, 0x60, 0x00, 0x18, 0x00, 0x7e, 0x0b, 0x24,
  0x3e, 0x00, 0x30, 0x0c, 0x18, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x18,
  0x00, 0x24, 0x3e, 0x10, 0x19, 0x00, 0x30, 0x0c, 0x7e, 0x7e, 0x00, 0x7e,
  0x00, 0x18, 0x00, 0x18, 0x12, 0x24, 0x68, 0x48, 0x3c, 0x08, 0x30, 0x0c,
  0x18, 0x18, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x18, 0x36, 0x7e, 0x3e, 0x64,
  0x66, 0x18, 0x18, 0x18, 0x24, 0x18, 0x00, 0x00, 0x00, 0x06, 0x00, 0x18,
  0x36, 0x24, 0x08, 0x00, 0x3c, 0x18, 0x0c, 0x30, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xb3, 0x4c,
  0x00, 0x00, 0x10, 0x10, 0x02, 0x44, 0xff, 0xff, 0x18, 0x08, 0x22, 0x00,
  0x00, 0x00, 0xcd, 0x32, 0x22, 0x14, 0x54, 0x38, 0x26, 0x44, 0x00, 0x7e,
  0x00, 0x08, 0x36, 0x00, 0x00, 0x00, 0xff, 0x00, 0x2a, 0x54, 0x54, 0x54,
  0x60, 0x66, 0x7e, 0x3c, 0x18, 0x22, 0x1e, 0x00, 0x00, 0x00, 0xff, 0x00,
  0x48, 0x44, 0x92, 0x54, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x1c, 0x1c, 0x00,
  0x00, 0x00, 0xb3, 0x4c, 0x08, 0x02, 0x10, 0x10, 0x04, 0x88, 0x3c, 0x3c,
  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xcd, 0x32, 0x00, 0x00, 0x00, 0x00,
  0x0c, 0x88, 0x00, 0x18, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0xff, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x55, 0xcc, 0xaa, 0xff, 0xc3,
  0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x11, 0xaa,
  0xcc, 0xaa, 0x00, 0x81, 0xff, 0x55, 0x7f, 0x6e, 0x00, 0x18, 0x00, 0x00,
  0x18, 0x7e, 0x44, 0x55, 0x33, 0xaa, 0xff, 0x18, 0xfd, 0x77, 0x7f, 0x7c,
  0x00, 0x3c, 0x00, 0x18, 0x3c, 0x7e, 0x11, 0xaa, 0x33, 0xaa, 0x00, 0x3c,
  0xff, 0x77, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x18, 0x3c, 0x7e, 0x44, 0x55,
  0xcc, 0xaa, 0xff, 0x3c, 0x00, 0x00, 0xf7, 0x5e, 0x00, 0x7e, 0x00, 0x00,
  0x18, 0x7e, 0x11, 0xaa, 0xcc, 0xaa, 0x00, 0x18, 0xff, 0xaa, 0xf7, 0x7a,
  0x00, 0x3c, 0x00, 0x00, 0x00, 0x3c, 0x44, 0x55, 0x33, 0xaa, 0xff, 0x81,
  0xfd, 0xee, 0xf7, 0x7e, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x11, 0xaa,
  0x33, 0xaa, 0x00, 0xc3, 0xff, 0xee, 0x00, 0x00, 0x00, 0x00
};
unsigned int font_bmp_len = 2110;

// begin Font.hpp
struct Font
{
  SDL_Texture* texture;
  int charW, charH;
  int cols;
};



inline Font
loadDefaultFont(SDL_Renderer* renderer)
{
  SDL_RWops* src = SDL_RWFromConstMem(font_bmp, font_bmp_len);
  SDL_Surface* surface = SDL_LoadBMP_RW(src, 1);
  SDL_SetColorKey(surface, 1, 0);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return {texture, 8, 8, 16};
}

// begin TextStyle.hpp
// Text style
struct TextStyle
{
  Font font;
  SDL_Color color;
  int scale; // 0: 1x, 1: 2x, 2: 4x, 3: 8x, and so on

  constexpr TextStyle withFont(const Font& font) const
  {
    return {font, color, scale};
  }

  constexpr TextStyle withColor(SDL_Color color) const
  {
    return {font, color, scale};
  }

  constexpr TextStyle withScale(int scale) const
  {
    return {font, color, scale};
  }
};

struct Text;

namespace style {

/// Default text style
template<class Theme>
struct FromTheme<Text, Theme>
{
  constexpr static TextStyle get()
  {
    return {{nullptr, 8, 8, 16}, {45, 72, 106, 255}, 0};
  }
};
}

// begin ElementStyle.hpp
// Style for element state
struct ElementPaintStyle
{
  SDL_Color text;
  SDL_Color background;
  BorderColorStyle border;

  constexpr ElementPaintStyle withText(SDL_Color text) const
  {
    return {text, background, border};
  }
  constexpr ElementPaintStyle withBackground(SDL_Color background) const
  {
    return {text, background, border};
  }
  constexpr ElementPaintStyle withBorder(const BorderColorStyle& border) const
  {
    return {text, background, border};
  }

  constexpr operator BoxPaintStyle() const { return {background, border}; }
};

struct ElementStyle
{
  EdgeSize padding;
  EdgeSize border;
  Font font;
  int scale;
  ElementPaintStyle paint;

  constexpr ElementStyle withPadding(EdgeSize padding) const
  {
    return {padding, border, font, scale, paint};
  }
  constexpr ElementStyle withBorder(EdgeSize border) const
  {
    return {padding, border, font, scale, paint};
  }
  constexpr ElementStyle withPaint(const ElementPaintStyle& paint) const
  {
    return {padding, border, font, scale, paint};
  }
  constexpr ElementStyle withFont(const Font& font) const
  {
    return {padding, border, font, scale, paint};
  }
  constexpr ElementStyle withScale(int scale) const
  {
    return {padding, border, font, scale, paint};
  }

  constexpr ElementStyle withText(SDL_Color text) const
  {
    return withPaint(paint.withText(text));
  }
  constexpr ElementStyle withBackgroundColor(SDL_Color c) const
  {
    return withPaint(paint.withBackground(c));
  }
  constexpr ElementStyle withBorderColor(
    const BorderColorStyle& borderColor) const
  {
    return withPaint(paint.withBorder(borderColor));
  }
  constexpr operator BoxStyle() const { return {border, paint}; }
  constexpr operator TextStyle() const { return {font, paint.text, scale}; }
};

struct Element;

namespace style {

template<class Theme>
struct FromTheme<Element, Theme>
{
  constexpr static ElementStyle get()
  {
    auto text = themeFor<Text, Theme>();
    return {
      EdgeSize::all(2),
      EdgeSize::all(0),
      text.font,
      text.scale,
      {text.color},
    };
  }
};
} // namespace style

// begin ButtonStyle.hpp
// Style for button
struct ButtonStyle
{
  EdgeSize padding;
  EdgeSize border;
  Font font;
  int scale;
  ElementPaintStyle normal;
  ElementPaintStyle grabbed;
  ElementPaintStyle pressed;
  ElementPaintStyle pressedGrabbed;
};

struct ButtonBase;
struct Button;
struct ToggleButton;
struct ChoiceButton;

namespace style {

template<class Theme>
struct FromTheme<ButtonBase, Theme>
{
  constexpr static ButtonStyle get()
  {
    auto element = themeFor<Element, Theme>();
    ElementPaintStyle buttonBox = {
      element.paint.text,
      {176, 195, 222, 255},
      {
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {0, 0, 0, 255},
        {0, 0, 0, 255},
      },
    };
    ElementPaintStyle buttonBoxGrabbed =
      buttonBox.withBackground({147, 173, 210, 255});
    return {
      EdgeSize::all(3),
      EdgeSize::all(2),
      element.font,
      element.scale,
      buttonBox,
      buttonBoxGrabbed,
      buttonBox.withBorder(buttonBox.border.invert()),
      buttonBoxGrabbed.withBorder(buttonBox.border.invert()),
    };
  }
};

template<class Theme>
struct FromTheme<Button, Theme> : FromTheme<ButtonBase, Theme>
{};
template<class Theme>
struct FromTheme<ToggleButton, Theme> : FromTheme<ButtonBase, Theme>
{};
template<class Theme>
struct FromTheme<ChoiceButton, Theme> : FromTheme<ButtonBase, Theme>
{};
} // namespace style

// begin DisplayList.hpp
struct Shape
{
  SDL_Texture* texture;
  SDL_Rect rect;
  SDL_Rect srcRect;
  SDL_Color color;

  static Shape Box(const SDL_Rect& r, SDL_Color c)
  {
    return {nullptr, r, {0}, c};
  }
  static Shape Texture(const SDL_Rect& r, SDL_Texture* texture)
  {
    return {texture, r, {0}, {255, 255, 255, 255}};
  }
  static Shape Texture(const SDL_Rect& r, SDL_Texture* texture, SDL_Color c)
  {
    return {texture, r, {0}, c};
  }
  static Shape Texture(const SDL_Rect& r,
                       SDL_Texture* texture,
                       SDL_Rect& srcRect)
  {
    return {texture, r, srcRect, {255, 255, 255, 255}};
  }
  static Shape Texture(const SDL_Rect& r,
                       SDL_Texture* texture,
                       SDL_Rect& srcRect,
                       SDL_Color c)
  {
    return {texture, r, srcRect, c};
  }
};

/**
 * @brief Contains the list of elements to render
 *
 */
class DisplayList
{
  enum CommandType
  {
    POP_CLIP,
    PUSH_CLIP,
    SHAPE,
  };

  struct Command
  {
    union
    {
      Shape shape;
      SDL_Rect rect;
    };
    CommandType type;

    Command()
      : type(POP_CLIP)
    {}
    Command(const Shape& shape)
      : shape(shape)
      , type(SHAPE)
    {}
    Command(const SDL_Rect& rect)
      : rect(rect)
      , type(PUSH_CLIP)
    {}
  };
  std::vector<Command> items;

public:
  void clear() { items.clear(); }

  size_t size() { return items.size(); }

  void insert(const Shape& item)
  {
    if (item.color.a > 0) {
      items.push_back(item);
    }
  }

  void pushClip(const SDL_Rect& rect)
  {
    // TODO coalesce multiple clips
    if (rect.w > 0 && rect.h > 0) {
      items.push_back(rect);
    } else {
      items.push_back(SDL_Rect{rect.x, rect.y, 1, 1});
    }
  }

  void popClip() { items.push_back({}); }

  void render(SDL_Renderer* renderer) const;
};

inline void
DisplayList::render(SDL_Renderer* renderer) const
{
  // Save render state
  SDL_BlendMode blendMode;
  SDL_GetRenderDrawBlendMode(renderer, &blendMode);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  // Stack
  constexpr int STACK_MAX_SIZE = 32;
  SDL_Rect stack[STACK_MAX_SIZE]; // TODO make this configurable
  int stackSz = 0;
  for (auto it = items.rbegin(); it != items.rend(); it++) {
    if (it->type == POP_CLIP) {
      SDL_assert(stackSz > 0);
      --stackSz;
      SDL_RenderSetClipRect(renderer,
                            stackSz > 0 ? &stack[stackSz - 1] : nullptr);
      continue;
    }
    if (it->type == PUSH_CLIP) {
      SDL_assert(stackSz < STACK_MAX_SIZE);
      stack[stackSz++] = it->rect;
      SDL_RenderSetClipRect(renderer, &it->rect);
      continue;
    }
    auto& shape = it->shape;
    auto c = shape.color;
    if (shape.texture == nullptr) {
      SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
      SDL_RenderFillRect(renderer, &shape.rect);
    }
    SDL_SetTextureColorMod(shape.texture, c.r, c.g, c.b);
    if (shape.srcRect.w) {
      SDL_RenderCopy(renderer, shape.texture, &shape.srcRect, &shape.rect);
    } else {
      SDL_RenderCopy(renderer, shape.texture, nullptr, &shape.rect);
    }
  }
  SDL_SetRenderDrawBlendMode(renderer, blendMode);
  SDL_assert(stackSz == 0);
}

// begin State.hpp
constexpr char groupNameSeparator = '/';

/**
 * @brief The mouse action and status for a element in a frame
 *
 */
enum class MouseAction
{
  NONE,   ///< Default status
  GRAB,   ///< The mouse was just grabbed at this element
  HOLD,   ///< The mouse grabbed this element an is holding inside its bounds
  ACTION, ///< The mouse was just released inside its bounds (do something!)
  CANCEL, ///< The mouse was just released outside its bounds
  DRAG,   ///< The mouse had this grabbed, but was moved to outside its bounds
};

/**
 * @brief The text action and status for a element in a frame
 *
 */
enum class TextAction
{
  NONE,    ///< Default status
  INPUT,   ///< text input
  KEYDOWN, ///< erased last character
};

class State
{
  bool inFrame = false;
  SDL_Renderer* renderer;
  DisplayList dList;

  SDL_Point mPos;
  bool mLeftPressed = false;
  std::string eGrabbed;
  bool mHovering = false;
  bool mGrabbing = false;
  bool mReleasing = false;
  std::string eActive;
  char tBuffer[SDL_TEXTINPUTEVENT_TEXT_SIZE];
  SDL_Keysym tKeysym;
  bool tChanged = false;
  TextAction tAction = TextAction::NONE;

  std::string group;
  bool gGrabbed = false;
  bool gActive = false;

  Uint32 ticksCount;

  Font font;

public:
  State(SDL_Renderer* renderer)
    : renderer(renderer)
    , font(loadDefaultFont(renderer))
  {}

  /**
   * @brief Render the ui
   *
   * This must not be in frame. You might want to call Frame.render() that
   * ensures the frame ended correctly.
   *
   */
  void render()
  {
    SDL_assert(!inFrame);
    dList.render(renderer);
  }

  /**
   * @brief Handle a SDL_Event
   *
   * @param ev event
   */
  void event(SDL_Event& ev);

  /**
   * @brief If a frame is in progress
   *
   * You shouldn't send events nor render during this
   *
   * @return true
   * @return false
   */
  bool isInFrame() const { return inFrame; }

  /**
   * @brief Check if the element was activated
   *
   * The activation is done when you click (mouse button down) with the mouse on
   * the element and last until you click outside of it.
   *
   * Other ways of activate might exist.
   *
   * @param id the element id
   * @return true
   * @return false
   */
  bool isActive(std::string_view id) const
  {
    return isSameGroupId(eActive, id);
  }

  /**
   * @brief Check the mouse action/status for element in this frame
   *
   * @param id element id
   * @param r the element global rect (Use Group.checkMouse() for local rect)
   * @return MouseAction
   */
  MouseAction checkMouse(std::string_view id, SDL_Rect r);

  /**
   * @brief Check the text action/status for element in this frame
   *
   * @param id the element id
   * @return TextAction
   */
  TextAction checkText(std::string_view id) const
  {
    if (!tChanged || !isSameGroupId(eActive, id)) {
      return TextAction::NONE;
    }
    return tAction;
  }

  /**
   * @brief Get the last input text
   *
   * To check if the text was for the current element and frame, use checkText()
   * or Group.checkText().
   *
   * @return std::string_view
   */
  std::string_view lastText() const { return {tBuffer}; }

  /**
   * @brief Get the last key
   *
   * To check if the text was for the current element and frame, use checkText()
   * or Group.checkText().
   *
   * @return SDL_Keysym
   */
  SDL_Keysym lastKeyDown() const { return tKeysym; }

  /**
   * @brief If true, the state wants the mouse events
   */
  bool wantsMouse() const { return mHovering || !eGrabbed.empty(); }

  /**
   * @brief If true, the state wants the keyboard events
   * @return true
   * @return false
   */
  bool wantsKeyboard() const { return !eActive.empty(); }

  /**
   * @brief Add the given item Shape to display list
   *
   * @param item
   */
  void display(const Shape& item) { dList.insert(item); }

  /// Ticks count
  Uint32 ticks() const { return ticksCount; }

  // These are experimental and should not be used
  void beginGroup(std::string_view id, const SDL_Rect& r);
  void endGroup(std::string_view id, const SDL_Rect& r);
  const Font& getFont() const { return font; }
  void setFont(const Font& f) { font = f; }

private:
  void beginFrame()
  {
    SDL_assert(inFrame == false);
    inFrame = true;
    dList.clear();
    mHovering = false;
    ticksCount = SDL_GetTicks();
  }

  void endFrame()
  {
    SDL_assert(inFrame == true);
    inFrame = false;
    tChanged = false;
    mGrabbing = false;
    if (mReleasing) {
      eGrabbed.clear();
      mReleasing = false;
    }
  }

  bool isSameGroupId(std::string_view qualifiedId, std::string_view id) const;

  friend class Frame;
};

inline bool
State::isSameGroupId(std::string_view qualifiedId, std::string_view id) const
{
  auto groupSize = group.size();
  if (qualifiedId.size() < groupSize + 1) {
    return false;
  }
  if (qualifiedId.substr(0, groupSize) != group ||
      qualifiedId[groupSize] != groupNameSeparator ||
      qualifiedId.substr(groupSize + 1) != id) {
    return false;
  }
  return true;
}

inline MouseAction
State::checkMouse(std::string_view id, SDL_Rect r)
{
  SDL_assert(inFrame);
  if (eGrabbed.empty()) {
    if (!mLeftPressed) {
      return MouseAction::NONE;
    }
    if (SDL_PointInRect(&mPos, &r) && !mGrabbing) {
      eGrabbed = group;
      eGrabbed += groupNameSeparator;
      eGrabbed += id;
      eActive = group;
      eActive += groupNameSeparator;
      eActive += id;
      gGrabbed = true;
      gActive = true;
      mGrabbing = true;
      return MouseAction::GRAB;
    }
    if (isSameGroupId(eActive, id)) {
      eActive.clear();
    }
    return MouseAction::NONE;
  }
  if (!isSameGroupId(eGrabbed, id)) {
    return MouseAction::NONE;
  }
  gGrabbed = true;
  if (mLeftPressed) {
    if (mGrabbing) {
      return MouseAction::GRAB;
    }
    if (!SDL_PointInRect(&mPos, &r)) {
      return MouseAction::DRAG;
    }
    return MouseAction::HOLD;
  }
  mReleasing = true;
  if (!SDL_PointInRect(&mPos, &r)) {
    return MouseAction::CANCEL;
  }
  return MouseAction::ACTION;
}

inline void
State::beginGroup(std::string_view id, const SDL_Rect& r)
{
  dList.popClip();
  if (id.empty()) {
    return;
  }
  auto idSize = id.size();
  auto groupSize = group.size();
  if (groupSize > 0) {
    group += groupNameSeparator;
    if (!gGrabbed || eGrabbed.size() <= idSize + groupSize + 1 ||
        std::string_view{eGrabbed}.substr(groupSize, idSize + 1) != id ||
        eGrabbed[groupSize + idSize + 1] != groupNameSeparator) {
      gGrabbed = false;
    }
    if (!gActive || eActive.size() <= idSize + groupSize + 1 ||
        std::string_view{eActive}.substr(groupSize, idSize + 1) != id ||
        eActive[groupSize + idSize + 1] != groupNameSeparator) {
      gActive = false;
    }
  } else {
    if (eGrabbed.size() <= idSize ||
        std::string_view{eGrabbed}.substr(0, idSize) != id ||
        eGrabbed[idSize] != groupNameSeparator) {
      gGrabbed = false;
    } else {
      gGrabbed = true;
    }
    if (eActive.size() <= idSize ||
        std::string_view{eActive}.substr(0, idSize) != id ||
        eActive[idSize] != groupNameSeparator) {
      gActive = false;
    } else {
      gActive = true;
    }
  }
  group += id;
}

inline void
State::endGroup(std::string_view id, const SDL_Rect& r)
{
  if (id.empty()) {
    // Nothing to do
  } else if (id.size() >= group.size()) {
    // A top level group
    SDL_assert(group == id);
    group.clear();
    gActive = gGrabbed = false;
    if (!mHovering && SDL_PointInRect(&mPos, &r)) {
      mHovering = true;
    }
  } else {
    auto groupSize = group.size();
    auto nextSize = groupSize - id.size() - 1;
    SDL_assert(group[nextSize] == groupNameSeparator);
    SDL_assert(std::string_view{group}.substr(nextSize + 1) == id);

    group.erase(group.begin() + nextSize, group.end());
    if (!gGrabbed && eGrabbed.size() > nextSize &&
        std::string_view{eGrabbed}.substr(0, nextSize) == group &&
        eGrabbed[nextSize] == groupNameSeparator) {
      gGrabbed = true;
    }
    if (!gActive && eActive.size() > nextSize &&
        std::string_view{eActive}.substr(0, nextSize) == group &&
        eActive[nextSize] == groupNameSeparator) {
      gActive = true;
    }
  }
  dList.pushClip(r);
}

inline void
State::event(SDL_Event& ev)
{
  if (ev.type == SDL_MOUSEBUTTONDOWN) {
    mPos = {ev.button.x, ev.button.y};
    if (ev.button.button == SDL_BUTTON_LEFT) {
      mLeftPressed = true;
    }
  } else if (ev.type == SDL_MOUSEMOTION) {
    if (!(eGrabbed.empty() && mLeftPressed)) {
      mPos = {ev.motion.x, ev.motion.y};
    }
  } else if (ev.type == SDL_MOUSEBUTTONUP) {
    mPos = {ev.button.x, ev.button.y};
    mLeftPressed = false;
  } else if (ev.type == SDL_TEXTINPUT) {
    if (eActive.empty()) {
      return;
    }
    for (int i = 0, j = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; ++i) {
      tBuffer[j] = ev.text.text[i];
      if (tBuffer[j] == 0) {
        break;
      }
      // Magic handling of utf8
      if ((tBuffer[j] & 0xc0) == 0x80) {
        continue;
      }
      if ((tBuffer[j] & 0x80) != 0) {
        tBuffer[j] = '\x0f'; // This is valid on our particular font
      }
      ++j;
    }
    tChanged = true;
    tAction = TextAction::INPUT;
  } else if (ev.type == SDL_KEYDOWN) {
    if (!tChanged) {
      tKeysym = ev.key.keysym;
      tChanged = true;
      tAction = TextAction::KEYDOWN;
    }
  }
}

// begin Target.hpp
namespace style {
constexpr int elementSpacing = 2;
} // namespace style

enum class Layout : Uint8
{
  NONE,
  VERTICAL,
  HORIZONTAL,
};

constexpr int
makeLen(int len, int delta, bool autoPos)
{
  if (len == 0) {
    len = delta;
    if (autoPos && len >= style::elementSpacing) {
      len -= style::elementSpacing;
    }
  }
  return len;
}

constexpr int
makeWidth(const SDL_Rect& r,
          const SDL_Point& topLeft,
          const SDL_Point& bottomRight,
          Layout layout)
{
  return makeLen(r.w, bottomRight.x - topLeft.x, layout == Layout::HORIZONTAL);
}

constexpr int
makeHeight(const SDL_Rect& r,
           const SDL_Point& topLeft,
           const SDL_Point& bottomRight,
           Layout layout)
{
  return makeLen(r.h, bottomRight.y - topLeft.y, layout == Layout::VERTICAL);
}

/**
 * @brief
 *
 */
class Target
{
  State* state;
  std::string_view id;
  SDL_Rect& rect;
  SDL_Point& topLeft;
  SDL_Point& bottomRight;
  Layout layout;
  bool& locked;

public:
  Target(State* state,
         std::string_view id,
         SDL_Rect& rect,
         SDL_Point& topLeft,
         SDL_Point& bottomRight,
         Layout layout,
         bool& locked)
    : state(state)
    , id(id)
    , rect(rect)
    , topLeft(topLeft)
    , bottomRight(bottomRight)
    , layout(layout)
    , locked(locked)
  {}

  // Move ctor
  Target(Target&& rhs)
    : state(rhs.state)
    , id(rhs.id)
    , rect(rhs.rect)
    , topLeft(rhs.topLeft)
    , bottomRight(rhs.bottomRight)
    , layout(rhs.layout)
    , locked(rhs.locked)
  {
    rhs.state = nullptr;
  }
  Target(const Target&) = delete;

  // Not a Copy ctor, a borrow actually
  Target(Target& rhs)
    : Target(rhs.state,
             rhs.id,
             rhs.rect,
             rhs.topLeft,
             rhs.bottomRight,
             rhs.layout,
             rhs.locked)
  {}

  /**
   * @brief Check the mouse action/status for element in this group
   *
   * @param id element id
   * @param r the element local rect (Use State.checkMouse() for global rect)
   * @return MouseAction
   */
  MouseAction checkMouse(std::string_view id, SDL_Rect r);

  /**
   * @brief Check if given contained element is active
   *
   * @param id the id to check
   * @return true
   * @return false
   */
  bool isActive(std::string_view id) const { return state->isActive(id); }

  /**
   * @brief Check the text action/status for element in this group
   *
   * @param id the element id
   * @return TextAction
   */
  TextAction checkText(std::string_view id) const
  {
    return state->checkText(id);
  }

  /**
   * @brief Get the last input text
   *
   * To check if the text was for the current element and frame, use checkText()
   * or Frame.checkText().
   *
   * @return std::string_view
   */
  std::string_view lastText() const { return state->lastText(); }
  /**
   * @brief Get the last key down
   *
   * To check if the text was for the current element and frame, use checkText()
   * or Frame.checkText().
   *
   * @return std::string_view
   */
  SDL_Keysym lastKeyDown() const { return state->lastKeyDown(); }

  /**
   * @brief Advances the caret with the given offset
   *
   * @param p a point where x and y are the horizontal and vertical offsets,
   * respectively.
   *
   * The new caret will also depend on the layout:
   * - VERTICAL layout means only the y is updated so elements are positioned
   * vertically;
   * - HORIZONTAL layout means only the x is updated so elements are positioned
   * horizontally;
   * - NONE layout means none are updated and elements all begin in the same
   * position;
   *
   * Keep in mind that elements have their own offset (their rect.x and rect.y),
   * that is added to the caret ones.
   */
  void advance(const SDL_Point& p);

  State& getState() const { return *state; }

  SDL_Point getCaret() const
  {
    auto caret = topLeft;
    if (layout == Layout::VERTICAL) {
      caret.y = bottomRight.y;
    } else if (layout == Layout::HORIZONTAL) {
      caret.x = bottomRight.x;
    }
    return caret;
  }

  bool isLocked() const { return locked; }

  int width() const { return makeWidth(rect, topLeft, bottomRight, layout); }

  int height() const { return makeHeight(rect, topLeft, bottomRight, layout); }

  void lock(std::string_view id, SDL_Rect r)
  {
    SDL_assert(!locked);
    locked = true;
    auto caret = getCaret();
    r.x += caret.x;
    r.y += caret.y;
    state->beginGroup(id, r);
  }

  void unlock(std::string_view id, SDL_Rect r)
  {
    SDL_assert(locked);
    auto caret = getCaret();
    r.x += caret.x;
    r.y += caret.y;
    state->endGroup(id, r);
    locked = false;
  }
};

inline MouseAction
Target::checkMouse(std::string_view id, SDL_Rect r)
{
  SDL_assert(!locked);
  SDL_Point caret = getCaret();
  r.x += caret.x;
  r.y += caret.y;
  return state->checkMouse(id, r);
}

inline void
Target::advance(const SDL_Point& p)
{
  SDL_assert(!locked);
  if (layout == Layout::VERTICAL) {
    bottomRight.x = std::max(p.x + topLeft.x, bottomRight.x);
    bottomRight.y += p.y + 2;
  } else if (layout == Layout::HORIZONTAL) {
    bottomRight.x += p.x + 2;
    bottomRight.y = std::max(p.y + topLeft.y, bottomRight.y);
  } else {
    bottomRight.x = std::max(p.x + topLeft.x, bottomRight.x);
    bottomRight.y = std::max(p.y + topLeft.y, bottomRight.y);
  }
}

// begin Group.hpp
constexpr SDL_Point
makeCaret(const SDL_Point& caret, int x, int y)
{
  return {caret.x + x, caret.y + y};
}

/**
 * @brief A grouping of widgets
 *
 * This externally viewed as a single widget, so it can be used to create
 * composed elements
 *
 */
class Group
{
  Target parent;
  std::string_view id;
  bool locked = false;
  bool ended = false;
  SDL_Rect rect;
  SDL_Point topLeft;
  SDL_Point bottomRight;
  Layout layout;

public:
  /**
   * @brief Construct a new branch Group object
   *
   * You probably want to use group() instead of this.
   *
   * @param parent the parent group. MUST NOT BE NULL
   * @param id the group id
   * @param rect the rect. Either w or h being 0 means it will auto size
   * @param layout the layout
   */
  Group(Target parent,
        std::string_view id,
        const SDL_Rect& rect,
        Layout layout);

  ~Group() { SDL_assert(ended); }
  Group(const Group&) = delete;
  Group(Group&& rhs);
  Group& operator=(Group&& rhs) = default;
  Group& operator=(const Group& rhs) = delete;

  operator bool() const { return !ended; }

  int width() const { return makeWidth(rect, topLeft, bottomRight, layout); }

  void width(int v) { rect.w = v; }

  int height() const { return makeHeight(rect, topLeft, bottomRight, layout); }

  void height(int v) { rect.h = v; }

  operator Target() &
  {
    return {&parent.getState(), id, rect, topLeft, bottomRight, layout, locked};
  }

  void end();
};

/**
 * @brief Create group
 *
 * @param target the parent group or frame
 * @param id the group id
 * @param rect the group dimensions
 * @return Group
 */
inline Group
group(Target target,
      std::string_view id,
      const SDL_Rect& rect,
      Layout layout = Layout::VERTICAL)
{
  return {target, id, rect, layout};
}

inline Group::Group(Target parent,
                    std::string_view id,
                    const SDL_Rect& rect,
                    Layout layout)
  : parent(parent)
  , id(id)
  , rect(rect)
  , topLeft(makeCaret(parent.getCaret(), rect.x, rect.y))
  , bottomRight(topLeft)
  , layout(layout)
{
  parent.lock(id, rect);
}

inline void
Group::end()
{
  if (rect.w == 0) {
    rect.w = width();
  }
  if (rect.h == 0) {
    rect.h = height();
  }
  parent.unlock(id, rect);
  parent.advance({rect.x + rect.w, rect.y + rect.h});
  ended = true;
}

inline Group::Group(Group&& rhs)
  : parent(rhs.parent)
  , id(std::move(rhs.id))
  , locked(rhs.locked)
  , rect(rhs.rect)
  , topLeft(rhs.topLeft)
  , bottomRight(rhs.bottomRight)
  , layout(rhs.layout)
{
  rhs.ended = true;
}

// begin Box.hpp
/**
 * @brief adds a colorBox element to target
 *
 * @param target the parent group or frame
 * @param rect the colorBox local position and size
 * @param c the colorBox color
 */
inline void
colorBox(Target target, SDL_Rect rect, SDL_Color c)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto caret = target.getCaret();
  target.advance({rect.x + rect.w, rect.y + rect.h});
  rect.x += caret.x;
  rect.y += caret.y;
  state.display(Shape::Box(rect, c));
}

/**
 * @brief adds an texturedBox element to target
 *
 * @param target the parent group or frame
 * @param texture the texture
 * @param rect the box local position and size
 */
inline void
textureBox(Target target, SDL_Texture* texture, SDL_Rect rect)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto caret = target.getCaret();
  target.advance({rect.x + rect.w, rect.y + rect.h});
  rect.x += caret.x;
  rect.y += caret.y;
  state.display(Shape::Texture(rect, texture));
}

// A box with colored border
inline void
box(Target target, const SDL_Rect& r, const BoxStyle& style = themeFor<Box>())
{
  auto c = style.paint.background;
  auto e = style.paint.border.right;
  auto n = style.paint.border.top;
  auto w = style.paint.border.left;
  auto s = style.paint.border.bottom;
  auto esz = style.border.right;
  auto nsz = style.border.top;
  auto wsz = style.border.left;
  auto ssz = style.border.bottom;
  auto g = group(target, {}, {0}, Layout::NONE);
  colorBox(g, {r.x + 1, r.y, r.w - 2, nsz}, {n.r, n.g, n.b, n.a});
  colorBox(g, {r.x, r.y + 1, wsz, r.h - 2}, {w.r, w.g, w.b, w.a});
  colorBox(g, {r.x + 1, r.y + r.h - ssz, r.w - 2, ssz}, {s.r, s.g, s.b, s.a});
  colorBox(g, {r.x + r.w - esz, r.y + 1, esz, r.h - 2}, {e.r, e.g, e.b, e.a});
  colorBox(g,
           {r.x + esz, r.y + nsz, r.w - esz - wsz, r.h - nsz - ssz},
           {c.r, c.g, c.b, c.a});
  g.end();
}

// begin Text.hpp
/// Measure the given character
constexpr SDL_Point
measure(char ch, const Font& font, int scale)
{
  return {font.charW << scale, font.charH << scale};
}

/// Measure the given text
constexpr SDL_Point
measure(std::string_view text, const Font& font, int scale)
{
  return {int((font.charW << scale) * text.size()), font.charH << scale};
}

/**
 * @brief Adds a character element
 *
 * @param target the parent group or frame
 * @param ch the character
 * @param p the position
 * @param c the color (style::TEXT by default)
 */
inline void
character(Target target,
          char ch,
          const SDL_Point& p,
          const TextStyle& style = themeFor<Text>())
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto& font = state.getFont();
  SDL_assert(font.texture != nullptr);

  auto caret = target.getCaret();
  SDL_Rect dstRect{p.x + caret.x,
                   p.y + caret.y,
                   font.charW << style.scale,
                   font.charH << style.scale};
  target.advance({p.x + dstRect.w, p.y + dstRect.h});
  SDL_Rect srcRect{(ch % font.cols) * font.charW,
                   (ch / font.cols) * font.charH,
                   font.charW,
                   font.charH};
  state.display(Shape::Texture(dstRect, font.texture, srcRect, style.color));
}

/**
 * @brief Adds a text element
 *
 * @param target the parent group or frame
 * @param str the text
 * @param p the position
 * @param c the color (style::TEXT by default)
 */
inline void
text(Target target,
     std::string_view str,
     const SDL_Point& p,
     const TextStyle& style = themeFor<Text>())
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto font = style.font.texture ? style.font : state.getFont();
  SDL_assert(font.texture != nullptr);

  auto caret = target.getCaret();
  SDL_Rect dstRect{p.x + caret.x,
                   p.y + caret.y,
                   font.charW << style.scale,
                   font.charH << style.scale};
  target.advance({p.x + dstRect.w * int(str.size()), p.y + dstRect.h});
  for (auto ch : str) {
    SDL_Rect srcRect{(ch % font.cols) * font.charW,
                     (ch / font.cols) * font.charH,
                     font.charW,
                     font.charH};
    state.display(Shape::Texture(dstRect, font.texture, srcRect, style.color));
    dstRect.x += dstRect.w;
  }
}

// begin Element.hpp
inline SDL_Point
computeSize(std::string_view str,
            const ElementStyle& style,
            const SDL_Point& sz)
{
  if (sz.x != 0 && sz.y != 0) {
    return sz;
  }
  auto clientSz = measure(str, style.font, style.scale);
  auto elementSz = elementSize(style.padding + style.border, clientSz);
  if (sz.x != 0) {
    elementSz.x = sz.x;
  } else if (sz.y != 0) {
    elementSz.y = sz.y;
  }
  return elementSz;
}

inline void
element(Target target,
        std::string_view str,
        const SDL_Rect& r = {0},
        const ElementStyle& style = themeFor<Element>())
{
  auto offset = style.border + style.padding;
  auto sz = computeSize(str, style, {r.w, r.h});
  auto g = group(target, {}, {r.x, r.y, sz.x, sz.y}, Layout::NONE);
  text(g, str, {offset.left, offset.top}, style);
  box(g, {0, 0, sz.x, sz.y}, style);
  g.end();
}

// begin PanelStyle.hpp
// Style for panels
struct PanelStyle
{
  EdgeSize padding;
  EdgeSize border;
  BoxPaintStyle paint;

  constexpr PanelStyle withPadding(const EdgeSize& padding) const
  {
    return {padding, border, paint};
  }
  constexpr PanelStyle withBorderSize(const EdgeSize& border) const
  {
    return {padding, border, paint};
  }
  constexpr PanelStyle withPaint(const BoxPaintStyle& paint) const
  {
    return {padding, border, paint};
  }
  constexpr PanelStyle withBackgroundColor(SDL_Color background) const
  {
    return withPaint(paint.withBackground(background));
  }
  constexpr PanelStyle withBorderColor(const BorderColorStyle& border) const
  {
    return withPaint(paint.withBorder(border));
  }
  constexpr operator BoxStyle() const { return {border, paint}; }
};

struct Panel;

namespace style {
/// Default panel style
template<class Theme>
struct FromTheme<Panel, Theme>
{
  constexpr static PanelStyle get()
  {
    auto boxStyle = themeFor<Box, Theme>();
    return {
      EdgeSize::all(2),
      boxStyle.border,
      boxStyle.paint,
    };
  }
};
}

// begin WrapperGroup.hpp
/// A class to make wrapper elements
class WrapperGroup
{
  Group wrapper;
  EdgeSize padding;
  Group client;
  bool onClient = false;
  bool ended = false;
  bool autoW;
  bool autoH;

  static constexpr SDL_Rect paddedSize(const SDL_Rect& rect,
                                       const EdgeSize& padding)
  {
    int pw = padding.left + padding.right;
    int dw = rect.w > pw ? pw : 0;
    int ph = padding.top + padding.bottom;
    int dh = rect.h > ph ? ph : 0;
    return {padding.left, padding.top, rect.w - dw, rect.h - dh};
  }

public:
  WrapperGroup(Target parent,
               std::string_view id,
               const SDL_Rect& rect,
               Layout layout,
               const EdgeSize& padding)
    : wrapper(parent, id, rect, Layout::NONE)
    , padding(padding)
    , client(wrapper, {}, paddedSize(rect, padding), layout)
    , autoW(rect.w == 0)
    , autoH(rect.h == 0)
  {
    onClient = true;
  }

  operator Target() & { return onClient ? Target{client} : Target{wrapper}; }
  operator bool() const { return !ended; }

  SDL_Point endClient();

  void endWrapper();
};

inline SDL_Point
WrapperGroup::endClient()
{
  SDL_assert(onClient);
  if (autoW) {
    wrapper.width(client.width() + padding.left + padding.right);
  }
  if (autoH) {
    wrapper.height(client.height() + padding.top + padding.bottom);
  }
  client.end();
  onClient = false;
  return {wrapper.width(), wrapper.height()};
}

inline void
WrapperGroup::endWrapper()
{
  SDL_assert(!ended && !onClient);
  wrapper.end();
  ended = true;
}

// begin Panel.hpp
/// A panel class @see panel()
class Panel
{
  PanelStyle style;
  WrapperGroup wrapper;

public:
  Panel(Target parent,
        std::string_view id,
        const SDL_Rect& r,
        Layout layout,
        const PanelStyle& style)
    : style(style)
    , wrapper(parent, id, r, layout, style.padding + style.border)
  {}

  void end()
  {
    auto sz = wrapper.endClient();
    box(*this, {0, 0, sz.x, sz.y}, style);
    wrapper.endWrapper();
  }

  operator Target() & { return wrapper; }

  operator bool() const { return wrapper; }
};

/**
 * @brief adds a panel element
 *
 * @param target the parent group or frame
 * @param id the panel id
 * @param r the panel relative postion and size
 * @param layout the layout
 * @param style the style
 * @return Panel
 */
inline Panel
panel(Target target,
      std::string_view id,
      const SDL_Rect& r = {0},
      Layout layout = Layout::VERTICAL,
      const PanelStyle& style = themeFor<Panel>())
{
  return {target, id, r, layout, style};
}

// begin Button.hpp
constexpr const ElementPaintStyle&
decideButtonColors(const ButtonStyle& style, bool pushed, bool grabbing)
{
  if (grabbing == pushed) {
    return (grabbing) ? style.grabbed : style.normal;
  } else {
    return (grabbing) ? style.pressedGrabbed : style.pressed;
  }
}

/**
 * @brief Common button behavior
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param pushed if the button is pushed or not
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state (just released)
 * @return false otherwise
 */
inline bool
buttonBase(Target target,
           std::string_view id,
           std::string_view str,
           bool pushed,
           const SDL_Point& p = {0},
           const ButtonStyle& style = themeFor<ButtonBase>())
{
  if (str.empty()) {
    str = id;
  }
  auto adv = elementSize(style.padding + style.border,
                         measure(str, style.font, style.scale));
  SDL_Rect r{p.x, p.y, adv.x, adv.y};
  auto action = target.checkMouse(id, r);

  element(target,
          str,
          r,
          {style.padding,
           style.border,
           style.font,
           style.scale,
           decideButtonColors(style, pushed, action == MouseAction::HOLD)});
  return action == MouseAction::ACTION;
}

/**
 * @{
 * @brief A push button
 *
 * It can be used to make an action every time it is pushed
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state (just released)
 * @return false otherwise
 */
inline bool
button(Target target,
       std::string_view id,
       std::string_view str,
       const SDL_Point& p = {0},
       const ButtonStyle& style = themeFor<Button>())
{
  return buttonBase(target, id, str, false, p, style);
}
inline bool
button(Target target,
       std::string_view id,
       const SDL_Point& p = {0},
       const ButtonStyle& style = themeFor<Button>())
{
  return button(target, id, id, p, style);
}
/// @}

/**
 * @{
 * @brief A button that toggle a boolean variable
 *
 * Every time it is pushed the value is negated. The button reflects this state
 * by being pushed or pulled if the variable is true of false, respectively.
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param value a pointer to a boolean with the state
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state
 * @return false otherwise
 */
inline bool
toggleButton(Target target,
             std::string_view id,
             std::string_view str,
             bool* value,
             const SDL_Point& p = {0},
             const ButtonStyle& style = themeFor<ToggleButton>())
{
  if (buttonBase(target, id, str, *value, p, style)) {
    *value = !*value;
    return true;
  }
  return false;
}
inline bool
toggleButton(Target target,
             std::string_view id,
             bool* value,
             const SDL_Point& p = {0},
             const ButtonStyle& style = themeFor<ToggleButton>())
{
  return toggleButton(target, id, id, value, p, style);
}
/// @}

/**
 * @{
 * @brief A button part of multiple choice question
 *
 * If this button is actionned the value is changed to the given option
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param value a pointer to the control variable
 * @param option the option this button represents if value is equivalent to
 * this, then the button will appear pressed. If the user pushed, then the value
 * is set to this option.
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state
 * @return false otherwise
 */
template<class T, class U>
inline bool
choiceButton(Target target,
             std::string_view id,
             std::string_view str,
             T* value,
             U option,
             const SDL_Point& p = {0},
             const ButtonStyle& style = themeFor<ChoiceButton>())
{
  bool selected = *value == option;
  if (buttonBase(target, id, str, selected, p, style) && !selected) {
    *value = option;
    return true;
  }
  return false;
}
template<class T, class U>
inline bool
choiceButton(Target target,
             std::string_view id,
             T* value,
             U option,
             const SDL_Point& p = {0},
             const ButtonStyle& style = themeFor<ChoiceButton>())
{
  return choiceButton(target, id, id, value, option, p, style);
}
/// @}

// begin Frame.hpp
/**
 * @brief Represents a single frame on the app
 *
 */
class Frame
{
  State* state;
  SDL_Rect rect;
  SDL_Point topLeft;
  SDL_Point bottomRight;
  bool locked = false;

public:
  Frame(State* state = nullptr);
  Frame(const Frame&) = delete;
  Frame(Frame&& rhs) = default;
  Frame& operator=(const Frame&) = delete;
  Frame& operator=(Frame&& rhs) = default;

  void render()
  {
    SDL_assert(state != nullptr);
    auto& state = *this->state;
    end();
    state.render();
  }

  // It is called by render normally
  void end();

  operator Target() &
  {
    return {state, {}, rect, topLeft, bottomRight, Layout::NONE, locked};
  }
};

inline Frame
frame(State& state)
{
  return {&state};
}

inline Frame::Frame(State* state)
  : state(state)
  , rect({0, 0, 0, 0})
  , topLeft({0, 0})
  , bottomRight({0, 0})
{
  state->beginFrame();
}

inline void
Frame::end()
{
  state->endFrame();
  locked = false;
}

// begin InputBoxStyle.hpp
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

// begin InputBox.hpp
inline SDL_Point
makeInputSize(SDL_Point defaultSz,
              const Font& font,
              int scale,
              const EdgeSize& padding)
{
  if (defaultSz.x != 0 && defaultSz.y != 0) {
    return defaultSz;
  }
  // TODO allow customization for this
  auto clientSz = measure('m', font, scale);
  clientSz.x *= 16;

  auto elementSz = elementSize(padding, clientSz);

  if (defaultSz.x == 0) {
    defaultSz.x = elementSz.x;
  }
  if (defaultSz.y == 0) {
    defaultSz.y = elementSz.y;
  }
  return defaultSz;
}

inline SDL_Rect
makeInputRect(SDL_Rect r, const InputBoxStyle& style)
{
  auto sz = makeInputSize(
    {r.w, r.h}, style.font, style.scale, style.padding + style.border);
  return {r.x, r.y, sz.x, sz.y};
}

struct TextChange
{
  std::string_view insert;
  size_t index;
  size_t erase;
};

inline TextChange
textBoxBase(Target target,
            std::string_view id,
            std::string_view value,
            SDL_Rect r,
            const InputBoxStyle& style = themeFor<InputBoxBase>())
{
  static size_t cursorPos = 0;
  static size_t maxPos = 0;
  r = makeInputRect(r, style);
  if (target.checkMouse(id, r) == MouseAction::GRAB) {
    maxPos = cursorPos = value.size();
  }

  auto action = target.checkText(id);
  bool active = action == TextAction::NONE ? target.isActive(id) : true;
  if (active && cursorPos > value.size()) {
    maxPos = cursorPos = value.size();
  }
  auto& currentColors = active ? style.active : style.normal;
  auto g = panel(
    target, id, r, Layout::NONE, {style.padding, style.border, currentColors});

  // This creates an auto scroll effect if value text don't fit in the box;
  auto clientSz = clientSize(style.padding + EdgeSize::all(1), {r.w, r.h});
  auto contentSz = measure(value, style.font, style.scale);
  int deltaX = contentSz.x - clientSz.x;
  if (deltaX < 0) {
    deltaX = 0;
  } else if (active && deltaX + 8 > int(cursorPos) * 8) {
    // TODO Use proper scrolling here
    deltaX = cursorPos * 8;
    if (deltaX > 8) {
      deltaX -= 8;
    } else {
      deltaX = 0;
    }
  }
  text(g, value, {-deltaX, 0}, {style.font, currentColors.text, style.scale});

  if (active && (target.getState().ticks() / 512) % 2) {
    // Show cursor
    colorBox(
      g, {int(cursorPos) * 8 - deltaX, 0, 1, clientSz.y}, currentColors.text);
  }
  g.end();
  if (action == TextAction::INPUT) {
    auto insert = target.lastText();
    auto index = cursorPos;
    cursorPos += insert.size();
    maxPos += insert.size();
    return {insert, index, 0};
  }
  if (action == TextAction::KEYDOWN) {
    SDL_Keysym keysym = target.lastKeyDown();
    switch (keysym.sym) {
      case SDLK_BACKSPACE:
        if (!value.empty()) {
          cursorPos -= 1;
          maxPos -= 1;
          return {{}, cursorPos, 1};
        }
        break;
      case SDLK_LEFT:
        if (cursorPos > 0) {
          cursorPos -= 1;
        }
        break;
      case SDLK_RIGHT:
        if (cursorPos < maxPos) {
          cursorPos += 1;
        }
        break;
      default:
        break;
    }
  }
  return {};
}

inline bool
textBox(Target target,
        std::string_view id,
        char* value,
        size_t maxSize,
        const SDL_Rect& r = {0},
        const InputBoxStyle& style = themeFor<TextBox>())
{
  auto len = strlen(value);
  auto change = textBoxBase(target, id, {value, len}, r, style);
  if (change.erase == 0 && change.insert.empty()) {
    return false;
  }
  int offset = int(change.insert.size()) - int(change.erase);
  if (offset != 0) {
    size_t target = change.index + change.insert.size();
    int maxCount = maxSize - target;
    if (maxCount > 0) {
      size_t source = target - offset;
      SDL_memmove(&value[target],
                  &value[source],
                  std::min(len - source + 1, size_t(maxCount)));
    }
  }
  if (!change.insert.empty()) {
    int maxCount = maxSize - 1 - change.index;
    if (maxCount > 0) {
      SDL_memcpy(&value[change.index],
                 change.insert.data(),
                 std::min(change.insert.size(), size_t(maxCount)));
    }
  }
  return true;
}

inline bool
textBox(Target target,
        std::string_view id,
        std::string* value,
        const SDL_Rect& r = {0},
        const InputBoxStyle& style = themeFor<TextBox>())
{
  auto change = textBoxBase(target, id, *value, r, style);
  if (change.erase == 0 && change.insert.empty()) {
    return false;
  }
  value->replace(change.index, change.erase, change.insert);
  return true;
}

// TODO delete copy ctor, other safety nets
class BufferedInputBox
{
  Target target;
  std::string_view id;
  SDL_Rect rect;
  const InputBoxStyle& style;

  bool active;
  bool refillBuffer;

public:
  int incAmount = 0;
  static constexpr int BUF_SZ = 256;
  char buffer[BUF_SZ];

  BufferedInputBox(Target target,
                   std::string_view id,
                   SDL_Rect r,
                   const InputBoxStyle& style)
    : target(target)
    , id(id)
    , rect(makeInputRect(r, style))
    , style(style)
  {
    bool clicked = target.checkMouse(id, rect) == MouseAction::GRAB;
    active = target.isActive(id);
    refillBuffer = !active || clicked;
    if (active && target.checkText(id) == TextAction::KEYDOWN) {
      auto keysym = target.lastKeyDown();
      if (keysym.sym == SDLK_UP) {
        incAmount = 1;
        refillBuffer = true;
      } else if (keysym.sym == SDLK_DOWN) {
        incAmount = -1;
        refillBuffer = true;
      }
    }
  }

  bool wantsRefill() const { return refillBuffer; }

  bool end()
  {
    if (!active) {
      textBox(target, id, buffer, BUF_SZ, rect, style);
      return false;
    }
    static char editBuffer[BUF_SZ];
    if (refillBuffer) {
      SDL_strlcpy(editBuffer, buffer, BUF_SZ);
    }
    if (!textBox(target, id, editBuffer, BUF_SZ, rect, style)) {
      return incAmount != 0;
    }
    SDL_strlcpy(buffer, editBuffer, BUF_SZ);
    return true;
  }
};

// An int Box
inline bool
numberBox(Target target,
          std::string_view id,
          int* value,
          SDL_Rect r = {0},
          const InputBoxStyle& style = themeFor<IntBox>())
{
  SDL_assert(value != nullptr);
  BufferedInputBox bufferedBox{target, id, r, style};
  if (bufferedBox.wantsRefill()) {
    if (bufferedBox.incAmount != 0) {
      *value += bufferedBox.incAmount;
    }
    SDL_itoa(*value, bufferedBox.buffer, 10);
  }
  if (bufferedBox.end()) {
    auto newValue = SDL_atoi(bufferedBox.buffer);
    if (newValue != *value) {
      *value = newValue;
      return true;
    }
  }
  return false;
}

// A double box
inline bool
numberBox(Target target,
          std::string_view id,
          double* value,
          SDL_Rect r = {0},
          const InputBoxStyle& style = themeFor<DoubleBox>())
{
  SDL_assert(value != nullptr);
  BufferedInputBox bufferedBox{target, id, r, style};
  if (bufferedBox.wantsRefill()) {
    if (bufferedBox.incAmount != 0) {
      *value += bufferedBox.incAmount;
    }
    auto text = bufferedBox.buffer;
    int n = SDL_snprintf(text, bufferedBox.BUF_SZ, "%f", *value);
    for (int i = n - 1; i > 0; --i) {
      if (text[i] != '0' && text[i] != '.') {
        text[i + 1] = 0;
        break;
      }
    }
  }
  if (bufferedBox.end()) {
    auto newValue = SDL_strtod(bufferedBox.buffer, nullptr);
    if (newValue != *value) {
      *value = newValue;
      return true;
    }
  }
  return false;
}

// begin LabelStyle.hpp
struct Label;

namespace style {
template<class Theme>
struct FromTheme<Label, Theme> : FromTheme<Element, Theme>
{};
} // namespace style

// begin InputFieldStyle.hpp
struct InputFieldStyle
{
  InputBoxStyle box;
  ElementStyle label;
};

struct TextField;
struct IntField;
struct DoubleField;

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
} // namespace style

// begin Label.hpp
/**
 * @brief
 *
 * @param target the parent group or frame
 * @param str the text to show
 * @param p the local relative postion to add the label. Its size is
 * automatically evaluated
 * @param style
 */
inline void
label(Target target,
      std::string_view str,
      const SDL_Point& p = {0},
      const ElementStyle& style = themeFor<Label>())
{
  element(target, str, {p.x, p.y, 0, 0}, style);
}

// begin InputField.hpp
inline Group
labeledGroup(Target target,
             std::string_view labelText,
             const SDL_Rect& clientRect,
             const ElementStyle& style = themeFor<Label>())
{
  SDL_Rect r{clientRect};
  SDL_Point labelPos = {r.w + 1, 0};
  r.w += measure(labelText, style.font, style.scale).x + 1;

  auto g = group(target, {}, r, Layout::NONE);
  label(g, labelText, labelPos, style);
  return g;
}

inline bool
textField(Target target,
          std::string_view id,
          std::string_view labelText,
          char* value,
          size_t maxSize,
          const SDL_Point& p = {0},
          const InputFieldStyle& style = themeFor<TextField>())
{
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, themeFor<TextBox>())};
  auto g = labeledGroup(target, labelText, box, style.label);
  auto changed = textBox(g, id, value, maxSize, box, style.box);
  g.end();
  return changed;
}

inline bool
textField(Target target,
          std::string_view id,
          char* value,
          size_t maxSize,
          const SDL_Point& p = {0},
          const InputFieldStyle& style = themeFor<TextField>())
{
  return textField(target, id, id, value, maxSize, p, style);
}

inline bool
textField(Target target,
          std::string_view id,
          std::string_view labelText,
          std::string* value,
          const SDL_Point& p = {0},
          const InputFieldStyle& style = themeFor<TextField>())
{
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, themeFor<TextBox>())};
  auto g = labeledGroup(target, labelText, box, style.label);
  auto changed = textBox(g, id, value, box, style.box);
  g.end();
  return changed;
}

inline bool
textField(Target target,
          std::string_view id,
          std::string* value,
          const SDL_Point& p = {0},
          const InputFieldStyle& style = themeFor<TextField>())
{
  return textField(target, id, id, value, p, style);
}

inline bool
numberField(Target target,
            std::string_view id,
            std::string_view labelText,
            int* value,
            const SDL_Point& p = {0},
            const InputFieldStyle& style = themeFor<IntField>())
{
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, themeFor<IntBox>())};
  auto g = labeledGroup(target, labelText, box, style.label);
  auto changed = numberBox(g, id, value, box, style.box);
  g.end();
  return changed;
}

inline bool
numberField(Target target,
            std::string_view id,
            int* value,
            const SDL_Point& p = {0},
            const InputFieldStyle& style = themeFor<IntField>())
{
  return numberField(target, id, id, value, p, style);
}

inline bool
numberField(Target target,
            std::string_view id,
            std::string_view labelText,
            double* value,
            const SDL_Point& p = {0},
            const InputFieldStyle& style = themeFor<DoubleField>())
{
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, themeFor<DoubleBox>())};
  auto g = labeledGroup(target, labelText, box, style.label);
  auto changed = numberBox(g, id, value, box, style.box);
  g.end();
  return changed;
}

inline bool
numberField(Target target,
            std::string_view id,
            double* value,
            const SDL_Point& p = {0},
            const InputFieldStyle& style = themeFor<DoubleField>())
{
  return numberField(target, id, id, value, p, style);
}

// begin ScrollBarStyle.hpp
// Style for button
struct ScrollBarSliderStyle
{
  PanelStyle panel;
  BoxStyle cursor;
  int minCursor;
};

struct ScrollBarStyle
{
  ButtonStyle buttons;
  ScrollBarSliderStyle bar;
};

struct ScrollBarSlider;
struct ScrollBar;

namespace style {

template<class Theme>
struct FromTheme<ScrollBarSlider, Theme>
{
  constexpr static ScrollBarSliderStyle get()
  {
    auto buttonStyle = themeFor<Button, Theme>();
    return {
      themeFor<Panel, Theme>().withPadding(EdgeSize::all(0)),
      themeFor<Box, Theme>()
        .withBackgroundColor(buttonStyle.normal.background)
        .withBorderSize(buttonStyle.border)
        .withBorderColor(buttonStyle.normal.border),
      buttonStyle.border.left + buttonStyle.border.right + 2,
    };
  }
};
template<class Theme>
struct FromTheme<ScrollBar, Theme>
{
  constexpr static ScrollBarStyle get()
  {
    return {themeFor<Button, Theme>(), themeFor<ScrollBarSlider, Theme>()};
  }
};
} // namespace style

// begin ScrollBar.hpp
inline bool
scrollBarSlider(Target target,
                std::string_view id,
                int* value,
                int min,
                int max,
                SDL_Rect r = {0},
                const ScrollBarSliderStyle& style = themeFor<ScrollBarSlider>())
{
  SDL_assert(value != nullptr);
  auto g = panel(target, id, r, Layout::NONE, style.panel);

  // TODO handle min>=max
  int distance = max - min;
  int cursorWidth = std::max(r.w / distance, style.minCursor);
  int cursorMax = r.w - cursorWidth;
  int cursorPos =
    std::clamp((*value - min) * cursorMax / distance, 0, cursorMax);
  box(g, {cursorPos - 1, -1, cursorWidth, r.h}, style.cursor);
  g.end();
  return false;
}

inline bool
scrollBar(Target target,
          std::string_view id,
          int* value,
          int min,
          int max,
          SDL_Rect r = {0},
          const ScrollBarStyle& style = themeFor<ScrollBar>())
{
  if (r.w == 0) {
    r.w = makeInputSize({r.w, r.h},
                        style.buttons.font,
                        0,
                        style.buttons.border + style.buttons.padding)
            .x;
  }
  auto& buttonStyle = style.buttons;
  int buttonWidth = buttonStyle.padding.left + buttonStyle.padding.right +
                    buttonStyle.border.left + buttonStyle.border.right + 8;
  if (r.w < buttonWidth * 4) {
    r.w = buttonWidth * 4;
  }
  int buttonHeight = buttonStyle.padding.top + buttonStyle.padding.bottom +
                     buttonStyle.border.top + buttonStyle.border.bottom + 8;
  if (buttonHeight < r.h) {
    buttonHeight = r.h;
  } else {
    r.h = buttonHeight;
  }
  bool action = false;
  Group g = group(target, id, r, Layout::NONE);
  if (button(g, "prev", "<")) {
    *value = *value > min ? *value - 1 : min;
    action = true;
  }
  if (button(g, "next", ">", {r.w - buttonWidth, 0})) {
    *value = *value < max ? *value + 1 : max;
    action = true;
  }
  action = scrollBarSlider(
    g,
    "bar",
    value,
    min,
    max,
    {buttonWidth - 1, 0, r.w - buttonWidth * 2 + 2, buttonHeight},
    style.bar);
  g.end();
  return action;
}

// begin SliderStyle.hpp
struct SliderStyle
{
  ScrollBarStyle scroll;
  ElementStyle label;
};
struct Slider;

namespace style {

template<class Slider, class Theme>
struct FromTheme
{
  constexpr static SliderStyle get()
  {
    return {
      themeFor<ScrollBar, Theme>(),
      themeFor<Label, Theme>(),
    };
  }
};
} // namespace style

// begin Slider.hpp
/**
 * @[
 * @brief A slider that allows the user to choose a value in a interval
 *
 * @param target
 * @param id
 * @param labelText
 * @param value
 * @param min
 * @param max
 * @param p
 * @param style
 * @return true
 * @return false
 */
inline bool
slider(Target target,
       std::string_view id,
       std::string_view labelText,
       int* value,
       int min,
       int max,
       const SDL_Point& p = {0},
       const SliderStyle& style = themeFor<Slider>())
{
  SDL_Rect box{makeInputRect({p.x, p.y, 0, 0}, themeFor<TextBox>())};
  auto g = labeledGroup(target, labelText, box, style.label);
  auto changed = scrollBar(g, id, value, min, max, box, style.scroll);
  g.end();
  return changed;
}

inline bool
slider(Target target,
       std::string_view id,
       int* value,
       int min,
       int max,
       const SDL_Point& p = {0},
       const SliderStyle& style = themeFor<Slider>())
{
  return slider(target, id, id, value, min, max, p, style);
}
/// @]

// begin dui.hpp


} // namespace dui

#endif // DUI_SINGLE_HPP
