#pragma once

#include <string_view>
#include <SDL_rect.h>
#include "EdgeSize.hpp"
#include "core/Container.hpp"
#include "core/State.hpp"
#include "core/Target.hpp"

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
      const SDL_Point& endPadding,
      const GroupStyle& style)
{
  return target.container(id,
                          r,
                          RequestEvent::INPUT,
                          offset,
                          endPadding,
                          style.layout,
                          style.elementSpacing);
}
inline Container
group(Target target,
      std::string_view id,
      const SDL_Rect& r,
      const SDL_Point& offset,
      const SDL_Point& endPadding = {})
{
  return group(target, id, r, offset, endPadding, target.styleFor<Group>());
}

inline Container
group(Target target,
      std::string_view id,
      const SDL_Rect& r,
      const EdgeSize& padding,
      const GroupStyle& style)
{
  return target.container(id,
                          r,
                          RequestEvent::INPUT,
                          {padding.left, padding.top},
                          {padding.right, padding.bottom},
                          style.layout,
                          style.elementSpacing);
}

inline Container
group(Target target,
      std::string_view id,
      const SDL_Rect& r,
      const EdgeSize& padding)
{
  return group(target, id, r, padding, target.styleFor<Group>());
}
inline Container
group(Target target,
      std::string_view id,
      const SDL_Rect& r,
      const GroupStyle& style)
{
  return target.container(
    id, r, RequestEvent::INPUT, {}, {}, style.layout, style.elementSpacing);
}
inline Container
group(Target target, std::string_view id, const SDL_Rect& r = {})
{
  return group(target, id, r, target.styleFor<Group>());
}

/// @copydoc group
/// @ingroup groups
inline Container
group(Target target, std::string_view id, const SDL_Rect& r, Layout layout)
{
  return group(target, id, r, target.styleFor<Group>().withLayout(layout));
}

} // namespace boto
