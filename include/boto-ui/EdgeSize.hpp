#ifndef BOTO_EDGESIZE_HPP
#define BOTO_EDGESIZE_HPP

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
elementSize(const EdgeSize& edge, const SDL_Point& client = {0})
{
  return {edge.left + edge.right + client.x, edge.top + edge.bottom + client.y};
}
constexpr SDL_Point
clientSize(const EdgeSize& edge, const SDL_Point& element)
{
  auto width = edge.left + edge.right;
  auto height = edge.top + edge.bottom;
  return {element.x > width ? element.x - width : 0,
          element.y > height ? element.y - height : 0};
}

static constexpr SDL_Rect
clientRect(const EdgeSize& edge, const SDL_Rect& element)
{
  auto sz = clientSize(edge, {element.w, element.h});
  return {edge.left, edge.top, sz.x, sz.y};
}
} // namespace boto

#endif // BOTO_EDGESIZE_HPP
