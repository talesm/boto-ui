#pragma once

#include <string_view>
#include <SDL_rect.h>
#include "GroupStyle.hpp"
#include "State.hpp"
#include "Target.hpp"

namespace dui {

/**
 * @brief A grouping of widgets
 *
 * This externally viewed as a single widget, so it can be used to create
 * composed elements
 *
 */
class Group : public Targetable<Group>
{
  Target parent;
  std::string_view id;
  bool locked = false;
  bool ended = false;
  SDL_Rect rect;
  SDL_Point topLeft;
  SDL_Point bottomRight;
  GroupStyle style;

  static constexpr SDL_Point makeCaret(const SDL_Point& caret, int x, int y)
  {
    return {caret.x + x, caret.y + y};
  }

public:
  /**
   * @brief Construct a new branch Group object
   *
   * You probably want to use group() instead of this.
   *
   * @param parent the parent group. MUST NOT BE NULL
   * @param scrollOffset the scrolling offset for the elements inside the group
   * @param id the group id
   * @param rect the rect. Either w or h being 0 means it will auto size
   * @param style
   */
  Group(Target parent,
        std::string_view id,
        const SDL_Point& scrollOffset,
        const SDL_Rect& rect,
        const GroupStyle& style);

  ~Group()
  {
    if (!ended) {
      end();
    }
  }
  Group(const Group&) = delete;
  /// Move ctor
  Group(Group&& rhs);
  Group& operator=(const Group& rhs) = delete;
  /// Move assignment op
  Group& operator=(Group&& rhs);

  /// Returns true if this can accept more elements
  operator bool() const { return !ended; }

  /// Set width
  void setWidth(int v) { rect.w = v; }

  /// Set height
  void setHeight(int v) { rect.h = v; }

  /// Convert to target object
  operator Target() &
  {
    return {
      &parent.getState(),
      id,
      rect,
      topLeft,
      bottomRight,
      locked,
      style,
    };
  }

  /// Finished group and stop accepting new elements
  void end();
};

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
 * @return Group
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

inline Group::Group(Target parent,
                    std::string_view id,
                    const SDL_Point& scroll,
                    const SDL_Rect& rect,
                    const GroupStyle& style)
  : parent(parent)
  , id(id)
  , rect(rect)
  , topLeft(makeCaret(parent.getCaret(), rect.x - scroll.x, rect.y - scroll.y))
  , bottomRight(topLeft)
  , style(style)
{
  parent.lock(id, rect);
}

inline void
Group::end()
{
  SDL_assert(!ended);
  if (rect.w == 0) {
    rect.w = width();
  }
  if (rect.h == 0) {
    rect.h = height();
  }
  parent.unlock(id, rect);
  parent.advance({rect.x + rect.w, rect.y + rect.h});
  ended = true;
  parent = {};
}

inline Group::Group(Group&& rhs)
  : parent(rhs.parent)
  , id(std::move(rhs.id))
  , rect(rhs.rect)
  , topLeft(rhs.topLeft)
  , bottomRight(rhs.bottomRight)
{
  rhs.ended = true;
}

inline Group&
Group::operator=(Group&& rhs)
{
  SDL_assert(ended);
  this->~Group();
  new (this) Group(std::move(rhs));
  return *this;
}

} // namespace dui
