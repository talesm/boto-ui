#ifndef DUI_BASE_WIDGETS_HPP_
#define DUI_BASE_WIDGETS_HPP_

#include <string_view>
#include "Frame.hpp"

namespace dui {

inline void
label(Frame* frame,
      std::string_view text,
      SDL_Point p,
      SDL_Color color = {0, 0, 0, 255})
{
  for (auto ch : text) {
    frame->character({p.x, p.y, 8, 8}, color, ch);
    p.x += 8; // TODO measure this
  }
}

} // namespace dui

#endif // DUI_BASE_WIDGETS_HPP_
