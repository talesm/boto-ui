#ifndef DUI_GROUP_HPP_
#define DUI_GROUP_HPP_

#include <string_view>
#include <SDL_rect.h>
#include "State.hpp"

namespace dui {

/**
 * @brief A grouping of widgets
 *
 * This externally viewed as a single widget, so it can be used to create
 * composed widgets
 *
 */
class Group
{
  Group* parent = nullptr;
  std::string_view id;
  SDL_Rect rect;
  SDL_Point caret;
  bool composingSubgroup = false;

protected:
  State* state;

  Group(std::string_view id,
        const SDL_Rect& rect,
        State* state,
        const SDL_Point& caret)
    : id(id)
    , rect(rect)
    , caret(caret)
    , state(state)
  {}

  Group(std::string_view id, const SDL_Rect& rect, Group* parent);

  Group(std::string_view id, const SDL_Rect& rect, State* state)
    : Group(id, rect, state, {rect.x, rect.y})
  {}

  void reset()
  {
    caret = {rect.x, rect.y};
    if (parent) {
      caret.x += parent->caret.x;
      caret.y += parent->caret.y;
    }
  }

public:
  Group(Group* parent, std::string_view id, const SDL_Rect& rect)
    : Group(id, rect, parent)
  {}
  ~Group();
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
  MouseAction testMouse(std::string_view id, SDL_Rect r);

  /**
   * @brief Check if given element is active
   *
   * @param id the id to check
   * @return true
   * @return false
   */
  bool isActive(std::string_view id) const { return state->isActive(id); }

  /// Check if has text to retrieve
  bool hasText() const { return state->hasText(); }

  /// Get last retrieved text
  std::string_view getText() const { return state->getText(); }

  void advance(const SDL_Point& p)
  {
    SDL_assert(!composingSubgroup);
    caret.y += p.y + 2;
  }

  State& getState() const { return *state; }

  SDL_Point getCaret() const { return caret; }

  bool isBlocked() const { return composingSubgroup; }
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
group(Group& parent, std::string_view id, const SDL_Rect& rect)
{
  return {&parent, id, rect};
}

inline Group::Group(std::string_view id, const SDL_Rect& rect, Group* parent)
  : Group(id, rect, parent->state, {rect.x, rect.y})
{
  if (parent) {
    this->parent = parent;
    SDL_assert(!parent->composingSubgroup);
    parent->composingSubgroup = true;
    caret.x += parent->caret.x;
    caret.y += parent->caret.y;
  }
}

inline Group::~Group()
{
  if (parent) {
    SDL_assert(parent->composingSubgroup);
    parent->composingSubgroup = false;
    if (rect.w == 0) {
      rect.w = caret.x - parent->caret.x;
    }
    if (rect.h == 0) {
      rect.h = caret.y - parent->caret.y;
    }
    parent->advance({rect.w, rect.h});
  }
}

inline Group::Group(Group&& rhs)
  : parent(rhs.parent)
  , id(std::move(rhs.id))
  , rect(rhs.rect)
  , caret(rhs.caret)
  , composingSubgroup(rhs.composingSubgroup)
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
Group::testMouse(std::string_view id, SDL_Rect r)
{
  SDL_assert(!composingSubgroup);

  r.x += caret.x;
  r.y += caret.y;
  return state->testMouse(id, r);
}
} // namespace dui

#endif // DUI_GROUP_HPP_
