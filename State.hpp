#ifndef DUI_STATE_HPP_
#define DUI_STATE_HPP_

#include <SDL_events.h>
#include "DisplayList.hpp"

namespace dui {

constexpr char groupNameSeparator = '/';

/**
 * @brief The mouse action and status for a element in a frame
 *
 */
enum class MouseAction
{
  NONE,   ///< Default status
  GRAB,   ///< The mouse grabbed this element an is holding inside its bounds
  ACTION, ///< The mouse was just released inside its bounds (do something!)
  DRAG,   ///< The mouse had this grabbed, but was moved to outside its bounds
};

/**
 * @brief The text action and status for a element in a frame
 *
 */
enum class TextAction
{
  NONE,      ///< Default status
  INPUT,     ///< text input
  BACKSPACE, ///< erased last character
};

class State
{
private:
  bool inFrame = false;
  SDL_Renderer* renderer;
  DisplayList dList;

  SDL_Point mPos;
  bool mLeftPressed = false;
  std::string eGrabbed;
  std::string eActive;
  char tBuffer[SDL_TEXTINPUTEVENT_TEXT_SIZE];
  bool tChanged = false;
  TextAction tAction = TextAction::NONE;

  std::string group;
  bool gGrabbed = false;
  bool gActive = false;

public:
  State(SDL_Renderer* renderer)
    : renderer(renderer)
  {}

  /**
   * @brief Render the ui
   *
   * This must not be in frame. You might want to call Frame.render() that
   * ensures the frame ended correctly.
   *
   */
  void render()
  {
    SDL_assert(!inFrame);
    dList.render(renderer);
  }

  /**
   * @brief Handle a SDL_Event
   *
   * @param ev event
   */
  void event(SDL_Event& ev);

  /**
   * @brief If a frame is in progress
   *
   * You shouldn't send events nor render during this
   *
   * @return true
   * @return false
   */
  bool isInFrame() const { return inFrame; }

  /**
   * @brief Check if the element was activated
   *
   * The activation is done when you click (mouse button down) with the mouse on
   * the element and last until you click outside of it.
   *
   * Other ways of activate might exist.
   *
   * @param id the element id
   * @return true
   * @return false
   */
  bool isActive(std::string_view id) const
  {
    return isSameGroupId(eActive, id);
  }

  /**
   * @brief Check the mouse action/status for element in this frame
   *
   * @param id element id
   * @param r the element global rect (Use Group.checkMouse() for local rect)
   * @return MouseAction
   */
  MouseAction checkMouse(std::string_view id, SDL_Rect r);

  /**
   * @brief Check the text action/status for element in this frame
   *
   * @param id the element id
   * @return TextAction
   */
  TextAction checkText(std::string_view id) const
  {
    if (!tChanged || !isSameGroupId(eActive, id)) {
      return TextAction::NONE;
    }
    return tAction;
  }

  /**
   * @brief Get the last input text
   *
   * To check if the text was for the current element and frame, use checkText()
   * or Group.checkText().
   *
   * @return std::string_view
   */
  std::string_view getText() const { return {tBuffer}; }

  /**
   * @brief Add the given item Shape to display list
   *
   * @param item
   */
  void display(const Shape& item) { dList.insert(item); }

  /**
   * @brief Context for frames
   *
   * Ignore this unless you are developing this library
   */
  class Context
  {
  private:
    State* state;
    Context(State* state)
      : state(state)
    {
      state->beginFrame();
    }
    friend class State;

  public:
    ~Context() { unlockFrame(); }

    /**
     * @brief Ends the lifetime of this object and unlock the state
     *
     */
    void unlockFrame()
    {
      if (state) {
        state->endFrame();
        state = nullptr;
      }
    }
    Context(const Context&) = delete;
    Context(Context&& rhs) { std::swap(state, rhs.state); }
    Context& operator=(Context rhs)
    {
      std::swap(state, rhs.state);
      return *this;
    }

    /// Pushes a group. Must be paired with a proper popGroup
    void pushGroup(std::string_view id, const SDL_Rect&)
    {
      state->dList.popClip();
      state->beginGroup(id);
    }
    /// Pops a group, Must be have been a pushGroup with same id before
    void popGroup(std::string_view id, const SDL_Rect& r)
    {
      state->endGroup(id);
      state->dList.pushClip(r);
    }
  };

  /**
   * @brief Lock the state, starts a frame
   *
   * The frame is going to be active until the Context lifetime ends
   *
   * You probably want to use a Frame instead of calling this directly.
   *
   * @return Context
   */
  Context lockFrame() { return Context{this}; }

private:
  void beginFrame()
  {
    SDL_assert(inFrame == false);
    inFrame = true;
    dList.clear();
  }

  void endFrame()
  {
    SDL_assert(inFrame == true);
    inFrame = false;
    tChanged = false;
  }

  void beginGroup(std::string_view id);
  void endGroup(std::string_view id);
  bool isSameGroupId(std::string_view qualifiedId, std::string_view id) const;
};

inline bool
State::isSameGroupId(std::string_view qualifiedId, std::string_view id) const
{
  auto groupSize = group.size();
  if (qualifiedId.size() < groupSize + 1) {
    return false;
  }
  if (qualifiedId.substr(0, groupSize) != group ||
      qualifiedId[groupSize] != groupNameSeparator ||
      qualifiedId.substr(groupSize + 1) != id) {
    return false;
  }
  return true;
}

inline MouseAction
State::checkMouse(std::string_view id, SDL_Rect r)
{
  SDL_assert(inFrame);
  if (eGrabbed.empty()) {
    if (!mLeftPressed) {
      return MouseAction::NONE;
    }
    if (SDL_PointInRect(&mPos, &r)) {
      eGrabbed = group;
      eGrabbed += groupNameSeparator;
      eGrabbed += id;
      eActive = group;
      eActive += groupNameSeparator;
      eActive += id;
      gGrabbed = true;
      gActive = true;
      return MouseAction::GRAB;
    }
    if (isSameGroupId(eActive, id)) {
      gActive = false;
      eActive.clear();
    }
    return MouseAction::NONE;
  }
  if (!isSameGroupId(eGrabbed, id)) {
    return MouseAction::NONE;
  }
  if (mLeftPressed) {
    return SDL_PointInRect(&mPos, &r) ? MouseAction::GRAB : MouseAction::DRAG;
  }
  eGrabbed.clear();
  gGrabbed = true;
  if (!SDL_PointInRect(&mPos, &r)) {
    return MouseAction::NONE;
  }
  return MouseAction::ACTION;
}

inline void
State::beginGroup(std::string_view id)
{
  if (id.empty()) {
    return;
  }
  auto idSize = id.size();
  auto groupSize = group.size();
  if (groupSize > 0) {
    group += groupNameSeparator;
    if (!gGrabbed || eGrabbed.size() <= idSize + groupSize + 1 ||
        std::string_view{eGrabbed}.substr(groupSize, idSize + 1) != id ||
        eGrabbed[groupSize + idSize + 1] != groupNameSeparator) {
      gGrabbed = false;
    }
    if (!gActive || eActive.size() <= idSize + groupSize + 1 ||
        std::string_view{eActive}.substr(groupSize, idSize + 1) != id ||
        eActive[groupSize + idSize + 1] != groupNameSeparator) {
      gActive = false;
    }
  } else {
    if (eGrabbed.size() <= idSize ||
        std::string_view{eGrabbed}.substr(0, idSize) != id ||
        eGrabbed[idSize] != groupNameSeparator) {
      gGrabbed = false;
    } else {
      gGrabbed = true;
    }
    if (eActive.size() <= idSize ||
        std::string_view{eActive}.substr(0, idSize) != id ||
        eActive[idSize] != groupNameSeparator) {
      gActive = false;
    } else {
      gActive = true;
    }
  }
  group += id;
}

inline void
State::endGroup(std::string_view id)
{
  if (id.empty()) {
    return;
  }
  if (id.size() >= group.size()) {
    SDL_assert(group == id);
    group.clear();
    gActive = gGrabbed = false;
  } else {
    auto groupSize = group.size();
    auto nextSize = groupSize - id.size() - 1;
    SDL_assert(group[nextSize] == groupNameSeparator);
    SDL_assert(std::string_view{group}.substr(nextSize + 1) == id);

    group.erase(group.begin() + nextSize, group.end());
    if (!gGrabbed && eGrabbed.size() > nextSize &&
        std::string_view{eGrabbed}.substr(0, nextSize) == group &&
        eGrabbed[nextSize] == groupNameSeparator) {
      gGrabbed = true;
    }
    if (!gActive && eActive.size() > nextSize &&
        std::string_view{eActive}.substr(0, nextSize) == group &&
        eActive[nextSize] == groupNameSeparator) {
      gActive = true;
    }
  }
}

inline void
State::event(SDL_Event& ev)
{
  if (ev.type == SDL_MOUSEBUTTONDOWN) {
    mPos = {ev.button.x, ev.button.y};
    if (ev.button.button == SDL_BUTTON_LEFT) {
      mLeftPressed = true;
    }
  } else if (ev.type == SDL_MOUSEBUTTONUP) {
    mPos = {ev.button.x, ev.button.y};
    mLeftPressed = false;
  } else if (ev.type == SDL_TEXTINPUT) {
    if (eActive.empty()) {
      return;
    }
    for (int i = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; ++i) {
      tBuffer[i] = ev.text.text[i];
      if (tBuffer[i] == 0) {
        break;
      }
    }
    tChanged = true;
    tAction = TextAction::INPUT;
  } else if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_BACKSPACE) {
      tChanged = true;
      tAction = TextAction::BACKSPACE;
    }
  }
}
} // namespace dui

#endif // DUI_STATE_HPP_
