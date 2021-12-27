#ifndef BOTO_CONTROL_HPP_
#define BOTO_CONTROL_HPP_

#include <string_view>
#include "EdgeSize.hpp"
#include "Theme.hpp"
#include "elements/Element.hpp"
#include "elements/Text.hpp"

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
  auto clientSz = measure(str, style.text.font, style.text.scale);
  auto elementSz =
    elementSize(style.padding + style.decoration.border, clientSz);
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
inline const EventTargetState&
control(Target target,
        std::string_view id,
        std::string_view str,
        const SDL_Rect& r = {0},
        RequestEvent req = RequestEvent::INPUT,
        const ControlStyle& style = themeFor<Control>())
{
  auto sz = computeSize(str, style, {r.w, r.h});
  auto& el = target.check(id, {r.x, r.y, sz.x, sz.y}, req);

  auto& dList = target.getDisplayList();
  auto clip = dList.clip(el.rect);
  auto offset = style.decoration.border + style.padding;
  presentText(dList,
              str,
              {offset.left + el.rect.x, offset.top + el.rect.y},
              el.status,
              adjustDefaultFont(style.text, target.getFont()));
  presentElement(dList, el.rect, el.status, style);
  return el;
}
inline void
control(Target target,
        std::string_view str,
        const SDL_Rect& r = {0},
        const ControlStyle& style = themeFor<Control>())
{
  control(target, str, str, r, RequestEvent::INPUT, style);
}

} // namespace boto

#endif // BOTO_CONTROL_HPP_