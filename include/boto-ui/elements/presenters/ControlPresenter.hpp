#ifndef BOTO_ELEMENTS_CONTROLPRESENTER_HPP
#define BOTO_ELEMENTS_CONTROLPRESENTER_HPP

#include "ControlPresenter.hpp"
#include "EdgeSize.hpp"
#include "ElementPresenter.hpp"
#include "TextPresenter.hpp"
#include "core/DisplayList.hpp"
#include "core/Status.hpp"
#include "core/Theme.hpp"
#include "util/Rect.hpp"

namespace boto {

template<class DECORATION>
struct ControlStyleT
{
  DECORATION decoration;
  EdgeSize padding;
  nullptr_t text;
};

template<class STYLE>
inline void
presentControl(DisplayList& dList,
               std::string_view text,
               const SDL_Rect& r,
               STYLE style)
{
  presentElement(dList, r, style);
}

template<class DECORATION>
inline void
presentControl(DisplayList& dList,
               std::string_view text,
               const SDL_Rect& r,
               const ControlStyleT<DECORATION>& style)
{
  if (r.w != Undefined && r.h != Undefined) {
    return presentElement(dList, r, style.decoration);
  }
}

} // namespace boto

#endif // BOTO_ELEMENTS_CONTROLPRESENTER_HPP
