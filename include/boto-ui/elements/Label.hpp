#ifndef BOTO_LABEL_HPP_
#define BOTO_LABEL_HPP_

#include <string_view>
#include "Control.hpp"
#include "EdgeSize.hpp"

namespace boto {

/**
 * @brief A simple label
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param str the text to show
 * @param p the local relative postion to add the label. Its size is
 * automatically evaluated
 * @param style
 */
template<class STYLE>
inline void
label(Target target, std::string_view str, const SDL_Point& p, STYLE style)
{
  control(target, {}, str, {p.x, p.y, 0, 0}, RequestEvent::HOVER, style);
}
inline void
label(Target target, std::string_view str, const SDL_Point& p = {0})
{
  label(target, str, p, target.styleFor<Label>());
}
/**
 * @brief A centered label
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param str the text to show
 * @param r the local relative rect to add the label.
 * @param style
 */
inline void
centeredLabel(Target target,
              std::string_view str,
              SDL_Rect r,
              const ControlStyle& style)
{
  auto textSz = measure(str, style.text.font, style.text.scale);
  SDL_Point minElementSz =
    elementSize(style.padding + style.decoration.border, textSz);
  if (r.w == 0) {
    r.w = minElementSz.x;
  }
  if (r.h == 0) {
    r.h = minElementSz.y;
  }
  SDL_Point clientSz =
    clientSize(style.padding + style.decoration.border, {r.w, r.h});

  text(target,
       str,
       {(clientSz.x - textSz.x) / 2 + style.padding.left,
        (clientSz.y - textSz.y) / 2 + style.padding.top},
       style);
  element(target, r, style.decoration);
}
inline void
centeredLabel(Target target, std::string_view str, const SDL_Rect& r)
{
  centeredLabel(target, str, r, target.styleFor<Label>());
}
} // namespace boto

#endif // BOTO_LABEL_HPP_
