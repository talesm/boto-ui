#ifndef BOTO_ELEMENTS_ELEMENTPRESENTER_HPP
#define BOTO_ELEMENTS_ELEMENTPRESENTER_HPP

#include "core/DisplayList.hpp"
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

struct BackgroundColor;
struct DefaultColor;
struct BorderColor;
struct LeftBorderColor;
struct TopBorderColor;
struct RightBorderColor;
struct BottomBorderColor;
struct Element;

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
} // namespace boto

#endif // BOTO_ELEMENTS_ELEMENTPRESENTER_HPP
