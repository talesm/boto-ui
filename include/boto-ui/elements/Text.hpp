#ifndef BOTO_ELEMENTS_TEXT_HPP_
#define BOTO_ELEMENTS_TEXT_HPP_

#include <SDL.h>
#include "Group.hpp"
#include "Theme.hpp"
#include "presenters/TextPresenter.hpp"

namespace boto {

/**
 * @brief Adds a character element
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param ch the character
 * @param p the position
 * @param style
 */
inline void
character(Target target,
          char ch,
          const SDL_Point& p,
          const TextStyle& style = themeFor<Text>())
{
  auto& state = target.getState();
  auto caret = target.getCaret();
  target.advance(presentCharacter(target.getDisplayList(),
                                  ch,
                                  {p.x + caret.x, p.y + caret.y},
                                  Status::NONE,
                                  adjustDefaultFont(style, state.getFont())));
}

/**
 * @brief Adds a text element
 * @ingroup elements
 *
 * @param target the parent group or frame
 * @param str the text
 * @param p the position
 * @param style
 */
inline void
text(Target target,
     std::string_view str,
     const SDL_Point& p,
     const TextStyle& style = themeFor<Text>())
{
  auto& state = target.getState();
  auto caret = target.getCaret();
  target.advance(presentText(target.getDisplayList(),
                             str,
                             {p.x + caret.x, p.y + caret.y},
                             Status::NONE,
                             adjustDefaultFont(style, state.getFont())));
}
} // namespace boto

#endif // BOTO_ELEMENTS_TEXT_HPP_
