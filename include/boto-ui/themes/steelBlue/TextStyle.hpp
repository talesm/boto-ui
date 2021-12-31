#ifndef BOTO_THEMES_STEELBLUE_TEXTSTYLE_HPP_
#define BOTO_THEMES_STEELBLUE_TEXTSTYLE_HPP_

#include "Font.hpp"
#include "core/DisplayList.hpp"
#include "core/Theme.hpp"
#include "elements/presenters/TextPresenter.hpp"

namespace boto {

// Text style
template<>
struct StyleFor<SteelBlue, TextColor>
{
  static SDL_Color get(ThemeT<SteelBlue>& theme) { return {45, 72, 106, 255}; }
};

} // namespace boto

#endif // BOTO_TEXTSTYLE_HPP_
