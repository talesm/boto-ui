#ifndef DUI_GROUP_HPP_
#define DUI_GROUP_HPP_

#include <string_view>
#include <SDL_rect.h>
#include "State.hpp"

namespace dui {

enum class MouseAction
{
  NONE,   ///< Default status
  GRAB,   ///< The mouse grabbed this element an is holding inside its bounds
  ACTION, ///< The mouse was just released inside its bounds (do something!)
  DRAG,   ///< The mouse had this grabbed, but was moved to outside its bounds
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

  void reset() { caret = {rect.x, rect.y}; }

public:
  Group(Group* parent, std::string_view id, const SDL_Rect& rect)
    : Group(id, rect, parent)
  {}
  ~Group();
  Group(const Group&) = delete;
  Group(Group&& rhs);
  Group& operator=(const Group&) = delete;
  Group& operator=(Group&& rhs);

  void box(SDL_Rect rect, SDL_Color color)
  {
    SDL_assert(state->inFrame);
    SDL_assert(!composingSubgroup);
    rect.x += caret.x;
    rect.y += caret.y;
    state->dList.insert(rect, color, 0);
  }

  void character(const SDL_Point& p, SDL_Color color, char ch)
  {
    SDL_assert(state->inFrame);
    SDL_assert(!composingSubgroup);
    state->dList.insert({caret.x + p.x, caret.y + p.y, 8, 8}, color, ch);
  }

  SDL_Point measure(char ch) { return {8, 8}; }

  void string(SDL_Point p, SDL_Color color, std::string_view text)
  {
    SDL_assert(state->inFrame);
    SDL_assert(!composingSubgroup);

    for (auto ch : text) {
      state->dList.insert({caret.x + p.x, caret.y + p.y, 8, 8}, color, ch);
      p.x += 8;
    }
  }

  SDL_Point measure(std::string_view text) { return {int(8 * text.size()), 8}; }

  MouseAction testMouse(std::string_view id, SDL_Rect r);

  void advance(const SDL_Point& p)
  {
    SDL_assert(state->inFrame);
    SDL_assert(!composingSubgroup);

    caret.y += p.y + 2;
  }
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
    this->rect.x += parent->caret.x;
    this->rect.y += parent->caret.y;
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
      rect.w = caret.x - rect.x;
    }
    if (rect.h == 0) {
      rect.h = caret.y - rect.y;
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
  SDL_assert(state->inFrame);
  SDL_assert(!composingSubgroup);

  r.x += caret.x;
  r.y += caret.y;
  if (state->mGrabbed.empty()) {
    if (state->mLeftPressed && SDL_PointInRect(&state->mPos, &r)) {
      state->mGrabbed = id;
      return MouseAction::GRAB;
    }
    return MouseAction::NONE;
  }
  if (state->mGrabbed != id) {
    return MouseAction::NONE;
  }
  if (state->mLeftPressed) {
    return SDL_PointInRect(&state->mPos, &r) ? MouseAction::GRAB
                                             : MouseAction::DRAG;
  }
  state->mGrabbed.clear();
  return SDL_PointInRect(&state->mPos, &r) ? MouseAction::ACTION
                                           : MouseAction::NONE;
}
} // namespace dui

#endif // DUI_GROUP_HPP_
