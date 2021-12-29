#ifndef BOTO_ELEMENTS_TEXTPRESENTER_HPP_
#define BOTO_ELEMENTS_TEXTPRESENTER_HPP_

#include <string_view>
#include "Font.hpp"
#include "core/Theme.hpp"

namespace boto {

template<class STYLE, class FONT>
STYLE
adjustDefaultFont(STYLE style, FONT f)
{
  return style;
}
// TODO generic default character & default text()
} // namespace boto

#endif // BOTO_ELEMENTS_TEXTPRESENTER_HPP_
