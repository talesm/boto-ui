#ifndef BOTO_ELEMENTS_ELEMENTPRESENTER_HPP
#define BOTO_ELEMENTS_ELEMENTPRESENTER_HPP

#include "Theme.hpp"

namespace boto {
inline void
presentElement(DisplayList& dList, const SDL_Rect& r, Status status)
{
  presentElement(dList, r, status, themeFor<Element>());
}
} // namespace boto

#endif // BOTO_ELEMENTS_ELEMENTPRESENTER_HPP
