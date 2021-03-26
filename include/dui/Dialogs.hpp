#pragma once

#include "Label.hpp"
#include "Layer.hpp"
#include "State.hpp"
#include "Window.hpp"

namespace dui {

inline bool
messageBox(Target target,
           std::string_view id,
           std::string_view str,
           bool* open,
           WindowStyle style = dui::themeFor<Window>())
{
  if (*open) {
    auto& state = target.getState();
    int wW = state.getWidth();
    int wH = state.getHeight();

    auto l = layer(target, id, {0});
    static std::string_view title{"Message"};
    auto textSz = elementSize(style.decoration.panel.padding +
                                style.decoration.panel.border,
                              measure(title.size() > str.size() ? title : str,
                                      style.decoration.title.font,
                                      0));
    auto w = window(l, {}, title, {(wW - textSz.x) / 2, wH / 8}, style);
    label(w, str);
    if (button(w, "Ok")) {
      *open = false;
      return true;
    }
    w.end();
    colorBox(l, {0, 0, wW, wH}, {0, 0, 0, 127});
  }
  return false;
}
inline bool
messageBox(Target target,
           std::string_view id,
           bool* open,
           WindowStyle style = dui::themeFor<Window>())
{
  return messageBox(target, id, id, open, style);
}

} // namespace dui
