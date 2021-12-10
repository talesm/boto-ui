#ifndef DUI_BASIC_WIDGETS_HPP_
#define DUI_BASIC_WIDGETS_HPP_

#include <string_view>
#include "Box.hpp"
#include "EdgeSize.hpp"
#include "ElementStyle.hpp"
#include "Group.hpp"
#include "Text.hpp"
#include "Theme.hpp"

namespace boto {

/// Compute element size based on its style
inline SDL_Point
computeSize(std::string_view str,
            const ElementStyle& style,
            const SDL_Point& sz)
{
  if (sz.x != 0 && sz.y != 0) {
    return sz;
  }
  auto clientSz = measure(str, style.font, style.scale);
  auto elementSz = elementSize(style.padding + style.border, clientSz);
  if (sz.x != 0) {
    elementSz.x = sz.x;
  } else if (sz.y != 0) {
    elementSz.y = sz.y;
  }
  return elementSz;
}

/**
 * @brief Adds a generic element
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param str the text inside the element
 * @param r the element dimensions
 * @param style
 */
inline void
element(Target target,
        std::string_view str,
        const SDL_Rect& r = {0},
        const ElementStyle& style = themeFor<Element>())
{
  auto offset = style.border + style.padding;
  auto sz = computeSize(str, style, {r.w, r.h});
  auto g = group(target, {}, {r.x, r.y, sz.x, sz.y}, Layout::NONE);
  text(g, str, {offset.left, offset.top}, style);
  box(g, {0, 0, sz.x, sz.y}, style);
}

} // namespace boto

#endif // DUI_BASIC_WIDGETS_HPP_
