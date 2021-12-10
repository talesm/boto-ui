#ifndef BOTO_LABELSTYLE_HPP_
#define BOTO_LABELSTYLE_HPP_

#include "ElementStyle.hpp"
#include "Theme.hpp"

namespace boto {

struct Label;

namespace style {
template<class Theme>
struct FromTheme<Label, Theme> : FromTheme<Element, Theme>
{};
} // namespace style

} // namespace boto

#endif // BOTO_LABELSTYLE_HPP_
