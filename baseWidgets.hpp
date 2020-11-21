#ifndef DUI_BASE_WIDGETS_HPP_
#define DUI_BASE_WIDGETS_HPP_

#include <string_view>
#include "Frame.hpp"

namespace dui {

inline void
label(Frame& frame,
      std::string_view text,
      const SDL_Point& p,
      SDL_Color color = {0, 0, 0, 255})
{
  frame.string({p.x + 1, p.y + 1}, color, text);
  auto adv = frame.measure(text);
  adv.x += p.x + 1;
  adv.y += p.y + 1;
  frame.advance(adv);
}

} // namespace dui

#endif // DUI_BASE_WIDGETS_HPP_
