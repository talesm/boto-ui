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
  dList.push(r, texture, {255, 255, 255, 255}, {0, 0, r.w, r.h});
}

} // namespace boto

#endif // BOTO_ELEMENTS_ELEMENTPRESENTER_HPP
