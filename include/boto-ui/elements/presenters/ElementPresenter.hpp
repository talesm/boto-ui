#ifndef BOTO_ELEMENTPRESENTER_HPP_
#define BOTO_ELEMENTPRESENTER_HPP_

#include "EdgeSize.hpp"
#include "core/DisplayList.hpp"
#include "core/Status.hpp"
#include "core/Theme.hpp"

namespace boto {

// Tag
inline void
presentElement(DisplayList& dList, const SDL_Rect& r, SDL_Color color)
{
  dList.push(r, color);
}

inline void
presentElement(DisplayList& dList, const SDL_Rect& r, SDL_Texture* texture)
{
  dList.push(r, texture, {255, 255, 255, 255});
}

struct TintedTextureStyle
{
  SDL_Texture* texture;
  SDL_Color color;
};

inline void
presentElement(DisplayList& dList,
               const SDL_Rect& r,
               const TintedTextureStyle& style)
{
  dList.push(r, style.texture, style.color);
}

struct PartialTextureStyle
{
  SDL_Texture* texture;
  SDL_Color color;
  SDL_Rect srcRect;

  constexpr PartialTextureStyle(SDL_Texture* texture,
                                SDL_Color color,
                                const SDL_Rect& srcRect)
    : texture(texture)
    , color(color)
    , srcRect(srcRect)
  {}

  constexpr PartialTextureStyle(SDL_Texture* texture, const SDL_Rect& srcRect)
    : texture(texture)
    , color{255, 255, 255, 255}
    , srcRect(srcRect)
  {}
};

inline void
presentElement(DisplayList& dList,
               const SDL_Rect& r,
               const PartialTextureStyle& style)
{
  dList.push(r, style.texture, style.color, style.srcRect);
}

template<class BoxStyle>
struct ElementStyleT
{
  EdgeSize border;
  BoxStyle background;
  BoxStyle left;
  BoxStyle top;
  BoxStyle right;
  BoxStyle bottom;

  constexpr ElementStyleT withBorderSize(const EdgeSize& border) const
  {
    return {border, background, right, bottom, left, top};
  }
  constexpr ElementStyleT withBackground(const BoxStyle& background) const
  {
    return {border, background, left, top, right, bottom};
  }
  constexpr ElementStyleT withLeft(const BoxStyle& left) const
  {
    return {border, background, left, top, right, bottom};
  }
  constexpr ElementStyleT withTop(const BoxStyle& top) const
  {
    return {border, background, left, top, right, bottom};
  }
  constexpr ElementStyleT withRight(const BoxStyle& right) const
  {
    return {border, background, left, top, right, bottom};
  }
  constexpr ElementStyleT withBottom(const BoxStyle& bottom) const
  {
    return {border, background, left, top, right, bottom};
  }
  constexpr ElementStyleT invertBorders() const
  {
    return {border, background, right, bottom, left, top};
  }
  constexpr ElementStyleT withBorder(const BoxStyle& color) const
  {
    return {border, background, color, color, color, color};
  }
};

template<class BoxStyle>
constexpr SDL_Point
clientSize(const SDL_Point& sz, const ElementStyleT<BoxStyle>& style)
{
  return clientSize(sz, style.border);
}
template<class BoxStyle>
constexpr SDL_Point
clientSize(const SDL_Rect& r, const ElementStyleT<BoxStyle>& style)
{
  return clientSize(r, style.border);
}

template<class BoxStyle>
constexpr SDL_Point
clientRect(const SDL_Rect& r, const ElementStyleT<BoxStyle>& style)
{
  return clientRect(r, style.border);
}

using ElementStyle = ElementStyleT<SDL_Color>;

template<class BoxStyle>
inline void
presentElement(DisplayList& dList,
               const SDL_Rect& r,
               const ElementStyleT<BoxStyle>& style)
{
  auto c = style.background;
  auto e = style.right;
  auto n = style.top;
  auto w = style.left;
  auto s = style.bottom;
  int esz = style.border.right;
  int nsz = style.border.top;
  int wsz = style.border.left;
  int ssz = style.border.bottom;
  presentElement(dList, {r.x + 1, r.y + r.h - ssz, r.w - 2, ssz}, s);
  presentElement(dList, {r.x + r.w - esz, r.y + 1, esz, r.h - 2}, e);
  presentElement(dList, {r.x + 1, r.y, r.w - 2, nsz}, n);
  presentElement(dList, {r.x, r.y + 1, wsz, r.h - 2}, w);
  presentElement(
    dList, {r.x + esz, r.y + nsz, r.w - esz - wsz, r.h - nsz - ssz}, c);
}

struct BorderSize;
struct BackgroundColor;
struct DefaultColor;
struct BorderColor;
struct LeftBorderColor;
struct TopBorderColor;
struct RightBorderColor;
struct BottomBorderColor;
struct Element;

template<class THEME>
struct StyleFor<THEME, BorderSize>
{
  constexpr static EdgeSize get(ThemeT<THEME>&) { return EdgeSize::all(1); }
};

template<class THEME>
struct StyleFor<THEME, BackgroundColor>
{
  constexpr static SDL_Color get(ThemeT<THEME>&)
  {
    return {255, 255, 255, 255};
  }
};

template<class THEME>
struct StyleFor<THEME, DefaultColor>
{
  constexpr static SDL_Color get(ThemeT<THEME>&) { return {0, 0, 0, 255}; }
};

template<class THEME>
struct StyleFor<THEME, BorderColor>
{
  constexpr static auto get(ThemeT<THEME>& t)
  {
    return t.template of<DefaultColor>();
  }
};
template<class THEME>
struct StyleFor<THEME, LeftBorderColor>
{
  constexpr static auto get(ThemeT<THEME>& t)
  {
    return t.template of<BorderColor>();
  }
};
template<class THEME>
struct StyleFor<THEME, TopBorderColor>
{
  constexpr static auto get(ThemeT<THEME>& t)
  {
    return t.template of<BorderColor>();
  }
};
template<class THEME>
struct StyleFor<THEME, RightBorderColor>
{
  constexpr static auto get(ThemeT<THEME>& t)
  {
    return t.template of<BorderColor>();
  }
};
template<class THEME>
struct StyleFor<THEME, BottomBorderColor>
{
  constexpr static auto get(ThemeT<THEME>& t)
  {
    return t.template of<BorderColor>();
  }
};
template<class THEME>
struct StyleFor<THEME, Element>
{
  constexpr static ElementStyle get(ThemeT<THEME>& t)
  {
    return {
      t.template of<BorderSize>(),
      t.template of<BackgroundColor>(),
      t.template of<LeftBorderColor>(),
      t.template of<TopBorderColor>(),
      t.template of<RightBorderColor>(),
      t.template of<BottomBorderColor>(),
    };
  }
};

template<class ELEMENT = Element, class THEME>
const StyleType<THEME, ELEMENT>&
elementStyle(ThemeT<THEME>& theme, StatusFlags status)
{
  if (status.test(Status::FOCUSED)) {
    if (status.test(Status::GRABBED)) {
      if (status.test(Status::HOVERED)) {
        return theme.template of<Hovered<Grabbed<Focused<ELEMENT>>>>();
      }
      return theme.template of<Grabbed<Focused<ELEMENT>>>();
    }
    if (status.test(Status::HOVERED)) {
      return theme.template of<Hovered<Focused<ELEMENT>>>();
    }
    return theme.template of<Focused<ELEMENT>>();
  }
  if (status.test(Status::GRABBED)) {
    if (status.test(Status::HOVERED)) {
      return theme.template of<Hovered<Grabbed<ELEMENT>>>();
    }
    return theme.template of<Grabbed<ELEMENT>>();
  }
  if (status.test(Status::HOVERED)) {
    return theme.template of<Hovered<ELEMENT>>();
  }
  return theme.template of<ELEMENT>();
}

} // namespace boto

#endif // BOTO_ELEMENTPRESENTER_HPP_
