#ifndef BOTO_UTIL_RECT_HPP_
#define BOTO_UTIL_RECT_HPP_

#include <climits>
#include <cmath>
#include <SDL_rect.h>

namespace boto {

constexpr int Undefined = INT_MIN;

constexpr bool
pointInRect(const SDL_Point& p, const SDL_Rect& r)
{
  if (p.x < r.x || p.y < r.y) {
    return false;
  }
  if (r.w != Undefined && p.x - r.x >= r.w) {
    return false;
  }
  if (r.h != Undefined && p.y - r.y >= r.h) {
    return false;
  }
  return true;
}

constexpr SDL_Rect
intersection(const SDL_Rect& r1, const SDL_Rect& r2)
{
  int xx = std::max(r1.x, r2.x);
  int yy = std::max(r1.y, r2.y);
  int ww = std::min(r1.w == Undefined ? Undefined : r1.w - xx + r1.x,
                    r2.w == Undefined ? Undefined : r2.w - xx + r2.x);
  int hh = std::min(r1.h == Undefined ? Undefined : r1.h - yy + r1.y,
                    r2.h == Undefined ? Undefined : r2.h - yy + r2.y);
  return {xx, yy, ww, hh};
}

} // namespace boto

#endif // BOTO_UTIL_RECT_HPP_
