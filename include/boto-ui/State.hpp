#ifndef BOTO_STATE_HPP_
#define BOTO_STATE_HPP_

#include <string>
#include <SDL.h>
#include "Font.hpp"
#include "core/DisplayList.hpp"

namespace boto {

constexpr char groupNameSeparator = '/';

/**
 * @brief The mouse action and status for a element in a frame
 *
 */
enum class MouseAction
{
  NONE,   ///< Default status
  GRAB,   ///< The mouse was just grabbed at this element
  HOLD,   ///< The mouse grabbed this element an is holding inside its bounds
  ACTION, ///< The mouse was just released inside its bounds (do something!)
  CANCEL, ///< The mouse was just released outside its bounds
  DRAG,   ///< The mouse had this grabbed, but was moved to outside its bounds
};

/**
 * @brief The text action and status for a element in a frame
 *
 */
enum class TextAction
{
  NONE,    ///< Default status
  INPUT,   ///< text input
  KEYDOWN, ///< erased last character
};

/**
 * @brief Stores the ui state
 *
 * This should be the only state that needs to be preserved between the program
 * loops.
 *
 */
class State
{
  bool inFrame = false;
  SDL_Renderer* renderer;
  DisplayList dList;

  SDL_Point mPos;
  bool mLeftPressed = false;
  std::string eGrabbed;
  bool mHovering = false;
  bool mGrabbing = false;
  bool mReleasing = false;
  std::string eActive;
  char tBuffer[SDL_TEXTINPUTEVENT_TEXT_SIZE];
  SDL_Keysym tKeysym;
  bool tChanged = false;
  TextAction tAction = TextAction::NONE;

  std::string group;
  bool gGrabbed = false;
  bool gActive = false;

  Uint32 ticksCount;

  Font font;

  std::vector<DisplayList::Clip> clips;

public:
  /// Ctor
  State(SDL_Renderer* renderer)
    : renderer(renderer)
    , font(loadDefaultFont(renderer))
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
    dList.visit([&](const DisplayListItem& item) {
      SDL_Color c = item.color;
      switch (item.action) {
      case DisplayListAction::RESET_CLIP:
        SDL_RenderSetClipRect(renderer, nullptr);
        break;
      case DisplayListAction::SET_CLIP:
        SDL_RenderSetClipRect(renderer, &item.rect);
        break;
      case DisplayListAction::COLOR_BOX:
        SDL_SetRenderDrawBlendMode(renderer, item.mode);
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        SDL_RenderFillRect(renderer, &item.rect);
        break;
      case DisplayListAction::TEXTURE_BOX:
        SDL_SetTextureAlphaMod(item.texture, c.a);
        SDL_SetTextureBlendMode(item.texture, item.mode);
        SDL_SetTextureColorMod(item.texture, c.r, c.g, c.b);
        SDL_RenderCopy(renderer, item.texture, &item.srcRect, &item.rect);
        break;
      }
    });
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
  std::string_view lastText() const { return {tBuffer}; }

  /**
   * @brief Get the last key
   *
   * To check if the text was for the current element and frame, use checkText()
   * or Group.checkText().
   *
   * @return SDL_Keysym
   */
  SDL_Keysym lastKeyDown() const { return tKeysym; }

  /**
   * @brief Last mouse position
   *
   * @return SDL_Point the last mouse pos
   */
  SDL_Point lastMousePos() const { return mPos; }

  /**
   * @brief If true, the state wants the mouse events
   */
  bool wantsMouse() const { return mHovering || !eGrabbed.empty(); }

  /**
   * @brief If true, the state wants the keyboard events
   * @return true
   * @return false
   */
  bool wantsKeyboard() const { return !eActive.empty(); }

  /**
   * @brief Add the given item Shape to display list
   *
   * @param item
   */
  void display(const SDL_Rect& r, SDL_Color c)
  {
    dList.push(r, c, SDL_BLENDMODE_BLEND);
  }
  void display(const SDL_Rect& r, SDL_Texture* texture, const SDL_Rect& srcRect)
  {
    display(r, texture, srcRect, {255, 255, 255, 255});
  }
  void display(const SDL_Rect& r,
               SDL_Texture* texture,
               const SDL_Rect& srcRect,
               SDL_Color c)
  {
    dList.push(r, c, SDL_BLENDMODE_BLEND, texture, srcRect);
  }

  /// Ticks count
  Uint32 ticks() const { return ticksCount; }

  // These are experimental and should not be used
  void beginGroup(std::string_view id, const SDL_Rect& r);
  void endGroup(std::string_view id, const SDL_Rect& r);
  const Font& getFont() const { return font; }
  void setFont(const Font& f) { font = f; }

private:
  void beginFrame()
  {
    SDL_assert(inFrame == false);
    inFrame = true;
    dList.clear();
    mHovering = false;
    ticksCount = SDL_GetTicks();
  }

  void endFrame()
  {
    SDL_assert(inFrame == true);
    inFrame = false;
    tChanged = false;
    mGrabbing = false;
    if (mReleasing) {
      eGrabbed.clear();
      mReleasing = false;
    }
  }

  bool isSameGroupId(std::string_view qualifiedId, std::string_view id) const;

  friend class Frame;
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
    if (SDL_PointInRect(&mPos, &r) && !mGrabbing) {
      eGrabbed = group;
      eGrabbed += groupNameSeparator;
      eGrabbed += id;
      eActive = group;
      eActive += groupNameSeparator;
      eActive += id;
      gGrabbed = true;
      gActive = true;
      mGrabbing = true;
      return MouseAction::GRAB;
    }
    if (isSameGroupId(eActive, id)) {
      eActive.clear();
    }
    return MouseAction::NONE;
  }
  if (!isSameGroupId(eGrabbed, id)) {
    return MouseAction::NONE;
  }
  gGrabbed = true;
  if (mLeftPressed) {
    if (mGrabbing) {
      return MouseAction::GRAB;
    }
    if (!SDL_PointInRect(&mPos, &r)) {
      return MouseAction::DRAG;
    }
    return MouseAction::HOLD;
  }
  mReleasing = true;
  if (!SDL_PointInRect(&mPos, &r)) {
    return MouseAction::CANCEL;
  }
  return MouseAction::ACTION;
}

inline void
State::beginGroup(std::string_view id, const SDL_Rect& r)
{
  clips.emplace_back(dList.clip(r));
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
State::endGroup(std::string_view id, const SDL_Rect& r)
{
  if (id.empty()) {
    // Nothing to do
  } else if (id.size() >= group.size()) {
    // A top level group
    SDL_assert(group == id);
    group.clear();
    gActive = gGrabbed = false;
    if (!mHovering && SDL_PointInRect(&mPos, &r)) {
      mHovering = true;
    }
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
  clips.pop_back();
}

inline void
State::event(SDL_Event& ev)
{
  if (ev.type == SDL_MOUSEBUTTONDOWN) {
    mPos = {ev.button.x, ev.button.y};
    if (ev.button.button == SDL_BUTTON_LEFT) {
      mLeftPressed = true;
    }
  } else if (ev.type == SDL_MOUSEMOTION) {
    if (!(eGrabbed.empty() && mLeftPressed)) {
      mPos = {ev.motion.x, ev.motion.y};
    }
  } else if (ev.type == SDL_MOUSEBUTTONUP) {
    mPos = {ev.button.x, ev.button.y};
    mLeftPressed = false;
  } else if (ev.type == SDL_TEXTINPUT) {
    if (eActive.empty()) {
      return;
    }
    for (int i = 0, j = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; ++i) {
      tBuffer[j] = ev.text.text[i];
      if (tBuffer[j] == 0) {
        break;
      }
      // Magic handling of utf8
      if ((tBuffer[j] & 0xc0) == 0x80) {
        continue;
      }
      if ((tBuffer[j] & 0x80) != 0) {
        tBuffer[j] = '\x0f'; // This is valid on our particular font
      }
      ++j;
    }
    tChanged = true;
    tAction = TextAction::INPUT;
  } else if (ev.type == SDL_KEYDOWN) {
    if (!tChanged) {
      tKeysym = ev.key.keysym;
      tChanged = true;
      tAction = TextAction::KEYDOWN;
    }
  }
}
} // namespace boto

#endif // BOTO_STATE_HPP_
