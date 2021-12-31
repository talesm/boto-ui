#ifndef BOTO_THEMES_STEELBLUE_ELEMENTSTYLE_HPP_
#define BOTO_THEMES_STEELBLUE_ELEMENTSTYLE_HPP_

#include <SDL.h>
#include "EdgeSize.hpp"
#include "core/DisplayList.hpp"
#include "core/Status.hpp"
#include "core/Theme.hpp"
#include "elements/presenters/ElementPresenter.hpp"

namespace boto {

template<>
struct StyleFor<SteelBlue, BackgroundColor>
{
  constexpr static SDL_Color get(Theme&) { return {219, 228, 240, 240}; }
};
template<>
struct StyleFor<SteelBlue, DefaultColor>
{
  constexpr static SDL_Color get(Theme&) { return {45, 72, 106, 255}; }
};
} // namespace boto

#endif // BOTO_THEMES_STEELBLUE_ELEMENTSTYLE_HPP_
