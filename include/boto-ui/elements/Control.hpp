#ifndef BOTO_CONTROL_HPP_
#define BOTO_CONTROL_HPP_

#include <string_view>
#include "EdgeSize.hpp"
#include "Element.hpp"
#include "Text.hpp"
#include "core/Theme.hpp"
#include "presenters/ControlPresenter.hpp"

namespace boto {

/// Compute control size based on its style
inline SDL_Point
computeSize(std::string_view str,
            const ControlStyle& style,
            const SDL_Point& sz)
{
  if (sz.x != 0 && sz.y != 0) {
    return sz;
  }
  auto elementSz = measureElement(style.padding + style.decoration.border) +
                   measure(str, style.text.font, style.text.scale);
  if (sz.x != 0) {
    elementSz.x = sz.x;
  } else if (sz.y != 0) {
    elementSz.y = sz.y;
  }
  return elementSz;
}

/**
 * @brief Adds a generic control
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param str the text inside the control
 * @param r the control dimensions
 * @param style
 */
inline void
control(Target target,
        std::string_view id,
        std::string_view str,
        const SDL_Rect& r,
        RequestEvent req,
        const ControlStyle& style)
{
  auto sz = computeSize(str, style, {r.w, r.h});
  auto el = target.element(id, {r.x, r.y, sz.x, sz.y}, req);

  auto& dList = target.displayList();
  auto clip = dList.clip(el.rect);
  auto offset = style.decoration.border + style.padding;
  presentText(
    dList, str, {offset.left + el.rect.x, offset.top + el.rect.y}, style.text);
  presentElement(dList, el.rect, style.decoration);
}
inline void
control(Target target,
        std::string_view str,
        const SDL_Rect& r,
        const ControlStyle& style)
{
  control(target, str, str, r, RequestEvent::INPUT, style);
}
inline void
control(Target target, std::string_view str, const SDL_Rect& r = {0})
{
  control(target, str, str, r, RequestEvent::INPUT, target.styleFor<Control>());
}

} // namespace boto

#endif // BOTO_CONTROL_HPP_
