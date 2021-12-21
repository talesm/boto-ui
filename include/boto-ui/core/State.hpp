#ifndef BOTO_CORE_STATE_HPP_
#define BOTO_CORE_STATE_HPP_

#include <optional>
#include <string_view>
#include <SDL.h>
#include "Font.hpp"
#include "core/DisplayList.hpp"
#include "core/EventDispatcher.hpp"
#include "util/CookieBase.hpp"

namespace boto {

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

struct ElementState
{
  DisplayList::Clip clip;
  EventDispatcher::EventTarget eventTarget;
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
  void render();

  /**
   * @brief Handle a SDL_Event
   *
   * @param ev event
   */
  void event(SDL_Event& ev);

  // Forward decl
  class Frame;

  Frame frame();

  /**
   * @brief Access event dispatcher
   *
   * @return EventDispatcher&
   */
  EventDispatcher& eventDispatcher() { return dispatcher; }
  const EventDispatcher& eventDispatcher() const { return dispatcher; }

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
   * @param qualifiedId the element id
   * @return true
   * @return false
   */
  bool isActive(std::string_view qualifiedId) const
  {
    return dispatcher.isActive(qualifiedId);
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
  TextAction checkText(std::string_view id) const;

  /**
   * @brief Get the last input text
   *
   * To check if the text was for the current element and frame, use checkText()
   * or Group.checkText().
   *
   * @return std::string_view
   */
  std::string_view lastText() const { return dispatcher.input(); }

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
  SDL_Point lastMousePos() const { return dispatcher.pointerPosition(); }

  /**
   * @brief If true, the state wants the mouse events
   */
  bool wantsMouse() const { return dispatcher.wantsMouse(); }

  /**
   * @brief If true, the state wants the keyboard events
   * @return true
   * @return false
   */
  bool wantsKeyboard() const { return dispatcher.wantsKeyboard(); }

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
  void beginGroup(std::string_view id, SDL_Rect r);
  void endGroup(std::string_view id, const SDL_Rect& r);
  const Font& getFont() const { return font; }
  void setFont(const Font& f) { font = f; }

private:
  void endFrame();

  struct FrameGuard
  {
    void operator()(State* state) { state->endFrame(); }
  };

  SDL_Renderer* renderer;
  DisplayList dList;
  EventDispatcher dispatcher;

  SDL_Keysym tKeysym;

  Uint32 ticksCount;

  Font font;

  std::vector<ElementState> elements;
  bool levelChanged = true;
  bool inFrame = false;
};

class State::Frame : public CookieBase<State, State::FrameGuard>
{
private:
  Frame(State* state)
    : CookieBase(state)
  {}

  friend class State;

public:
  constexpr Frame() = default;

  // TODO remove me
  using CookieBase::get;
};

inline MouseAction
State::checkMouse(std::string_view id, SDL_Rect r)
{
  if (levelChanged) {
    levelChanged = false;
  } else {
    elements.pop_back();
  }
  elements.emplace_back(ElementState{
    dList.clip(r),
    dispatcher.check(RequestEvent::INPUT, r, id),
  });
  auto& elState = elements.back().eventTarget.state();
  switch (elState.event) {
  case Event::GRAB:
    return MouseAction::GRAB;
  case Event::ACTION:
    return MouseAction::ACTION;
  case Event::CANCEL:
    return MouseAction::CANCEL;
  default:
    if (!elState.status.test(Status::GRABBED)) {
      return MouseAction::NONE;
    }
    return elState.status.test(Status::HOVERED) ? MouseAction::HOLD
                                                : MouseAction::DRAG;
  }
}

inline TextAction
State::checkText(std::string_view id) const
{
  auto& state = elements.back().eventTarget.state();
  switch (state.event) {
  case Event::INPUT:
    return TextAction::INPUT;
  case Event::END_LINE:
  case Event::SPACE:
  case Event::BACKSPACE:
    return TextAction::KEYDOWN;
  default:
    return TextAction::NONE;
  }
}

inline void
State::beginGroup(std::string_view id, SDL_Rect r)
{
  if (!levelChanged) {
    elements.pop_back();
  }
  if (id.empty()) {
    elements.emplace_back(ElementState{
      dList.clip(r),
      dispatcher.check(RequestEvent::HOVER, r, id),
    });
  } else {
    if (r.w == 0) {
      r.w = 0xFFFF;
    }
    if (r.h == 0) {
      r.h = 0xFFFF;
    }
    elements.emplace_back(ElementState{
      dList.clip(r),
      dispatcher.check(RequestEvent::INPUT, r, id),
    });
  }
  levelChanged = true;
}

inline void
State::endGroup(std::string_view id, const SDL_Rect& r)
{
  if (!levelChanged) {
    elements.pop_back();
  }
  dispatcher.shrink(r.w, r.h);
  elements.pop_back();
  levelChanged = true;
}

inline void
State::render()
{
  SDL_assert(elements.empty());
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

inline void
State::event(SDL_Event& ev)
{
  switch (ev.type) {
  case SDL_MOUSEBUTTONDOWN:
    dispatcher.movePointer({ev.button.x, ev.button.y});
    dispatcher.pressPointer(ev.button.button - SDL_BUTTON_LEFT);
    break;
  case SDL_MOUSEMOTION:
    dispatcher.movePointer({ev.motion.x, ev.motion.y});
    break;
  case SDL_MOUSEBUTTONUP:
    dispatcher.movePointer({ev.button.x, ev.button.y});
    dispatcher.releasePointer(ev.button.button - SDL_BUTTON_LEFT);
    break;
  case SDL_TEXTINPUT:
    dispatcher.input(ev.text.text);
    break;
  case SDL_KEYDOWN:
    switch (ev.key.keysym.sym) {
    case SDLK_RETURN:
    case SDLK_KP_ENTER:
      dispatcher.command(Command::ENTER);
      break;
    case SDLK_SPACE:
      dispatcher.command(Command::SPACE);
      break;
    case SDLK_ESCAPE:
      dispatcher.command(Command::ESCAPE);
      break;
    case SDLK_BACKSPACE:
      dispatcher.command(Command::BACKSPACE);
      break;
    default:
      break;
    }
    tKeysym = ev.key.keysym;
  default:
    break;
  }
}

inline State::Frame
State::frame()
{
  SDL_assert(inFrame == false);
  ticksCount = SDL_GetTicks();
  levelChanged = true;
  inFrame = true;
  dList.clear();
  return {this};
}

inline void
State::endFrame()
{
  SDL_assert(inFrame == true);
  tKeysym = {};
  dispatcher.reset();
  inFrame = false;
}
} // namespace boto

#endif // BOTO_CORE_STATE_HPP_
