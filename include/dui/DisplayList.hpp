#ifndef DUI_DISPLAY_LIST_HPP
#define DUI_DISPLAY_LIST_HPP

#include <vector>
#include <SDL_rect.h>
#include <SDL_render.h>

namespace dui {

struct Shape
{
  SDL_Texture* texture;
  SDL_Rect rect;
  SDL_Rect srcRect;
  SDL_Color color;

  static Shape Box(const SDL_Rect& r, SDL_Color c)
  {
    return {nullptr, r, {0}, c};
  }
  static Shape Texture(const SDL_Rect& r, SDL_Texture* texture)
  {
    return {texture, r, {0}, {255, 255, 255, 255}};
  }
  static Shape Texture(const SDL_Rect& r, SDL_Texture* texture, SDL_Color c)
  {
    return {texture, r, {0}, c};
  }
  static Shape Texture(const SDL_Rect& r,
                       SDL_Texture* texture,
                       SDL_Rect& srcRect)
  {
    return {texture, r, srcRect, {255, 255, 255, 255}};
  }
  static Shape Texture(const SDL_Rect& r,
                       SDL_Texture* texture,
                       SDL_Rect& srcRect,
                       SDL_Color c)
  {
    return {texture, r, srcRect, c};
  }
};

/**
 * @brief Contains the list of elements to render
 *
 */
class DisplayList
{
  enum CommandType
  {
    POP_CLIP,
    PUSH_CLIP,
    SHAPE,
  };

  struct Command
  {
    union
    {
      Shape shape;
      SDL_Rect rect;
    };
    CommandType type;

    Command()
      : type(POP_CLIP)
    {}
    Command(const Shape& shape)
      : shape(shape)
      , type(SHAPE)
    {}
    Command(const SDL_Rect& rect)
      : rect(rect)
      , type(PUSH_CLIP)
    {}
  };
  static constexpr int MAX_LAYERS = 8;
  std::vector<Command> items[MAX_LAYERS];
  int zIndex = 0;
  int maxZIndex = 0;

public:
  void clear()
  {
    for (int i = 0; i <= maxZIndex; ++i) {
      items[i].clear();
    }
    maxZIndex = 0;
  }

  void insert(const Shape& item)
  {
    if (item.color.a > 0) {
      items[zIndex].push_back({item});
    }
  }

  void pushClip(const SDL_Rect& rect)
  {
    // TODO coalesce multiple clips
    if (rect.w > 0 && rect.h > 0) {
      items[zIndex].push_back({rect});
    } else {
      items[zIndex].push_back({{rect.x, rect.y, 1, 1}});
    }
  }

  void popClip() { items[zIndex].push_back({}); }

  void render(SDL_Renderer* renderer) const;

  void incZ()
  {
    zIndex++;
    if (zIndex > maxZIndex) {
      maxZIndex = zIndex;
      SDL_assert(maxZIndex < MAX_LAYERS);
    }
  }
  void decZ() { zIndex--; }

  int getZIndex() const { return zIndex; }

  int getMaxZIndex() const { return maxZIndex; }
};

inline void
DisplayList::render(SDL_Renderer* renderer) const
{
  // Save render state
  SDL_BlendMode blendMode;
  SDL_GetRenderDrawBlendMode(renderer, &blendMode);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  // Stack
  constexpr int STACK_MAX_SIZE = 32;
  SDL_Rect stack[STACK_MAX_SIZE]; // TODO make this configurable
  for (int zIndex = 0; zIndex <= maxZIndex; ++zIndex) {
    int stackSz = 0;
    for (auto it = items[zIndex].rbegin(); it != items[zIndex].rend(); it++) {
      if (it->type == POP_CLIP) {
        SDL_assert(stackSz > 0);
        --stackSz;
        SDL_RenderSetClipRect(renderer,
                              stackSz > 0 ? &stack[stackSz - 1] : nullptr);
        continue;
      }
      if (it->type == PUSH_CLIP) {
        SDL_assert(stackSz < STACK_MAX_SIZE);
        SDL_Rect rect = it->rect;
        if (stackSz > 0) {
          SDL_IntersectRect(&it->rect, &stack[stackSz - 1], &rect);
        }
        stack[stackSz++] = rect;
        SDL_RenderSetClipRect(renderer, &rect);
        continue;
      }
      auto& shape = it->shape;
      auto c = shape.color;
      if (shape.texture == nullptr) {
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        SDL_RenderFillRect(renderer, &shape.rect);
      }
      SDL_SetTextureColorMod(shape.texture, c.r, c.g, c.b);
      if (shape.srcRect.w) {
        SDL_RenderCopy(renderer, shape.texture, &shape.srcRect, &shape.rect);
      } else {
        SDL_RenderCopy(renderer, shape.texture, nullptr, &shape.rect);
      }
    }
    SDL_assert(stackSz == 0);
  }
  SDL_SetRenderDrawBlendMode(renderer, blendMode);
}

} // namespace dui

#endif // DUI_DISPLAY_LIST_HPP
