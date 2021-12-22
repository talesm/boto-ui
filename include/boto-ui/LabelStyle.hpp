#ifndef BOTO_LABELSTYLE_HPP_
#define BOTO_LABELSTYLE_HPP_

#include "ControlStyle.hpp"
#include "Theme.hpp"

namespace boto {

struct Label;

namespace style {
template<class Theme>
struct FromTheme<Label, Theme> : FromTheme<Control, Theme>
{};
} // namespace style

} // namespace boto

#endif // BOTO_LABELSTYLE_HPP_
