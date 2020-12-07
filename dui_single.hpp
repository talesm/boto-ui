/*
 * MIT License
 *
 * Copyright (c) 2020 Tales Miranda
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#ifndef DUI_SINGLE_HPP
#define DUI_SINGLE_HPP

#include <string>
#include <string_view>
#include <vector>
#include <SDL.h>

namespace dui {

// begin DisplayList.hpp
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
      items.push_back(Shape{nullptr, rect, {0}, 0});
    } else {
      items.push_back(Shape{nullptr, {rect.x, rect.y, 1, 1}, {0}, 0});
    }
  }

  void popClip() { items.push_back(Shape{nullptr, {0}, {0}, 0}); }

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
    } else if (it->texture != nullptr) {
      SDL_SetTextureColorMod(it->texture, c.r, c.g, c.b);
      if (it->srcRect.w) {
        SDL_RenderCopy(renderer, it->texture, &it->srcRect, &it->rect);
      } else {
        SDL_RenderCopy(renderer, it->texture, nullptr, &it->rect);
      }
    } else {
      SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
      SDL_RenderFillRect(renderer, &it->rect);
    }
  }
  SDL_assert(stackSz == 0);
}

// begin defaultFont.h
unsigned char font_bmp[] = {
  0x42, 0x4d, 0x3e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00,
  0x00, 0x28, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x18, 0x08, 0x18,
  0x10, 0x00, 0x00, 0x10, 0x08, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x3c,
  0x18, 0x0c, 0x3c, 0x30, 0x18, 0x00, 0x18, 0x18, 0x44, 0x7c, 0x44, 0x44, 0xdb,
  0x10, 0x66, 0x18, 0x18, 0x0e, 0x7e, 0x70, 0x3c, 0x7e, 0x1c, 0x38, 0xaa, 0xaa,
  0xaa, 0x92, 0x8a, 0x30, 0x66, 0x81, 0x18, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x1c,
  0x38, 0xba, 0xb2, 0x92, 0xba, 0xda, 0x7e, 0x18, 0xc3, 0xff, 0xff, 0x18, 0xff,
  0x00, 0x18, 0x18, 0x18, 0xaa, 0xaa, 0xaa, 0xaa, 0x92, 0x32, 0x18, 0xc3, 0x7e,
  0x0e, 0x18, 0x70, 0x00, 0x00, 0x10, 0x08, 0x92, 0xb2, 0x82, 0x82, 0xdb, 0x12,
  0x66, 0x81, 0x3c, 0x0c, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44,
  0x44, 0x00, 0x06, 0x66, 0x18, 0x18, 0x08, 0x18, 0x10, 0x00, 0x00, 0x00, 0x00,
  0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff,
  0xfe, 0x3f, 0xff, 0xfe, 0x3c, 0x3c, 0x7e, 0x7e, 0x20, 0x66, 0xfe, 0x40, 0x00,
  0x00, 0x41, 0x01, 0x01, 0x7f, 0xff, 0xff, 0x42, 0x42, 0x42, 0x42, 0x70, 0x3c,
  0xee, 0x20, 0x00, 0x00, 0x40, 0x00, 0x01, 0x7f, 0xff, 0xff, 0x42, 0x5a, 0x42,
  0x5a, 0xd8, 0x18, 0xfe, 0x1c, 0x00, 0x00, 0x40, 0x00, 0x01, 0x7f, 0xff, 0xff,
  0x42, 0x5a, 0x42, 0x5a, 0x0c, 0x3c, 0xee, 0x12, 0x00, 0x00, 0x41, 0x01, 0x01,
  0x7f, 0xff, 0xff, 0x42, 0x42, 0x42, 0x42, 0x06, 0x66, 0x6c, 0x12, 0x00, 0x00,
  0x3f, 0xff, 0xfe, 0x3f, 0xff, 0xfe, 0x3c, 0x3c, 0x7e, 0x7e, 0x00, 0x00, 0x38,
  0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x7e, 0x24, 0xfc, 0x3c,
  0x14, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x7e,
  0x3c, 0xfe, 0x3c, 0x14, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf9,
  0x00, 0x42, 0x7a, 0x24, 0x91, 0x3c, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xfb, 0x3c, 0x42, 0x7a, 0x3c, 0x10, 0x3c, 0x08, 0x7e, 0x7e, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0x3c, 0x42, 0x7e, 0x24, 0xfc, 0x3c, 0x14,
  0x7e, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf9, 0x00, 0x42, 0x7e, 0x3c,
  0x7e, 0x18, 0x14, 0x7e, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c,
  0x24, 0x3c, 0x24, 0x7e, 0x00, 0x08, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x18, 0x18, 0x3c, 0xfc, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x88, 0xf0, 0x80, 0x80, 0x90, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x8c, 0x46, 0x41, 0x60, 0x40,
  0x3c, 0x24, 0x00, 0x00, 0x00, 0x24, 0x00, 0x08, 0x00, 0x00, 0x30, 0x46, 0x27,
  0x21, 0x50, 0x20, 0xbd, 0x24, 0x24, 0x66, 0x00, 0x3c, 0x18, 0x1c, 0x00, 0x00,
  0x68, 0x22, 0x17, 0x13, 0xa8, 0x10, 0xbd, 0x24, 0x24, 0x24, 0x24, 0x3c, 0x24,
  0x1c, 0x00, 0x00, 0x94, 0x11, 0x0f, 0x0e, 0x18, 0x08, 0x7e, 0x3c, 0x18, 0x00,
  0x24, 0x3c, 0x24, 0x3e, 0x00, 0x00, 0x0a, 0x09, 0x04, 0x0c, 0x00, 0x06, 0x3c,
  0x3c, 0x00, 0x00, 0x00, 0x18, 0x18, 0x1c, 0x00, 0x00, 0x05, 0x05, 0x00, 0x1a,
  0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x03,
  0x03, 0x00, 0x70, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x24, 0x4a, 0x00, 0x00, 0x80, 0x00, 0x24, 0x4a, 0x14, 0x14, 0x24,
  0x24, 0x00, 0x00, 0x00, 0x00, 0xbd, 0x70, 0x42, 0x00, 0x7d, 0x2c, 0x99, 0x48,
  0x5d, 0x5d, 0xbd, 0x81, 0x00, 0x00, 0x00, 0x00, 0x42, 0x75, 0x5a, 0x00, 0x36,
  0x3e, 0x42, 0x72, 0x3e, 0x08, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x72,
  0x24, 0x00, 0x04, 0x7e, 0x28, 0x77, 0x1c, 0x1c, 0x3c, 0x5a, 0x00, 0x00, 0x00,
  0x00, 0x7e, 0x87, 0x00, 0x18, 0x00, 0x7e, 0x7e, 0x72, 0x2a, 0x2a, 0x66, 0x7e,
  0x00, 0x00, 0x00, 0x00, 0x5a, 0x05, 0x00, 0x5a, 0x00, 0x6a, 0x5a, 0x87, 0x2a,
  0x7f, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x24, 0x00, 0x3c,
  0x7e, 0x05, 0x7f, 0x3e, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00,
  0x00, 0x00, 0x18, 0x3c, 0x00, 0x41, 0x1c, 0x3c, 0x5a, 0x00, 0x00, 0x00, 0x00,
  0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x2c, 0x00, 0x2c,
  0x2c, 0x00, 0x00, 0xff, 0xe7, 0xe7, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff,
  0x24, 0x00, 0x24, 0x24, 0x00, 0x00, 0xff, 0xe7, 0xe3, 0xc7, 0xff, 0xff, 0xff,
  0xff, 0xe7, 0xff, 0x34, 0xff, 0x37, 0xf4, 0x1f, 0xf8, 0x00, 0x66, 0x60, 0x06,
  0x70, 0x0e, 0x60, 0x06, 0x66, 0x66, 0x24, 0x88, 0x20, 0x84, 0x28, 0x44, 0x00,
  0x66, 0x70, 0x0e, 0x60, 0x06, 0x60, 0x06, 0x66, 0x66, 0x2c, 0x22, 0x22, 0x24,
  0x22, 0x0c, 0xff, 0xe7, 0xff, 0xff, 0xe3, 0xc7, 0xff, 0xff, 0xff, 0xe7, 0x24,
  0xff, 0x1f, 0xf8, 0x27, 0xe4, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xe7, 0xff, 0xff,
  0xff, 0xe7, 0x34, 0x00, 0x00, 0x00, 0x34, 0x34, 0x66, 0x66, 0x66, 0x66, 0x66,
  0x66, 0x66, 0x66, 0x66, 0x66, 0x24, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x66,
  0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x66, 0x3c, 0x66, 0x66, 0x66, 0x00, 0x66,
  0x00, 0xff, 0x66, 0x67, 0xe6, 0x3f, 0xfc, 0x7f, 0xfe, 0x66, 0x7e, 0xe7, 0xe6,
  0x67, 0xff, 0xe7, 0x3c, 0xff, 0x66, 0x63, 0xc6, 0x7f, 0xfe, 0xff, 0xff, 0x66,
  0x66, 0xc3, 0xc6, 0x63, 0xff, 0xc3, 0x7e, 0x00, 0x66, 0x60, 0x06, 0x70, 0x0e,
  0xc0, 0x03, 0x66, 0x66, 0x00, 0x06, 0x60, 0x00, 0x00, 0x66, 0x00, 0x66, 0x70,
  0x0e, 0x60, 0x06, 0xc0, 0x03, 0x66, 0x66, 0x00, 0x06, 0x60, 0x00, 0x00, 0x66,
  0xff, 0x66, 0x7f, 0xfe, 0x63, 0xc6, 0xff, 0xff, 0x66, 0x66, 0xc3, 0xc6, 0x63,
  0xc3, 0xff, 0x7e, 0xff, 0x66, 0x3f, 0xfc, 0x67, 0xe6, 0x7f, 0xfe, 0x7e, 0x66,
  0xe7, 0xe6, 0x67, 0xe7, 0xff, 0x3c, 0x00, 0x66, 0x00, 0x00, 0x66, 0x66, 0x00,
  0x00, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18,
  0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00, 0x00,
  0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x18, 0x18, 0x18, 0x00,
  0x18, 0x00, 0x00, 0x18, 0x1c, 0x38, 0x00, 0x00, 0x60, 0x06, 0x18, 0x18, 0x18,
  0x18, 0x18, 0x00, 0x18, 0x18, 0xff, 0x18, 0x1f, 0xf8, 0x0f, 0xf0, 0x7f, 0xfe,
  0x18, 0x18, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0x3c, 0xff, 0x18, 0x0f, 0xf0, 0x1f,
  0xf8, 0x7f, 0xfe, 0x18, 0x18, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0x3c, 0x00, 0x18,
  0x00, 0x00, 0x1c, 0x38, 0x60, 0x06, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00,
  0x18, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x3c, 0x18, 0x18, 0x18,
  0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
  0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x78, 0x07, 0x78, 0x7c, 0x1c, 0x1c,
  0x18, 0x36, 0x36, 0x3c, 0x7e, 0x0c, 0x18, 0x30, 0x00, 0x00, 0x30, 0x3e, 0x30,
  0x06, 0x30, 0x36, 0x3c, 0x6b, 0x1c, 0x06, 0x32, 0x18, 0x18, 0x18, 0x00, 0x00,
  0x3e, 0x66, 0x30, 0x7e, 0x30, 0x36, 0x24, 0x6b, 0x08, 0x3e, 0x18, 0x10, 0x18,
  0x08, 0x00, 0x00, 0x33, 0x66, 0x3b, 0x60, 0x30, 0x36, 0x66, 0x6b, 0x1c, 0x66,
  0x4c, 0x30, 0x18, 0x0c, 0x4c, 0x00, 0x7e, 0x3e, 0x36, 0x3e, 0x78, 0x77, 0x66,
  0x63, 0x36, 0x66, 0x7c, 0x10, 0x18, 0x08, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x32, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x18, 0x30,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x7c, 0x3c, 0x3e, 0x3c, 0x3c, 0x78,
  0xe7, 0x3c, 0x70, 0x73, 0x3c, 0xeb, 0xe7, 0x3c, 0x00, 0x66, 0x66, 0x66, 0x66,
  0x60, 0x18, 0x0c, 0x66, 0x18, 0x18, 0x36, 0x18, 0x6a, 0x66, 0x66, 0x00, 0x3e,
  0x66, 0x60, 0x66, 0x7e, 0x18, 0x7c, 0x66, 0x18, 0x18, 0x3c, 0x18, 0x6a, 0x66,
  0x66, 0x00, 0x06, 0x66, 0x66, 0x66, 0x66, 0x7e, 0xcc, 0x66, 0x18, 0x18, 0x36,
  0x18, 0x6a, 0x66, 0x66, 0x00, 0x3c, 0x7c, 0x3c, 0x3e, 0x3c, 0x18, 0x7e, 0x7c,
  0x38, 0x38, 0x33, 0x18, 0x54, 0x5c, 0x3c, 0x08, 0x00, 0x60, 0x00, 0x06, 0x00,
  0x18, 0x00, 0x60, 0x00, 0x00, 0x30, 0x18, 0x00, 0x00, 0x00, 0x10, 0x00, 0x60,
  0x00, 0x06, 0x00, 0x0e, 0x00, 0xe0, 0x18, 0x18, 0x70, 0x38, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x78, 0x3f, 0xe7, 0x3c, 0x1e, 0x3c, 0x18, 0x24, 0xc3, 0x70,
  0x7e, 0x1c, 0x00, 0x38, 0x00, 0x7e, 0x30, 0x6e, 0x66, 0x26, 0x0c, 0x7e, 0x18,
  0x3c, 0x66, 0x18, 0x62, 0x18, 0x06, 0x18, 0x00, 0x00, 0x30, 0x66, 0x66, 0x06,
  0x0c, 0x66, 0x3c, 0x5a, 0x3c, 0x0c, 0x30, 0x18, 0x0c, 0x18, 0x00, 0x00, 0x3e,
  0x66, 0x7c, 0x1c, 0x0c, 0x66, 0x24, 0x5a, 0x18, 0x3e, 0x18, 0x18, 0x18, 0x18,
  0x00, 0x00, 0x33, 0x66, 0x66, 0x30, 0x0c, 0x66, 0x66, 0x42, 0x3c, 0x66, 0x0c,
  0x18, 0x30, 0x18, 0x66, 0x00, 0x33, 0x66, 0x66, 0x32, 0x2d, 0x66, 0x66, 0x66,
  0x66, 0x66, 0x46, 0x18, 0x60, 0x18, 0x3c, 0x00, 0x7e, 0x3c, 0xfc, 0x1e, 0x3f,
  0xe7, 0xe7, 0xe7, 0xc3, 0xe7, 0x7e, 0x1c, 0x40, 0x38, 0x18, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x3c, 0xe7, 0x7e, 0x3c, 0x7c, 0x7f, 0x78, 0x3f, 0xe7, 0x3c, 0x38, 0x73,
  0x7f, 0x63, 0xe6, 0x3c, 0x60, 0x66, 0x33, 0x76, 0x36, 0x31, 0x30, 0x73, 0x66,
  0x18, 0x0c, 0x36, 0x31, 0x63, 0x6e, 0x66, 0x6e, 0x66, 0x33, 0x60, 0x33, 0x30,
  0x30, 0x67, 0x66, 0x18, 0x0c, 0x3c, 0x30, 0x63, 0x6e, 0x66, 0x6e, 0x7e, 0x3e,
  0x60, 0x33, 0x3c, 0x3c, 0x60, 0x7e, 0x18, 0x0c, 0x38, 0x30, 0x6b, 0x7e, 0x66,
  0x66, 0x66, 0x33, 0x60, 0x33, 0x30, 0x30, 0x60, 0x66, 0x18, 0x0c, 0x3c, 0x30,
  0x7f, 0x76, 0x66, 0x66, 0x3c, 0x33, 0x76, 0x36, 0x31, 0x31, 0x73, 0x66, 0x18,
  0x0c, 0x36, 0x30, 0x77, 0x76, 0x66, 0x3c, 0x18, 0x7e, 0x3c, 0x7c, 0x7f, 0x7f,
  0x3e, 0xe7, 0x3c, 0x3f, 0x73, 0x70, 0x63, 0x67, 0x3c, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c,
  0x3c, 0x7e, 0x3c, 0x06, 0x3c, 0x3c, 0x38, 0x3c, 0x38, 0x18, 0x08, 0x06, 0x00,
  0x60, 0x18, 0x66, 0x18, 0x62, 0x66, 0x7f, 0x06, 0x66, 0x18, 0x66, 0x0c, 0x18,
  0x18, 0x0c, 0x00, 0x30, 0x00, 0x76, 0x18, 0x30, 0x06, 0x67, 0x06, 0x66, 0x18,
  0x66, 0x06, 0x00, 0x00, 0x18, 0x3c, 0x18, 0x18, 0x6e, 0x18, 0x18, 0x0c, 0x66,
  0x3c, 0x66, 0x0c, 0x3c, 0x3e, 0x00, 0x00, 0x30, 0x00, 0x0c, 0x1c, 0x66, 0x18,
  0x0c, 0x06, 0x36, 0x30, 0x7c, 0x06, 0x66, 0x66, 0x18, 0x18, 0x18, 0x3c, 0x18,
  0x06, 0x66, 0x38, 0x66, 0x66, 0x1e, 0x32, 0x30, 0x06, 0x66, 0x66, 0x18, 0x18,
  0x0c, 0x00, 0x30, 0x66, 0x3c, 0x18, 0x3c, 0x3c, 0x0e, 0x3e, 0x1e, 0x7e, 0x3c,
  0x3c, 0x00, 0x00, 0x06, 0x00, 0x60, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00,
  0x00, 0x08, 0x00, 0x3d, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x20, 0x00, 0x18, 0x00,
  0x00, 0x00, 0x00, 0x24, 0x3e, 0x4c, 0x66, 0x00, 0x18, 0x18, 0x24, 0x18, 0x10,
  0x00, 0x18, 0x60, 0x00, 0x18, 0x00, 0x7e, 0x0b, 0x24, 0x3e, 0x00, 0x30, 0x0c,
  0x18, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x18, 0x00, 0x24, 0x3e, 0x10, 0x19,
  0x00, 0x30, 0x0c, 0x7e, 0x7e, 0x00, 0x7e, 0x00, 0x18, 0x00, 0x18, 0x12, 0x24,
  0x68, 0x48, 0x3c, 0x08, 0x30, 0x0c, 0x18, 0x18, 0x00, 0x00, 0x00, 0x0c, 0x00,
  0x18, 0x36, 0x7e, 0x3e, 0x64, 0x66, 0x18, 0x18, 0x18, 0x24, 0x18, 0x00, 0x00,
  0x00, 0x06, 0x00, 0x18, 0x36, 0x24, 0x08, 0x00, 0x3c, 0x18, 0x0c, 0x30, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xb3, 0x4c,
  0x00, 0x00, 0x10, 0x10, 0x02, 0x44, 0xff, 0xff, 0x18, 0x08, 0x22, 0x00, 0x00,
  0x00, 0xcd, 0x32, 0x22, 0x14, 0x54, 0x38, 0x26, 0x44, 0x00, 0x7e, 0x00, 0x08,
  0x36, 0x00, 0x00, 0x00, 0xff, 0x00, 0x2a, 0x54, 0x54, 0x54, 0x60, 0x66, 0x7e,
  0x3c, 0x18, 0x22, 0x1e, 0x00, 0x00, 0x00, 0xff, 0x00, 0x48, 0x44, 0x92, 0x54,
  0x00, 0x00, 0x00, 0x7e, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0xb3, 0x4c, 0x08,
  0x02, 0x10, 0x10, 0x04, 0x88, 0x3c, 0x3c, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0xcd, 0x32, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x88, 0x00, 0x18, 0x00, 0x00, 0x20,
  0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x18, 0x18,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x55, 0xcc,
  0xaa, 0xff, 0xc3, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c,
  0x11, 0xaa, 0xcc, 0xaa, 0x00, 0x81, 0xff, 0x55, 0x7f, 0x6e, 0x00, 0x18, 0x00,
  0x00, 0x18, 0x7e, 0x44, 0x55, 0x33, 0xaa, 0xff, 0x18, 0xfd, 0x77, 0x7f, 0x7c,
  0x00, 0x3c, 0x00, 0x18, 0x3c, 0x7e, 0x11, 0xaa, 0x33, 0xaa, 0x00, 0x3c, 0xff,
  0x77, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x18, 0x3c, 0x7e, 0x44, 0x55, 0xcc, 0xaa,
  0xff, 0x3c, 0x00, 0x00, 0xf7, 0x5e, 0x00, 0x7e, 0x00, 0x00, 0x18, 0x7e, 0x11,
  0xaa, 0xcc, 0xaa, 0x00, 0x18, 0xff, 0xaa, 0xf7, 0x7a, 0x00, 0x3c, 0x00, 0x00,
  0x00, 0x3c, 0x44, 0x55, 0x33, 0xaa, 0xff, 0x81, 0xfd, 0xee, 0xf7, 0x7e, 0x00,
  0x18, 0x00, 0x00, 0x00, 0x00, 0x11, 0xaa, 0x33, 0xaa, 0x00, 0xc3, 0xff, 0xee,
  0x00, 0x00, 0x00, 0x00};
unsigned int font_bmp_len = 2110;

// begin Font.hpp
inline SDL_Texture*
loadDefaultFont(SDL_Renderer* renderer)
{
  SDL_RWops* src = SDL_RWFromConstMem(font_bmp, font_bmp_len);
  SDL_Surface* surface = SDL_LoadBMP_RW(src, 1);
  SDL_SetColorKey(surface, 1, 0);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

// begin State.hpp
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
  bool mHovering = false;
  bool mGrabbing = false;
  bool mReleasing = false;
  std::string eActive;
  char tBuffer[SDL_TEXTINPUTEVENT_TEXT_SIZE];
  bool tChanged = false;
  TextAction tAction = TextAction::NONE;

  std::string group;
  bool gGrabbed = false;
  bool gActive = false;

  SDL_Texture* font;

public:
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
    void pushGroup(std::string_view id, const SDL_Rect& r)
    {
      state->beginGroup(id, r);
    }
    /// Pops a group, Must be have been a pushGroup with same id before
    void popGroup(std::string_view id, const SDL_Rect& r)
    {
      state->endGroup(id, r);
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

  SDL_Texture* getFont() const { return font; }

private:
  void beginFrame()
  {
    SDL_assert(inFrame == false);
    inFrame = true;
    dList.clear();
    mHovering = false;
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

  void beginGroup(std::string_view id, const SDL_Rect& r);
  void endGroup(std::string_view id, const SDL_Rect& r);
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
  dList.popClip();
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
  dList.pushClip(r);
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

// begin Group.hpp
namespace style {
constexpr int elementSpacing = 2;
} // namespace style

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

  int width() const
  {
    if (rect.w > 0) {
      return rect.w;
    }
    int w = bottomRight.x - topLeft.x;
    if (layout == Layout::HORIZONTAL && w >= style::elementSpacing) {
      w -= style::elementSpacing;
    }
    return w;
  }

  int height() const
  {
    if (rect.h > 0) {
      return rect.h;
    }
    int h = bottomRight.y - topLeft.y;
    if (layout == Layout::VERTICAL && h >= style::elementSpacing) {
      h -= style::elementSpacing;
    }
    return h;
  }

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
      rect.w = width();
    }
    if (rect.h == 0) {
      rect.h = height();
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
    bottomRight.y += p.y + 2;
  } else if (layout == Layout::HORIZONTAL) {
    bottomRight.x += p.x + 2;
    bottomRight.y = std::max(p.y + topLeft.y, bottomRight.y);
  } else {
    bottomRight.x = std::max(p.x + topLeft.x, bottomRight.x);
    bottomRight.y = std::max(p.y + topLeft.y, bottomRight.y);
  }
}

// begin Frame.hpp
/**
 * @brief Represents a single frame on the app
 *
 */
class Frame : public Group
{
  State::Context context;

public:
  Frame(State* state = nullptr);
  ~Frame() { end(); }
  Frame(const Frame&) = delete;
  Frame(Frame&& rhs) = default;
  Frame& operator=(const Frame&) = delete;
  Frame& operator=(Frame&& rhs) = default;

  void render()
  {
    SDL_assert(valid());
    auto& state = getState();
    end();
    state.render();
  }

  // It is called by render normally
  void end();

protected:
  void afterLock(int deepness, std::string_view id, const SDL_Rect& r) final
  {
    context.pushGroup(id, r);
  }
  void beforeUnlock(int deepness, std::string_view id, const SDL_Rect& r) final
  {
    context.popGroup(id, r);
  }
};

inline Frame
frame(State& state)
{
  return {&state};
}

inline Frame::Frame(State* state)
  : Group("", {0}, state, Layout::NONE)
  , context(state->lockFrame())
{}

inline void
Frame::end()
{
  if (bool(*this)) {
    context.unlockFrame();
    Group::end();
  }
}

// begin EdgeSize.hpp
struct EdgeSize
{
  Uint8 left;
  Uint8 top;
  Uint8 right;
  Uint8 bottom;

  constexpr static EdgeSize all(Uint8 sz) { return {sz, sz, sz, sz}; }
  constexpr EdgeSize withLeft(Uint8 sz) const
  {
    return {sz, top, right, bottom};
  }
  constexpr EdgeSize withTop(Uint8 sz) const
  {
    return {left, sz, right, bottom};
  }
  constexpr EdgeSize withRight(Uint8 sz) const { return {sz, top, sz, bottom}; }
  constexpr EdgeSize withBottom(Uint8 sz) const { return {sz, top, right, sz}; }

  constexpr EdgeSize operator+(const EdgeSize& rhs) const
  {
    return {Uint8(left + rhs.left),
            Uint8(top + rhs.top),
            Uint8(right + rhs.right),
            Uint8(bottom + rhs.bottom)};
  }
  constexpr EdgeSize operator-(const EdgeSize& rhs) const
  {
    return {Uint8(left - rhs.left),
            Uint8(top - rhs.top),
            Uint8(right - rhs.right),
            Uint8(bottom - rhs.bottom)};
  }
};

constexpr SDL_Point
elementSize(const EdgeSize& edge, const SDL_Point& client = {0})
{
  return {edge.left + edge.right + client.x, edge.top + edge.bottom + client.y};
}
constexpr SDL_Point
clientSize(const EdgeSize& edge, const SDL_Point& element)
{
  return {element.x - edge.left - edge.right,
          element.y - edge.top - edge.bottom};
}

// begin WrapperGroup.hpp
/// A class to make wrapper elements
class WrapperGroup : public Group
{
  EdgeSize padding;
  static constexpr SDL_Rect paddedSize(SDL_Rect rect, const EdgeSize& padding)
  {
    rect.x += padding.left;
    rect.y += padding.top;
    if (rect.w > padding.left + padding.right) {
      rect.w -= padding.left + padding.right;
    }
    if (rect.h > padding.top + padding.bottom) {
      rect.h -= padding.top + padding.bottom;
    }
    return rect;
  }

protected:
  virtual void afterUnwrap() = 0;

public:
  WrapperGroup(Group* parent,
               std::string_view id,
               const SDL_Rect& rect,
               Layout layout,
               const EdgeSize& padding)
    : Group(parent, id, paddedSize(rect, padding), layout)
    , padding(padding)
  {}

  void end();
};
inline void
WrapperGroup::end()
{
  if (!valid()) {
    return;
  }
  rect.x -= padding.left;
  rect.y -= padding.top;
  if (layout == Layout::HORIZONTAL &&
      bottomRight.x - topLeft.x > style::elementSpacing) {
    bottomRight.x -= style::elementSpacing;
  }
  if (layout == Layout::VERTICAL &&
      bottomRight.y - topLeft.y > style::elementSpacing) {
    bottomRight.y -= style::elementSpacing;
  }
  topLeft.x -= padding.left;
  topLeft.y -= padding.top;
  bottomRight.x += padding.right;
  bottomRight.y += padding.bottom;
  if (rect.w > 0) {
    rect.w += padding.left + padding.right;
  }
  if (rect.h > 0) {
    rect.h += padding.top + padding.bottom;
  }
  afterUnwrap();
  Group::end();
}

// begin element.hpp
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
  auto caret = target.getCaret();
  target.advance({rect.x + rect.w, rect.y + rect.h});
  rect.x += caret.x;
  rect.y += caret.y;
  state.display(Shape::Box(rect, c));
}

// Border style
struct BoxStyle
{
  SDL_Color center;
  SDL_Color left;
  SDL_Color top;
  SDL_Color right;
  SDL_Color bottom;

  constexpr BoxStyle withCenter(SDL_Color c) const
  {
    return {c, left, top, right, bottom};
  }
  constexpr BoxStyle withLeft(SDL_Color c) const
  {
    return {center, c, top, right, bottom};
  }
  constexpr BoxStyle withTop(SDL_Color c) const
  {
    return {center, left, c, right, bottom};
  }
  constexpr BoxStyle withRight(SDL_Color c) const
  {
    return {center, left, top, c, bottom};
  }
  constexpr BoxStyle withBotton(SDL_Color c) const
  {
    return {center, left, top, right, c};
  }
  constexpr BoxStyle withInvertedBorders() const
  {
    return {center, right, bottom, left, top};
  }
};

// A box with colored border
inline void
borderedBox(Group& target, const SDL_Rect& r, const BoxStyle& style)
{
  auto c = style.center;
  auto e = style.right;
  auto n = style.top;
  auto w = style.left;
  auto s = style.bottom;
  auto g = group(target, {}, {0}, Layout::NONE);
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
  auto caret = target.getCaret();
  target.advance({p.x + 8, p.y + 8});
  SDL_Rect dstRect{p.x + caret.x, p.y + caret.y, 8, 8};
  SDL_Rect srcRect{(ch % 16) * 8, (ch / 16) * 8, 8, 8};
  state.display(Shape::Texture(dstRect, state.getFont(), srcRect, c));
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
     const SDL_Point& p,
     SDL_Color c = style::TEXT)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto caret = target.getCaret();
  target.advance({p.x + 8 * int(str.size()), p.y + 8});
  SDL_Rect dstRect{p.x + caret.x, p.y + caret.y, 8, 8};
  for (auto ch : str) {
    SDL_Rect srcRect{(ch % 16) * 8, (ch / 16) * 8, 8, 8};
    state.display(Shape::Texture(dstRect, state.getFont(), srcRect, c));
    dstRect.x += 8;
  }
}

/**
 * @brief adds an texturedBox element to target
 *
 * @param target the parent group or frame
 * @param texture the texture
 * @param rect the box local position and size
 */
inline void
texturedBox(Group& target, SDL_Texture* texture, SDL_Rect rect)
{
  auto& state = target.getState();
  SDL_assert(state.isInFrame());
  SDL_assert(!target.isLocked());
  auto caret = target.getCaret();
  target.advance({rect.x + rect.w, rect.y + rect.h});
  rect.x += caret.x;
  rect.y += caret.y;
  state.display(Shape::Texture(rect, texture));
}

// begin Panel.hpp
// Style for panels
struct PanelStyle
{
  BoxStyle border;
  EdgeSize padding;
};

namespace style {
/// Default panel style
constexpr PanelStyle PANEL{{{219, 228, 240, 240}, TEXT, TEXT, TEXT, TEXT},
                           EdgeSize::all(2)};
}

/// A panel class @see panel()
class Panel final : public WrapperGroup
{
  PanelStyle style;

public:
  Panel(Group* parent,
        std::string_view id,
        const SDL_Rect& r,
        Layout layout,
        const PanelStyle& style)
    : WrapperGroup(parent, id, r, layout, style.padding + EdgeSize::all(1))
    , style(style)
  {}

protected:
  void afterUnwrap() final
  {
    layout = Layout::NONE;
    borderedBox(*this, {0, 0, width(), height()}, style.border);
  }
};

/**
 * @brief adds a panel element
 *
 * @param target the parent group or frame
 * @param id the panel id
 * @param r the panel relative postion and size
 * @param layout the layout
 * @param style the style
 * @return Panel
 */
inline Panel
panel(Group& target,
      std::string_view id,
      const SDL_Rect& r = {0},
      Layout layout = Layout::VERTICAL,
      const PanelStyle& style = style::PANEL)
{
  return {&target, id, r, layout, style};
}

// begin button.hpp
// Style for button
struct ButtonStyle
{
  SDL_Color text;
  EdgeSize padding;
  BoxStyle normal;
  BoxStyle grabbed;
  BoxStyle pressed;
  BoxStyle pressedGrabbed;
};

namespace style {

constexpr BoxStyle BUTTONBOX{
  {176, 195, 222, 255},
  {255, 255, 255, 255},
  {255, 255, 255, 255},
  {0, 0, 0, 255},
  {0, 0, 0, 255},
};

constexpr BoxStyle BUTTONBOX_GRABBED{
  BUTTONBOX.withCenter({147, 173, 210, 255})};
constexpr BoxStyle BUTTONBOX_PRESSED{BUTTONBOX.withInvertedBorders()};
constexpr BoxStyle BUTTONBOX_PRESSED_GRABBED{
  BUTTONBOX_PRESSED.withCenter(BUTTONBOX_GRABBED.center)};

constexpr ButtonStyle BUTTON{
  TEXT,
  EdgeSize::all(3),
  BUTTONBOX,
  BUTTONBOX_GRABBED,
  BUTTONBOX_PRESSED,
  BUTTONBOX_PRESSED_GRABBED,
};
}

/**
 * @brief Common button behavior
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param pushed if the button is pushed or not
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state (just released)
 * @return false otherwise
 */
inline bool
buttonBase(Group& target,
           std::string_view id,
           std::string_view str,
           bool pushed,
           const SDL_Point& p = {0},
           const ButtonStyle& style = style::BUTTON)
{
  if (str.empty()) {
    str = id;
  }
  auto adv = elementSize(style.padding + EdgeSize::all(1), measure(str));
  SDL_Rect r{p.x, p.y, adv.x, adv.y};
  auto action = target.checkMouse(id, r);
  bool grabbing = action == MouseAction::HOLD;

  PanelStyle curStyle = style::PANEL;
  curStyle.padding = style.padding;
  if (grabbing == pushed) {
    curStyle.border = grabbing ? style.grabbed : style.normal;
  } else {
    curStyle.border = grabbing ? style.pressedGrabbed : style.pressed;
  }
  auto g = panel(target, id, r, Layout::NONE, curStyle);
  text(g, str, {0}, style.text);
  g.end();
  return action == MouseAction::ACTION;
}

/**
 * @{
 * @brief A push button
 *
 * It can be used to make an action every time it is pushed
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state (just released)
 * @return false otherwise
 */
inline bool
button(Group& target,
       std::string_view id,
       std::string_view str,
       const SDL_Point& p = {0},
       const ButtonStyle& style = style::BUTTON)
{
  return buttonBase(target, id, str, false, p, style);
}
inline bool
button(Group& target,
       std::string_view id,
       const SDL_Point& p = {0},
       const ButtonStyle& style = style::BUTTON)
{
  return button(target, id, id, p, style);
}
/// @}

/**
 * @{
 * @brief A button that toggle a boolean variable
 *
 * Every time it is pushed the value is negated. The button reflects this state
 * by being pushed or pulled if the variable is true of false, respectively.
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param value a pointer to a boolean with the state
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state
 * @return false otherwise
 */
inline bool
toggleButton(Group& target,
             std::string_view id,
             std::string_view str,
             bool* value,
             const SDL_Point& p = {0},
             const ButtonStyle& style = style::BUTTON)
{
  if (buttonBase(target, id, str, *value, p, style)) {
    *value = !*value;
    return true;
  }
  return false;
}
inline bool
toggleButton(Group& target,
             std::string_view id,
             bool* value,
             const SDL_Point& p = {0},
             const ButtonStyle& style = style::BUTTON)
{
  return toggleButton(target, id, id, value, p, style);
}
/// @}

/**
 * @{
 * @brief A button part of multiple choice question
 *
 * If this button is actionned the value is changed to the given option
 *
 * @param target the parent group or frame
 * @param id the button id
 * @param str the text to appear on screen (if not present the id is used)
 * @param value a pointer to the control variable
 * @param option the option this button represents if value is equivalent to
 * this, then the button will appear pressed. If the user pushed, then the value
 * is set to this option.
 * @param p the button relative position
 * @param style
 *
 * @return true when the button is action state
 * @return false otherwise
 */
template<class T, class U>
inline bool
choiceButton(Group& target,
             std::string_view id,
             std::string_view str,
             T* value,
             U option,
             const SDL_Point& p = {0},
             const ButtonStyle& style = style::BUTTON)
{
  bool selected = *value == option;
  if (buttonBase(target, id, str, selected, p, style) && !selected) {
    *value = option;
    return true;
  }
  return false;
}
template<class T, class U>
inline bool
choiceButton(Group& target,
             std::string_view id,
             T* value,
             U option,
             const SDL_Point& p = {0},
             const ButtonStyle& style = style::BUTTON)
{
  return choiceButton(target, id, id, value, option, p, style);
}
/// @}

// begin inputBox.hpp
/// Input box style
struct InputBoxStyle
{
  SDL_Color text;
  EdgeSize padding;
  BoxStyle box;
  BoxStyle boxActive;
};

namespace style {

/// Default input box style
constexpr InputBoxStyle INPUTBOX{
  TEXT,
  EdgeSize::all(2),
  {{240, 240, 240, 255}, TEXT, TEXT, TEXT, TEXT},
  {{255, 255, 255, 255}, TEXT, TEXT, TEXT, TEXT},
};

}

inline SDL_Rect
makeInputSize(SDL_Rect r, const EdgeSize& padding = style::INPUTBOX.padding)
{
  if (r.w == 0 || r.h == 0) {
    auto clientSz = measure('m'); // TODO allow customization for this
    clientSz.x *= 16;

    auto borderSize = EdgeSize::all(1);
    auto elementSz = elementSize(padding + borderSize, clientSz);

    if (r.w == 0) {
      r.w = elementSz.x;
    }
    if (r.h == 0) {
      r.h = elementSz.y;
    }
  }
  return r;
}

inline TextAction
textBoxBase(Group& target,
            std::string_view id,
            std::string_view value,
            SDL_Rect r,
            const InputBoxStyle& style = style::INPUTBOX)
{
  r = makeInputSize(r, style.padding);
  target.checkMouse(id, r);

  auto action = target.checkText(id);
  bool active = false;
  switch (action) {
    case TextAction::NONE:
      active = target.isActive(id);
      break;
    case TextAction::INPUT:
    case TextAction::BACKSPACE:
      active = true;
      break;
  }
  auto& currentStyle = active ? style.boxActive : style.box;
  auto g = panel(target, id, r, Layout::NONE, {currentStyle, style.padding});

  // This creates an auto scroll effect if value text don't fit in the box;
  auto clientSz = clientSize(style.padding + EdgeSize::all(1), {r.w, r.h});
  auto contentSz = measure(value);
  int deltaX = contentSz.x - clientSz.x;
  if (deltaX > 0) {
    int deltaChar = deltaX / 8 + 1;
    value.remove_prefix(deltaChar);
  }

  text(g, value, {0}, style.text);

  if (active) {
    // Show cursor
    box(g, {int(value.size()) * 8, 0, 1, clientSz.y}, style.text);
  }
  g.end();
  return action;
}

inline bool
textBox(Group& target,
        std::string_view id,
        char* value,
        size_t maxSize,
        const SDL_Rect& r = {0})
{
  auto len = strlen(value);
  switch (textBoxBase(target, id, {value, len}, r)) {
    case TextAction::NONE:
      break;
    case TextAction::INPUT:
      if (auto input = target.getText(); !input.empty() && maxSize > 0) {
        if (len >= maxSize - 1) {
          value[maxSize - 2] = input[0];
          value[maxSize - 1] = 0;
        } else {
          auto count = std::min(maxSize - len - 1, input.size());
          for (size_t i = 0; i < count; ++i) {
            value[len + i] = input[i];
          }
          value[len + count] = 0;
        }
        return true;
      }
      break;
    case TextAction::BACKSPACE:
      if (len > 0) {
        value[len - 1] = 0;
        return true;
      }
      break;
  }
  return false;
}

inline bool
textBox(Group& target,
        std::string_view id,
        std::string* value,
        const SDL_Rect& r = {0})
{
  switch (textBoxBase(target, id, *value, r)) {
    case TextAction::NONE:
      break;
    case TextAction::INPUT:
      if (auto input = target.getText(); !input.empty()) {
        *value += input;
        return true;
      }
      break;
    case TextAction::BACKSPACE:
      if (value->size() > 0) {
        value->pop_back();
        return true;
      }
      break;
  }
  return false;
}

// TODO delete copy ctor, other safety nets
class BufferedInputBox
{
  Group& target;
  std::string_view id;
  SDL_Rect rect;

  bool active;
  bool refillBuffer;

public:
  static constexpr int BUF_SZ = 256;
  char buffer[BUF_SZ];

  BufferedInputBox(Group& target, std::string_view id, SDL_Rect r)
    : target(target)
    , id(id)
    , rect(makeInputSize(r))
  {
    bool clicked = target.checkMouse(id, rect) == MouseAction::GRAB;
    active = target.isActive(id);
    refillBuffer = !active || clicked;
  }

  bool wantsRefill() const { return refillBuffer; }

  bool end()
  {
    if (!active) {
      textBox(target, id, buffer, BUF_SZ, rect);
      return false;
    }
    static char editBuffer[BUF_SZ];
    if (refillBuffer) {
      SDL_strlcpy(editBuffer, buffer, BUF_SZ);
    }
    if (!textBox(target, id, editBuffer, BUF_SZ, rect)) {
      return false;
    }
    SDL_strlcpy(buffer, editBuffer, BUF_SZ);
    return true;
  }
};

// A intBox
inline bool
intBox(Group& target, std::string_view id, int* value, SDL_Rect r = {0})
{
  SDL_assert(value != nullptr);
  BufferedInputBox bufferedBox{target, id, r};
  if (bufferedBox.wantsRefill()) {
    SDL_itoa(*value, bufferedBox.buffer, 10);
  }
  if (bufferedBox.end()) {
    auto newValue = SDL_atoi(bufferedBox.buffer);
    if (newValue != *value) {
      *value = newValue;
      return true;
    }
  }
  return false;
}

// A doubleBox
inline bool
doubleBox(Group& target, std::string_view id, double* value, SDL_Rect r = {0})
{
  SDL_assert(value != nullptr);
  BufferedInputBox bufferedBox{target, id, r};
  if (bufferedBox.wantsRefill()) {
    auto text = bufferedBox.buffer;
    int n = SDL_snprintf(text, bufferedBox.BUF_SZ, "%f", *value);
    for (int i = n - 1; i > 0; --i) {
      if (text[i] != '0' && text[i] != '.') {
        text[i + 1] = 0;
        break;
      }
    }
  }
  if (bufferedBox.end()) {
    auto newValue = SDL_strtod(bufferedBox.buffer, nullptr);
    if (newValue != *value) {
      *value = newValue;
      return true;
    }
  }
  return false;
}

// begin label.hpp
/// The label style
struct LabelStyle
{
  SDL_Color text;
  EdgeSize padding;

  constexpr LabelStyle withText(SDL_Color c) const
  {
    auto next = *this;
    next.text = c;
    return next;
  }

  constexpr LabelStyle withMargin(EdgeSize sz) const
  {
    auto next = *this;
    next.padding = sz;
    return next;
  }
};

namespace style {
constexpr LabelStyle LABEL{TEXT, EdgeSize::all(2)};
} // namespace style

/**
 * @brief
 *
 * @param target the parent group or frame
 * @param str the text to show
 * @param p the local relative postion to add the label. Its size is
 * automatically evaluated
 * @param style
 */
inline void
label(Group& target,
      std::string_view str,
      const SDL_Point& p = {0},
      const LabelStyle& style = style::LABEL)
{
  auto adv = measure(str);
  auto margin = style.padding;
  SDL_Rect r{p.x,
             p.y,
             adv.x + margin.left + margin.right,
             adv.y + margin.top + margin.bottom};
  auto g = group(target, {}, r, Layout::NONE);
  text(g, str, {margin.left, margin.top}, style.text);
  g.end();
}

// begin inputField.hpp
inline Group
labeledGroup(Group& target,
             std::string_view labelText,
             const SDL_Rect& clientRect)
{
  SDL_Rect r{clientRect};
  SDL_Point labelPos = {r.w + 1, 0};
  r.w += measure(labelText).x + 1;

  auto g = group(target, {}, r, Layout::NONE);
  label(g, labelText, labelPos);
  return g;
}

inline bool
textField(Group& target,
          std::string_view id,
          std::string_view labelText,
          char* value,
          size_t maxSize,
          const SDL_Point& p = {0})
{
  SDL_Rect box{makeInputSize({p.x, p.y, 0, 0})};
  auto g = labeledGroup(target, labelText, box);
  auto changed = textBox(g, id, value, maxSize, box);
  g.end();
  return changed;
}

inline bool
textField(Group& target,
          std::string_view id,
          char* value,
          size_t maxSize,
          const SDL_Point& p = {0})
{
  return textField(target, id, id, value, maxSize, p);
}

inline bool
textField(Group& target,
          std::string_view id,
          std::string_view labelText,
          std::string* value,
          const SDL_Point& p = {0})
{
  SDL_Rect box{makeInputSize({p.x, p.y, 0, 0})};
  auto g = labeledGroup(target, labelText, box);
  auto changed = textBox(g, id, value, box);
  g.end();
  return changed;
}

inline bool
textField(Group& target,
          std::string_view id,
          std::string* value,
          const SDL_Point& p = {0})
{
  return textField(target, id, id, value, p);
}

inline bool
intField(Group& target,
         std::string_view id,
         std::string_view labelText,
         int* value,
         const SDL_Point& p = {0})
{
  SDL_Rect box{makeInputSize({p.x, p.y, 0, 0})};
  auto g = labeledGroup(target, labelText, box);
  auto changed = intBox(g, id, value, box);
  g.end();
  return changed;
}

inline bool
intField(Group& target,
         std::string_view id,
         int* value,
         const SDL_Point& p = {0})
{
  return intField(target, id, id, value, p);
}

inline bool
doubleField(Group& target,
            std::string_view id,
            std::string_view labelText,
            double* value,
            const SDL_Point& p = {0})
{
  SDL_Rect box{makeInputSize({p.x, p.y, 0, 0})};
  auto g = labeledGroup(target, labelText, box);
  auto changed = doubleBox(g, id, value, box);
  g.end();
  return changed;
}

inline bool
doubleField(Group& target,
            std::string_view id,
            double* value,
            const SDL_Point& p = {0})
{
  return doubleField(target, id, id, value, p);
}

// begin dui.hpp

} // namespace dui

#endif // DUI_SINGLE_HPP
