#ifndef BOTO_CORE_DISPLAYLIST_HPP
#define BOTO_CORE_DISPLAYLIST_HPP

#include <vector>
#include <SDL_blendmode.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include "util/CookieBase.hpp"

struct SDL_Texture;
typedef struct SDL_Texture SDL_Texture;

namespace boto {
enum class DisplayListAction
{
  RESET_CLIP,
  SET_CLIP,
  COLOR_BOX,
  TEXTURE_BOX,
};

struct DisplayListItem
{
  DisplayListAction action;
  SDL_Rect rect;
  SDL_Rect srcRect;
  SDL_BlendMode mode;
  SDL_Texture* texture;
  SDL_Color color;
};

class DisplayList
{
private:
  std::vector<SDL_Rect> clipRects;
  std::vector<DisplayListItem> items;

  struct UnClipper
  {
    void operator()(DisplayList* dList) const { dList->unClip(); }
  };

public:
  void clear()
  {
    clipRects.clear();
    items.clear();
  }

  template<class F>
  void visit(F visitor) const
  {
    for (auto it = items.rbegin(); it != items.rend(); it++) {
      visitor(*it);
    }
  }

  void push(SDL_Rect rect,
            SDL_Color color,
            SDL_BlendMode mode,
            SDL_Texture* texture,
            SDL_Rect srcRect)
  {
    if (!clipRects.empty() && !SDL_HasIntersection(&clipRects.back(), &rect))
      return;
    items.push_back({
      texture ? DisplayListAction::TEXTURE_BOX : DisplayListAction::COLOR_BOX,
      rect,
      srcRect,
      mode,
      texture,
      color,
    });
  }

  void push(SDL_Rect rect,
            SDL_Color color,
            SDL_BlendMode mode = SDL_BLENDMODE_BLEND)
  {
    push(rect, color, mode, nullptr, {0});
  }

  using Clip = CookieBase<DisplayList, UnClipper>;

  Clip clip(const SDL_Rect& clip)
  {
    if (clipRects.empty()) {
      items.push_back({DisplayListAction::RESET_CLIP});
      clipRects.push_back(clip);
    } else {
      auto& back = clipRects.back();
      items.push_back({DisplayListAction::SET_CLIP, back});
      SDL_Rect result;
      SDL_IntersectRect(&back, &clip, &result);
      clipRects.push_back(result);
    }
    return {this};
  }

private:
  void unClip()
  {
    auto& back = clipRects.back();
    items.push_back({DisplayListAction::SET_CLIP, back});
    clipRects.pop_back();
  }
};

} // namespace boto

#endif // BOTO_CORE_DISPLAYLIST_HPP
