#ifndef DUI_DISPLAY_LIST_HPP
#define DUI_DISPLAY_LIST_HPP

#include <algorithm>
#include <string>
#include <vector>
#include <SDL_rect.h>
#include <SDL_render.h>

// TODO include this conditionally
#include <SDL2_gfxPrimitives.h>

namespace dui {

struct Shape
{
  SDL_Rect rect;
  SDL_Color color;
  char content;
  static Shape Character(const SDL_Point& p, SDL_Color c, char ch)
  {
    return {{p.x, p.y, 8, 8}, c, ch};
  }

  static Shape Box(const SDL_Rect& r, SDL_Color c) { return {r, c, 0}; }
};

/**
 * @brief Contains the list of elements to render
 *
 */
class DisplayList
{
private:
  std::vector<Shape> items;

public:
  void clear() { items.clear(); }

  size_t size() { return items.size(); }

  void insert(const Shape& item)
  {
    if (item.color.a > 0) {
      items.push_back(item);
    }
  }

  void pushClip(const SDL_Rect& rect)
  {
    // TODO coalesce multiple clips
    if (rect.w > 0 && rect.h > 0) {
      items.push_back(Shape{rect, {0}, 0});
    } else {
      items.push_back(Shape{{rect.x, rect.y, 1, 1}, {0}, 0});
    }
  }

  void popClip() { items.push_back(Shape{{0}, {0}, 0}); }

  void render(SDL_Renderer* renderer) const;
};

inline void
DisplayList::render(SDL_Renderer* renderer) const
{
  constexpr int STACK_MAX_SIZE = 32;
  SDL_Rect stack[STACK_MAX_SIZE]; // TODO make this configurable
  int stackSz = 0;
  for (auto it = items.rbegin(); it != items.rend(); it++) {
    auto c = it->color;
    if (c.a == 0) {
      if (it->rect.w == 0) {
        SDL_assert(stackSz > 0);
        --stackSz;
      } else {
        SDL_assert(stackSz < STACK_MAX_SIZE);
        stack[stackSz++] = it->rect;
      }
      SDL_RenderSetClipRect(renderer,
                            stackSz > 0 ? &stack[stackSz - 1] : nullptr);
    } else if (it->content == 0) {
      SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
      SDL_RenderFillRect(renderer, &it->rect);
    } else {
      characterRGBA(
        renderer, it->rect.x, it->rect.y, it->content, c.r, c.g, c.b, c.a);
    }
  }
  SDL_assert(stackSz == 0);
}

} // namespace dui

#endif // DUI_DISPLAY_LIST_HPP
