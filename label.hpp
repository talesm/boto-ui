#ifndef DUI_LABEL_HPP_
#define DUI_LABEL_HPP_

#include <string_view>
#include "EdgeSize.hpp"
#include "Group.hpp"

namespace dui {

/// The label style
struct LabelStyle
{
  SDL_Color text;
  EdgeSize padding;

  constexpr LabelStyle withText(SDL_Color c) const
  {
    auto next = *this;
    next.text = c;
    return next;
  }

  constexpr LabelStyle withMargin(EdgeSize sz) const
  {
    auto next = *this;
    next.padding = sz;
    return next;
  }
};

namespace style {
constexpr LabelStyle LABEL{TEXT, EdgeSize::all(2)};
} // namespace style

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
label(Group& target,
      std::string_view str,
      const SDL_Point& p = {0},
      const LabelStyle& style = style::LABEL)
{
  auto adv = measure(str);
  auto margin = style.padding;
  SDL_Rect r{p.x,
             p.y,
             adv.x + margin.left + margin.right,
             adv.y + margin.top + margin.bottom};
  auto g = group(target, str, r, Layout::NONE);
  text(g, str, {margin.left, margin.top}, style.text);

  g.end();
}
} // namespace dui

#endif // DUI_LABEL_HPP_
