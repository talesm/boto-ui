#pragma once

#include "Label.hpp"
#include "Layer.hpp"
#include "State.hpp"
#include "Window.hpp"

namespace dui {

template<class T>
inline int
choiceDialog(Target target,
             std::string_view id,
             std::string_view str,
             std::initializer_list<T> options,
             bool* open,
             WindowStyle style = dui::themeFor<Window>())
{
  int selOption = 0;
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
    if (!str.empty()) {
      label(w, str);
    }
    int i = 0;
    for (auto option : options) {
      ++i;
      if (button(w, option)) {
        selOption = i;
        *open = false;
      }
    }
    w.end();
    colorBox(l, {0, 0, wW, wH}, {0, 0, 0, 127});
  }
  return selOption;
}

template<class T>
inline int
choiceDialog(Target target,
             std::string_view id,
             std::initializer_list<T> options,
             bool* open,
             WindowStyle style = dui::themeFor<Window>())
{
  return choiceDialog(target, id, id, options, open, style);
}

inline bool
messageDialog(Target target,
              std::string_view id,
              std::string_view str,
              bool* open,
              WindowStyle style = dui::themeFor<Window>())
{
  return choiceDialog(target, id, str, {"Ok"}, open, style) == 1;
}
inline bool
messageDialog(Target target,
              std::string_view id,
              bool* open,
              WindowStyle style = dui::themeFor<Window>())
{
  return messageDialog(target, id, id, open, style);
}

} // namespace dui
