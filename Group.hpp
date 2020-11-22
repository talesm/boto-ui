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
 * composed widgets
 *
 */
class Group
{
  Group* parent = nullptr;
  std::string_view id;
  SDL_Rect rect;
  SDL_Point topLeft;
  SDL_Point bottomRight;
  bool composingSubgroup = false;
  Layout layout;

protected:
  State* state;

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

public:
  Group(Group* parent, std::string_view id, const SDL_Rect& rect, Layout layout)
    : Group(id, rect, parent, layout)
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
  SDL_assert(!parent->composingSubgroup);
  parent->composingSubgroup = true;
  topLeft.x += rect.x;
  topLeft.y += rect.y;
  bottomRight.x += rect.x;
  bottomRight.y += rect.y;
}

inline Group::~Group()
{
  if (parent) {
    SDL_assert(parent->composingSubgroup);
    parent->composingSubgroup = false;
    auto c = parent->getCaret();
    SDL_Point adv;
    adv.x = rect.w ? rect.w + rect.x : bottomRight.x - c.x;
    adv.y = rect.h ? rect.h + rect.y : bottomRight.y - c.y;
    parent->advance(adv);
  }
}

inline Group::Group(Group&& rhs)
  : parent(rhs.parent)
  , id(std::move(rhs.id))
  , rect(rhs.rect)
  , topLeft(rhs.topLeft)
  , bottomRight(rhs.bottomRight)
  , composingSubgroup(rhs.composingSubgroup)
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
Group::testMouse(std::string_view id, SDL_Rect r)
{
  SDL_assert(!composingSubgroup);
  SDL_Point caret = getCaret();
  r.x += caret.x;
  r.y += caret.y;
  return state->testMouse(id, r);
}
} // namespace dui

#endif // DUI_GROUP_HPP_
