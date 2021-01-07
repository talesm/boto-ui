#ifndef DUI_LABEL_HPP_
#define DUI_LABEL_HPP_

#include <string_view>
#include "EdgeSize.hpp"
#include "Group.hpp"
#include "Element.hpp"

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
} // namespace dui

#endif // DUI_LABEL_HPP_