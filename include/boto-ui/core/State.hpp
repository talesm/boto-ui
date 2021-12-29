#ifndef BOTO_CORE_STATE_HPP_
#define BOTO_CORE_STATE_HPP_

#include <climits>
#include <optional>
#include <string_view>
#include <SDL.h>
#include "DisplayList.hpp"
#include "EventDispatcher.hpp"
#include "Font.hpp"
#include "Theme.hpp"
#include "util/CookieBase.hpp"

namespace boto {

// Forward decl
class ContainerState;
class Container;
class Frame;
class Target;

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
  {
    theme.set<Font>(loadDefaultFont(renderer));
  }

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

  /**
   * @brief Starts a new Frame
   *
   * @return Frame
   */
  Frame frame();

  /**
   * @brief Access event dispatcher
   *
   * @return EventDispatcher&
   */
  EventDispatcher& eventDispatcher() { return dispatcher; }
  const EventDispatcher& eventDispatcher() const { return dispatcher; }

  /**
   * @brief Access to display list
   *
   * @return DisplayList&
   */
  DisplayList& displayList() { return dList; }
  const DisplayList& displayList() const { return dList; }

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
   * @brief Get the last input text
   *
   * To check if the text was for the current element and frame, use checkText()
   * or Group.checkText().
   *
   * @return std::string_view
   */
  std::string_view input() const { return dispatcher.input(); }

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
  SDL_Point pointerPosition() const { return dispatcher.pointerPosition(); }

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

  /// Ticks count
  Uint32 ticks() const { return ticksCount; }

  const Font& getFont() const { return font; }
  void setFont(const Font& f) { font = f; }

  struct FrameGuard
  {
    void operator()(State* state) { state->endFrame(); }
  };

  struct ContainerGuard
  {
    void operator()(State* state) { state->popContainer(); }
  };

  template<class T>
  const auto& styleFor()
  {
    return theme.of<T>();
  }

private:
  void endFrame();
  void popContainer();

  Container container(std::string_view id,
                      SDL_Rect r,
                      const SDL_Point& offset = {},
                      const SDL_Point& endPadding = {},
                      Layout layout = Layout::NONE,
                      int elementSpacing = 0);

  EventTargetState element(std::string_view id,
                           SDL_Rect r,
                           RequestEvent req = RequestEvent::INPUT);

  EventTargetState element(SDL_Rect r, RequestEvent req = RequestEvent::GRAB)
  {
    return element({}, r, req);
  }

  const ContainerState* getTop() const
  {
    if (containers.empty())
      return nullptr;
    return &containers.back();
  }

  friend class Container;
  friend class Target;

  SDL_Renderer* renderer;
  DisplayList dList;
  EventDispatcher dispatcher;

  SDL_Keysym tKeysym;

  Uint32 ticksCount;

  Font font;
  bool inFrame = false;
  std::vector<ContainerState> containers;
  Theme theme;
};

inline void
State::render()
{
  SDL_assert(!isInFrame());
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

} // namespace boto
#include "ContainerState.hpp"
#include "Frame.hpp"

#endif // BOTO_CORE_STATE_HPP_
