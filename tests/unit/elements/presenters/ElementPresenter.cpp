#include "catch.hpp"
#include <cstring>
#include "core/DisplayList.hpp"
#include "elements/presenters/ElementPresenter.hpp"

using namespace std;
using namespace boto;

TEST_CASE("Color Presenter")
{
  DisplayList dList;
  presentElement(dList, {0, 0, 10, 10}, SDL_Color{1, 2, 3, 4});
  auto c = dList.visit([&](const DisplayListItem& el) {
    REQUIRE(el.action == DisplayListAction::COLOR_BOX);
    REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
    REQUIRE(el.color == SDL_Color{1, 2, 3, 4});
  });
  REQUIRE(c == 1);
}

TEST_CASE("Texture Presenter")
{
  DisplayList dList;
  presentElement(dList, {0, 0, 10, 10}, (SDL_Texture*)0xdeadbeef);
  auto c = dList.visit([&](const DisplayListItem& el) {
    REQUIRE(el.action == DisplayListAction::TEXTURE_BOX);
    REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
    REQUIRE(el.texture == (SDL_Texture*)0xdeadbeef);
    REQUIRE(el.color == SDL_Color{255, 255, 255, 255});
  });
  REQUIRE(c == 1);
}

TEST_CASE("Tinted Texture Presenter")
{
  DisplayList dList;
  presentElement(dList,
                 {0, 0, 10, 10},
                 TintedTextureStyle{(SDL_Texture*)0xdeadbeef, {1, 2, 3, 4}});
  auto c = dList.visit([&](const DisplayListItem& el) {
    REQUIRE(el.action == DisplayListAction::TEXTURE_BOX);
    REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
    REQUIRE(el.texture == (SDL_Texture*)0xdeadbeef);
    REQUIRE(el.color == SDL_Color{1, 2, 3, 4});
  });
  REQUIRE(c == 1);
}

TEST_CASE("Partial Texture Presenter")
{
  DisplayList dList;
  presentElement(dList,
                 {0, 0, 10, 10},
                 PartialTextureStyle{(SDL_Texture*)0xdeadbeef, {1, 2, 3, 4}});
  auto c = dList.visit([&](const DisplayListItem& el) {
    REQUIRE(el.action == DisplayListAction::PARTIAL_TEXTURE_BOX);
    REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
    REQUIRE(el.texture == (SDL_Texture*)0xdeadbeef);
    REQUIRE(el.color == SDL_Color{255, 255, 255, 255});
    REQUIRE(el.srcRect == SDL_Rect{1, 2, 3, 4});
  });
  REQUIRE(c == 1);
}

TEST_CASE("Tinted Partial Texture Presenter")
{
  DisplayList dList;
  presentElement(dList,
                 {0, 0, 10, 10},
                 PartialTextureStyle{
                   (SDL_Texture*)0xdeadbeef,
                   {1, 2, 3, 4},
                   {5, 6, 7, 8},
                 });
  auto c = dList.visit([&](const DisplayListItem& el) {
    REQUIRE(el.action == DisplayListAction::PARTIAL_TEXTURE_BOX);
    REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
    REQUIRE(el.texture == (SDL_Texture*)0xdeadbeef);
    REQUIRE(el.color == SDL_Color{1, 2, 3, 4});
    REQUIRE(el.srcRect == SDL_Rect{5, 6, 7, 8});
  });
  REQUIRE(c == 1);
}
