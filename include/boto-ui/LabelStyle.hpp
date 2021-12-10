#ifndef DUI_LABELSTYLE_HPP_
#define DUI_LABELSTYLE_HPP_

#include "ElementStyle.hpp"
#include "Theme.hpp"

namespace dui {

struct Label;

namespace style {
template<class Theme>
struct FromTheme<Label, Theme> : FromTheme<Element, Theme>
{};
} // namespace style

} // namespace dui

#endif // DUI_LABELSTYLE_HPP_
