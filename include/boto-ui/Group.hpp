#pragma once

#include <string_view>
#include <SDL_rect.h>
#include "GroupStyle.hpp"
#include "Target.hpp"
#include "core/Container.hpp"
#include "core/State.hpp"

namespace boto {

/**
 * @brief Create group
 * @ingroup groups
 *
 * @param target the parent group or frame
 * @param id the group id
 * @param offset the scroll offset
 * @param r the group dimensions
 * @param layout the layout
 * @param style the group style
 * @return Container
 */
inline Container
group(Target target,
      std::string_view id,
      const SDL_Rect& r,
      const SDL_Point& offset,
      const SDL_Point& endPadding = {},
      const GroupStyle& style = themeFor<Group>())
{
  return target.container(
    id, r, offset, endPadding, style.layout, style.elementSpacing);
}
inline Container
group(Target target,
      std::string_view id,
      const SDL_Rect& r = {0},
      const GroupStyle& style = themeFor<Group>())
{
  return target.container(id, r, {}, {}, style.layout, style.elementSpacing);
}

/// @copydoc group
/// @ingroup groups
inline Container
group(Target target,
      std::string_view id,
      const SDL_Rect& r,
      Layout layout,
      const GroupStyle& style = themeFor<Group>())
{
  return group(target, id, r, style.withLayout(layout));
}

/// @copydoc group
/// @ingroup groups
inline Container
offsetGroup(Target target,
            std::string_view id,
            const SDL_Point& offset,
            const SDL_Rect& r,
            const GroupStyle& style = themeFor<Group>())
{
  return target.container(
    id, r, offset, {}, style.layout, style.elementSpacing);
}

/// @copydoc group
/// @ingroup groups
inline Container
offsetGroup(Target target,
            std::string_view id,
            const SDL_Point& offset,
            const SDL_Rect& r,
            Layout layout,
            const GroupStyle& style = themeFor<Group>())
{
  return offsetGroup(target, id, offset, r, style.withLayout(layout));
}

} // namespace boto
