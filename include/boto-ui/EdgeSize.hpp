#ifndef BOTO_EDGESIZE_HPP_
#define BOTO_EDGESIZE_HPP_

#include <SDL.h>

namespace boto {

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
measureElement(const EdgeSize& edge)
{
  return {edge.left + edge.right, edge.top + edge.bottom};
}
// Deprecated??
template<class T>
constexpr SDL_Point
measureElement(T t, const SDL_Point& client)
{
  return measureElement(t) + client;
}
constexpr SDL_Point
clientSize(const SDL_Point& sz, EdgeSize edge)
{
  auto width = edge.left + edge.right;
  auto height = edge.top + edge.bottom;
  return {sz.x > width ? sz.x - width : 0, sz.y > height ? sz.y - height : 0};
}
constexpr SDL_Point
clientSize(const SDL_Rect& r, EdgeSize edge)
{
  return clientSize(SDL_Point{r.w, r.h}, edge);
}

/// @deprecated use clientRect() instead
constexpr SDL_Rect
clientRectRelative(const SDL_Rect& element, EdgeSize edge)
{
  auto sz = clientSize(SDL_Point{element.w, element.h}, edge);
  return {edge.left, edge.top, sz.x, sz.y};
}
constexpr SDL_Rect
clientRect(const SDL_Rect& r, EdgeSize edge)
{
  auto sz = clientSize(r, edge);
  return {r.x + edge.left, r.y + edge.top, sz.x, sz.y};
}
} // namespace boto

#endif // BOTO_EDGESIZE_HPP_
