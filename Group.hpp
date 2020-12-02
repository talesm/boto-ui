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
  bool locked = false;
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

  // Parent ctor
  Group(Group* parent,
        State* state,
        std::string_view id,
        const SDL_Rect& rect,
        const SDL_Point& caret,
        Layout layout);

protected:
  SDL_Rect rect;
  SDL_Point topLeft;
  SDL_Point bottomRight;
  Layout layout;

  /**
   * @brief Construct a new root Group
   *
   * @param state the state the group belongs
   * @param id the group id
   * @param rect its size
   * @param layout its layout
   */
  Group(std::string_view id, const SDL_Rect& rect, State* state, Layout layout)
    : Group(nullptr, state, id, rect, {rect.x, rect.y}, layout)
  {}

  virtual void afterLock(int depth,
                         std::string_view id,
                         const SDL_Rect& initialRect)
  {}

  virtual void beforeUnlock(int depth,
                            std::string_view id,
                            const SDL_Rect& finalRect)
  {}

public:
  /**
   * @brief Construct a new branch Group object
   *
   * You probably want to use group() instead of this.
   *
   * @param parent the parent group. MUST NOT BE NULL
   * @param id the group id
   * @param rect the rect. Either w or h being 0 means it will auto size
   * @param layout the layout
   */
  Group(Group* parent,
        std::string_view id,
        const SDL_Rect& rect,
        Layout layout);

  virtual ~Group() { SDL_assert(!valid()); }
  Group(const Group&) = delete;
  Group(Group&& rhs);
  Group& operator=(Group rhs);

  /**
   * @brief Check the mouse action/status for element in this group
   *
   * @param id element id
   * @param r the element local rect (Use State.checkMouse() for global rect)
   * @return MouseAction
   */
  MouseAction checkMouse(std::string_view id, SDL_Rect r);

  /**
   * @brief Check if given contained element is active
   *
   * @param id the id to check
   * @return true
   * @return false
   */
  bool isActive(std::string_view id) const { return state->isActive(id); }

  /**
   * @brief Check the text action/status for element in this group
   *
   * @param id the element id
   * @return TextAction
   */
  TextAction checkText(std::string_view id) const
  {
    return state->checkText(id);
  }

  /**
   * @brief Get the last input text
   *
   * To check if the text was for the current element and frame, use checkText()
   * or Frame.checkText().
   *
   * @return std::string_view
   */
  std::string_view getText() const { return state->getText(); }

  /**
   * @brief Advances the caret with the given offset
   *
   * @param p a point where x and y are the horizontal and vertical offsets,
   * respectively.
   *
   * The new caret will also depend on the layout:
   * - VERTICAL layout means only the y is updated so elements are positioned
   * vertically;
   * - HORIZONTAL layout means only the x is updated so elements are positioned
   * horizontally;
   * - NONE layout means none are updated and elements all begin in the same
   * position;
   *
   * Keep in mind that elements have their own offset (their rect.x and rect.y),
   * that is added to the caret ones.
   */
  void advance(const SDL_Point& p);

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

  bool valid() const { return state != nullptr; }

  operator bool() const { return valid(); }

  int width() const { return rect.w ? rect.w : bottomRight.x - topLeft.x; }
  int height() const { return rect.h ? rect.h : bottomRight.y - topLeft.y; }

  void end();
};

/**
 * @brief Create group
 *
 * @param target the parent group or frame
 * @param id the group id
 * @param rect the group dimensions
 * @return Group
 */
inline Group
group(Group& target,
      std::string_view id,
      const SDL_Rect& rect,
      Layout layout = Layout::VERTICAL)
{
  return {&target, id, rect, layout};
}

namespace style {
/// Default text style
constexpr SDL_Color TEXT{45, 72, 106, 255};
}

/**
 * @brief adds a box element to target
 *
 * @param target the parent group or frame
 * @param rect the box local position and size
 * @param c the box color
 */
inline void
box(Group& target, SDL_Rect rect, SDL_Color c)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  target.advance({rect.x + rect.w, rect.y + rect.h});
  auto caret = target.getCaret();
  rect.x += caret.x;
  rect.y += caret.y;
  state.display(Shape::Box(rect, c));
}

// Border style
struct BorderedBoxStyle
{
  SDL_Color center;
  SDL_Color left;
  SDL_Color top;
  SDL_Color right;
  SDL_Color bottom;
};

// A box with colored border
inline void
borderedBox(Group& target,
            std::string_view id,
            const SDL_Rect& r,
            const BorderedBoxStyle& style)
{
  auto c = style.center;
  auto e = style.right;
  auto n = style.top;
  auto w = style.left;
  auto s = style.bottom;
  auto g = group(target, id, {0}, Layout::NONE);
  box(g, {r.x + 1, r.y, r.w - 2, 1}, {n.r, n.g, n.b, n.a});
  box(g, {r.x, r.y + 1, 1, r.h - 2}, {w.r, w.g, w.b, w.a});
  box(g, {r.x + 1, r.y + r.h - 2 + 1, r.w - 2, 1}, {s.r, s.g, s.b, s.a});
  box(g, {r.x + r.w - 2 + 1, r.y + 1, 1, r.h - 2}, {e.r, e.g, e.b, e.a});
  box(g, {r.x + 1, r.y + 1, r.w - 2, r.h - 2}, {c.r, c.g, c.b, c.a});
  g.end();
}

/// Measure the given character
SDL_Point
measure(char ch)
{
  return {8, 8};
}

/// Measure the given text
SDL_Point
measure(std::string_view text)
{
  return {int(8 * text.size()), 8};
}

/**
 * @brief Adds a character element
 *
 * @param target the parent group or frame
 * @param ch the character
 * @param p the position
 * @param c the color (style::TEXT by default)
 */
inline void
character(Group& target, char ch, const SDL_Point& p, SDL_Color c = style::TEXT)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  target.advance({p.x + 8, p.y + 8});
  auto caret = target.getCaret();
  state.display(Shape::Character({caret.x + p.x, caret.y + p.y}, c, ch));
}

/**
 * @brief Adds a text element
 *
 * @param target the parent group or frame
 * @param str the text
 * @param p the position
 * @param c the color (style::TEXT by default)
 */
inline void
text(Group& target,
     std::string_view str,
     SDL_Point p,
     SDL_Color c = style::TEXT)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  target.advance({p.x + 8 * int(str.size()), p.y + 8});
  auto caret = target.getCaret();
  for (auto ch : str) {
    state.display(Shape::Character({caret.x + p.x, caret.y + p.y}, c, ch));
    p.x += 8;
  }
}

inline Group::Group(Group* parent,
                    State* state,
                    std::string_view id,
                    const SDL_Rect& rect,
                    const SDL_Point& caret,
                    Layout layout)
  : parent(parent)
  , id(id)
  , state(state)
  , rect(rect)
  , topLeft(caret)
  , bottomRight(caret)
  , layout(layout)
{}

inline Group::Group(Group* parent,
                    std::string_view id,
                    const SDL_Rect& rect,
                    Layout layout)
  : Group(parent, parent->state, id, rect, parent->getCaret(), layout)
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
    }
    if (rect.h == 0) {
      rect.h = bottomRight.y - topLeft.y;
    }
    parent->unlock(id, rect);
    parent->advance({rect.x + rect.w, rect.y + rect.h});
    parent = nullptr;
  }
  state = nullptr;
}

inline Group::Group(Group&& rhs)
  : parent(rhs.parent)
  , id(std::move(rhs.id))
  , locked(rhs.locked)
  , state(rhs.state)
  , rect(rhs.rect)
  , topLeft(rhs.topLeft)
  , bottomRight(rhs.bottomRight)
  , layout(rhs.layout)
{
  rhs.state = nullptr;
  rhs.parent = nullptr;
}

inline Group&
Group::operator=(Group rhs)
{
  SDL_assert(!valid());
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

inline void
Group::advance(const SDL_Point& p)
{
  SDL_assert(!locked);
  if (layout == Layout::VERTICAL) {
    bottomRight.x = std::max(p.x + topLeft.x, bottomRight.x);
    bottomRight.y += p.y;
  } else if (layout == Layout::HORIZONTAL) {
    bottomRight.x += p.x;
    bottomRight.y = std::max(p.y + topLeft.y, bottomRight.y);
  } else {
    bottomRight.x = std::max(p.x + topLeft.x, bottomRight.x);
    bottomRight.y = std::max(p.y + topLeft.y, bottomRight.y);
  }
}
} // namespace dui

#endif // DUI_GROUP_HPP_
