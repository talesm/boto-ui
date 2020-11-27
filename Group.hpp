#ifndef DUI_GROUP_HPP_
#define DUI_GROUP_HPP_

#include <string_view>
#include <SDL_rect.h>
#include "State.hpp"

namespace dui {

enum class Layout : Uint8
{
  NONE,
  VERTICAL,
  HORIZONTAL,
};

/**
 * @brief A grouping of widgets
 *
 * This externally viewed as a single widget, so it can be used to create
 * composed elements
 *
 */
class Group
{
  Group* parent = nullptr;
  std::string_view id;
  SDL_Rect rect;
  SDL_Point topLeft;
  SDL_Point bottomRight;
  bool locked = false;
  Layout layout;
  State* state;

  void lock(std::string_view id, SDL_Rect r)
  {
    SDL_assert(!locked);
    locked = true;
    auto caret = getCaret();
    r.x += caret.x;
    r.y += caret.y;
    notifyLock(0, id, r);
  }

  void unlock(std::string_view id, SDL_Rect r)
  {
    SDL_assert(locked);
    auto caret = getCaret();
    r.x += caret.x;
    r.y += caret.y;
    notifyUnlock(0, id, r);
    locked = false;
  }

  void notifyLock(int depth, std::string_view id, const SDL_Rect& r)
  {
    if (parent) {
      parent->notifyLock(depth + 1, id, r);
    }
    afterLock(depth, id, r);
  }

  void notifyUnlock(int depth, std::string_view id, const SDL_Rect& r)
  {
    beforeUnlock(depth, id, r);
    if (parent) {
      parent->notifyUnlock(depth + 1, id, r);
    }
  }

protected:
  Group(std::string_view id,
        const SDL_Rect& rect,
        State* state,
        const SDL_Point& caret,
        Layout layout)
    : id(id)
    , rect(rect)
    , topLeft(caret)
    , bottomRight(caret)
    , layout(layout)
    , state(state)
  {}

  Group(std::string_view id,
        const SDL_Rect& rect,
        Group* parent,
        Layout layout);

  Group(std::string_view id, const SDL_Rect& rect, State* state, Layout layout)
    : Group(id, rect, state, {rect.x, rect.y}, layout)
  {}

  virtual void afterLock(int depth,
                         std::string_view id,
                         const SDL_Rect& initialRect)
  {}

  virtual void beforeUnlock(int depth,
                            std::string_view id,
                            const SDL_Rect& finalRect)
  {}

  void end();

public:
  Group(Group* parent, std::string_view id, const SDL_Rect& rect, Layout layout)
    : Group(id, rect, parent, layout)
  {}
  virtual ~Group() { end(); }
  Group(const Group&) = delete;
  Group(Group&& rhs);
  Group& operator=(const Group&) = delete;
  Group& operator=(Group&& rhs);

  /**
   * @brief Check for mouse actions
   *
   * @param id
   * @param r
   * @return MouseAction
   */
  MouseAction checkMouse(std::string_view id, SDL_Rect r);

  /**
   * @brief Check if given element is active
   *
   * @param id the id to check
   * @return true
   * @return false
   */
  bool isActive(std::string_view id) const { return state->isActive(id); }

  /// Check if has text to retrieve
  TextAction checkText(std::string_view id) const
  {
    return state->checkText(id);
  }

  /// Get last retrieved text
  std::string_view getText() const { return state->getText(); }

  void advance(const SDL_Point& p)
  {
    SDL_assert(!locked);
    if (layout == Layout::VERTICAL) {
      bottomRight.x = std::max(p.x + topLeft.x, bottomRight.x);
      bottomRight.y += p.y + 2;
    } else if (layout == Layout::HORIZONTAL) {
      bottomRight.x += p.x + 2;
      bottomRight.y = std::max(p.y + topLeft.y, bottomRight.y);
    } else {
      bottomRight.x = std::max(p.x + topLeft.x, bottomRight.x);
      bottomRight.y = std::max(p.y + topLeft.y, bottomRight.y);
    }
  }

  State& getState() const { return *state; }

  SDL_Point getCaret() const
  {
    auto caret = topLeft;
    if (layout == Layout::VERTICAL) {
      caret.y = bottomRight.y;
    } else if (layout == Layout::HORIZONTAL) {
      caret.x = bottomRight.x;
    }
    return caret;
  }

  bool isLocked() const { return locked; }

  operator bool() const { return state != nullptr; }
};

/**
 * @brief Create group
 *
 * @param parent the parent group or frame
 * @param id the group id
 * @param rect the group dimensions
 * @return Group
 */
inline Group
group(Group& parent,
      std::string_view id,
      const SDL_Rect& rect,
      Layout layout = Layout::VERTICAL)
{
  return {&parent, id, rect, layout};
}

inline Group::Group(std::string_view id,
                    const SDL_Rect& rect,
                    Group* parent,
                    Layout layout)
  : Group(id, rect, parent->state, parent->getCaret(), layout)
{
  this->parent = parent;
  parent->lock(id, rect);
  topLeft.x += rect.x;
  topLeft.y += rect.y;
  bottomRight.x += rect.x;
  bottomRight.y += rect.y;
}

inline void
Group::end()
{
  if (parent) {
    if (rect.w == 0) {
      rect.w = bottomRight.x - topLeft.x;
      rect.h = bottomRight.y - topLeft.y;
    }
    parent->unlock(id, rect);
    parent->advance({rect.x + rect.w, rect.y + rect.h});
    parent = nullptr;
  }
}

inline Group::Group(Group&& rhs)
  : parent(rhs.parent)
  , id(std::move(rhs.id))
  , rect(rhs.rect)
  , topLeft(rhs.topLeft)
  , bottomRight(rhs.bottomRight)
  , locked(rhs.locked)
  , layout(rhs.layout)
  , state(rhs.state)
{
  rhs.state = nullptr;
  rhs.parent = nullptr;
}

inline Group&
Group::operator=(Group&& rhs)
{
  this->~Group();
  new (this) Group(std::move(rhs));
  return *this;
}

inline MouseAction
Group::checkMouse(std::string_view id, SDL_Rect r)
{
  SDL_assert(!locked);
  SDL_Point caret = getCaret();
  r.x += caret.x;
  r.y += caret.y;
  return state->checkMouse(id, r);
}
} // namespace dui

#endif // DUI_GROUP_HPP_
