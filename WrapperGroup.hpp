#ifndef DUI_WRAPPERGROUP_HPP
#define DUI_WRAPPERGROUP_HPP

#include "EdgeSize.hpp"
#include "Group.hpp"

namespace dui {

/// A class to make wrapper elements
class WrapperGroup : public Group
{
  EdgeSize padding;
  static constexpr SDL_Rect paddedSize(SDL_Rect rect, const EdgeSize& padding)
  {
    rect.x += padding.left;
    rect.y += padding.top;
    if (rect.w > padding.left + padding.right) {
      rect.w -= padding.left + padding.right;
    }
    if (rect.h > padding.top + padding.bottom) {
      rect.h -= padding.top + padding.bottom;
    }
    return rect;
  }

protected:
  virtual void afterUnwrap() = 0;

public:
  WrapperGroup(Group* parent,
               std::string_view id,
               const SDL_Rect& rect,
               Layout layout,
               const EdgeSize& padding)
    : Group(parent, id, paddedSize(rect, padding), layout)
    , padding(padding)
  {}

  void end();
};
inline void
WrapperGroup::end()
{
  if (!valid()) {
    return;
  }
  rect.x -= padding.left;
  rect.y -= padding.top;
  topLeft.x -= padding.left;
  topLeft.y -= padding.top;
  bottomRight.x += padding.right;
  bottomRight.y += padding.bottom;
  if (rect.w > 0) {
    rect.w += padding.left + padding.right;
  }
  if (rect.h > 0) {
    rect.h += padding.top + padding.bottom;
  }
  afterUnwrap();
  Group::end();
}

} // namespace dui

#endif // DUI_WRAPPERGROUP_HPP
