#pragma once

#include <string_view>
#include <SDL_rect.h>
#include "GroupStyle.hpp"
#include "Target.hpp"
#include "core/Container.hpp"
#include "core/State.hpp"

namespace boto {

/**
 * @brief A grouping of elements
 *
 * This externally viewed as a single widget, so it can be used to create
 * composed elements
 *
 */
// class Group
// {
// public:
//   Group
// };

/**
 * @brief Create group
 * @ingroup groups
 *
 * @param target the parent group or frame
 * @param id the group id
 * @param scrollOffset the scroll offset
 * @param r the group dimensions
 * @param layout the layout
 * @param style the group style
 * @return Container
 */
inline Group
group(Target target,
      std::string_view id,
      const SDL_Rect& r = {0},
      const GroupStyle& style = themeFor<Group>())
{
  return {target, id, {0, 0}, r, style};
}

/// @copydoc group
/// @ingroup groups
inline Group
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
inline Group
offsetGroup(Target target,
            std::string_view id,
            const SDL_Point& scrollOffset,
            const SDL_Rect& r,
            const GroupStyle& style = themeFor<Group>())
{
  return {target, id, scrollOffset, r, style};
}

/// @copydoc group
/// @ingroup groups
inline Group
offsetGroup(Target target,
            std::string_view id,
            const SDL_Point& scrollOffset,
            const SDL_Rect& r,
            Layout layout,
            const GroupStyle& style = themeFor<Group>())
{
  return offsetGroup(target, id, scrollOffset, r, style.withLayout(layout));
}

} // namespace boto
