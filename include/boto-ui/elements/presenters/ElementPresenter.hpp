#ifndef BOTO_ELEMENTS_ELEMENTPRESENTER_HPP
#define BOTO_ELEMENTS_ELEMENTPRESENTER_HPP

#include "core/DisplayList.hpp"
#include "core/Theme.hpp"

namespace boto {
// Tag
struct Element;

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
  SDL_Rect srcRect;
};

struct TintedPartialTextureStyle
{
  SDL_Texture* texture;
  SDL_Color color;
  SDL_Rect srcRect;

  constexpr TintedPartialTextureStyle(SDL_Texture* texture,
                                      SDL_Color color,
                                      SDL_Rect srcRect)
    : texture(texture)
    , color(color)
    , srcRect(srcRect)
  {}

  constexpr TintedPartialTextureStyle(const PartialTextureStyle& style)
    : texture(style.texture)
    , color{255, 255, 255, 255}
    , srcRect(style.srcRect)
  {}
};
inline void
presentElement(DisplayList& dList,
               const SDL_Rect& r,
               const TintedPartialTextureStyle& style)
{
  dList.push(r, style.texture, style.color, style.srcRect);
}
} // namespace boto

#endif // BOTO_ELEMENTS_ELEMENTPRESENTER_HPP
