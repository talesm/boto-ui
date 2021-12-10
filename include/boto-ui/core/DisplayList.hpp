#ifndef BOTO_CORE_DISPLAY_LIST_ITEM
#define BOTO_CORE_DISPLAY_LIST_ITEM

#include <vector>
#include <SDL_blendmode.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>

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

  void push(SDL_Rect rect, SDL_Color color, SDL_BlendMode mode)
  {
    push(rect, color, mode, nullptr, {0});
  }

  class Clip
  {
    DisplayList* dList;

    Clip(DisplayList* dList)
      : dList(dList)
    {}

  public:
    Clip(const Clip&) = delete;
    Clip(Clip&& c)
      : dList(c.dList)
    {
      c.dList = nullptr;
    }
    ~Clip()
    {
      if (dList)
        dList->unClip();
    }

    operator bool() const { return true; }

    friend class DisplayList;
  };

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

#endif // BOTO_CORE_DISPLAY_LIST_ITEM
