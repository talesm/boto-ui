#ifndef DUI_LABEL_HPP_
#define DUI_LABEL_HPP_

#include <string_view>
#include "EdgeSize.hpp"
#include "Element.hpp"
#include "Group.hpp"

namespace dui {

/**
 * @brief
 *
 * @param target the parent group or frame
 * @param str the text to show
 * @param p the local relative postion to add the label. Its size is
 * automatically evaluated
 * @param style
 */
inline void
label(Target target,
      std::string_view str,
      const SDL_Point& p = {0},
      const ElementStyle& style = themeFor<Label>())
{
  element(target, str, {p.x, p.y, 0, 0}, style);
}
inline void
centeredLabel(Target target,
              std::string_view str,
              SDL_Rect r,
              const ElementStyle& style = themeFor<Label>())
{
  auto textSz = measure(str, style.font, style.scale);
  SDL_Point minElementSz = elementSize(style.padding + style.border, textSz);
  if (r.w == 0) {
    r.w = minElementSz.x;
  }
  if (r.h == 0) {
    r.h = minElementSz.y;
  }
  SDL_Point clientSz = clientSize(style.padding + style.border, {r.w, r.h});

  text(target,
       str,
       {(clientSz.x - textSz.x) / 2 + style.padding.left,
        (clientSz.y - textSz.y) / 2 + style.padding.top},
       style);
  box(target, r, style);
}
} // namespace dui

#endif // DUI_LABEL_HPP_
