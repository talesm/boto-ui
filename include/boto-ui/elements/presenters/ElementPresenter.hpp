#ifndef BOTO_ELEMENTS_ELEMENTPRESENTER_HPP
#define BOTO_ELEMENTS_ELEMENTPRESENTER_HPP

#include "Theme.hpp"

namespace boto {

inline void
presentElement(DisplayList& dList,
               const SDL_Rect& r,
               StatusFlags status,
               SDL_Color color)
{
  dList.push(r, color);
}

inline void
presentElement(DisplayList& dList,
               const SDL_Rect& r,
               StatusFlags status,
               SDL_Texture* texture)
{
  dList.push(
    r, {255, 255, 255, 255}, SDL_BLENDMODE_BLEND, texture, {0, 0, r.w, r.h});
}

inline void
presentElement(DisplayList& dList, const SDL_Rect& r, StatusFlags status)
{
  presentElement(dList, r, status, themeFor<Element>());
}
} // namespace boto

#endif // BOTO_ELEMENTS_ELEMENTPRESENTER_HPP
