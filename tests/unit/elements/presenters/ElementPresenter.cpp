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

struct DummyThemeType;
using DummyTheme = ThemeT<DummyThemeType>;
TEST_CASE("Default Presenter")
{
  DummyTheme theme;
  DisplayList dList;
  SECTION("Background Color")
  {
    presentElement(dList, {0, 0, 10, 10}, theme.of<BackgroundColor>());
    auto c = dList.visit([&](const DisplayListItem& el) {
      REQUIRE(el.action == DisplayListAction::COLOR_BOX);
      REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
      REQUIRE(el.color == SDL_Color{255, 255, 255, 255});
    });
    REQUIRE(c == 1);
  }
  SECTION("Border Color")
  {
    presentElement(dList, {0, 0, 10, 10}, theme.of<BorderColor>());
    auto c = dList.visit([&](const DisplayListItem& el) {
      REQUIRE(el.action == DisplayListAction::COLOR_BOX);
      REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
      REQUIRE(el.color == SDL_Color{0, 0, 0, 255});
    });
    REQUIRE(c == 1);
  }
  SECTION("Border Color")
  {
    presentElement(dList, {0, 0, 10, 10}, theme.of<Element>());
    int c = 0;
    dList.visit([&](const DisplayListItem& el) {
      REQUIRE(el.action == DisplayListAction::COLOR_BOX);
      if (c == 0) {
        REQUIRE(el.color == SDL_Color{255, 255, 255, 255});
        REQUIRE(el.rect == SDL_Rect{1, 1, 8, 8});
      } else {
        REQUIRE(el.color == SDL_Color{0, 0, 0, 255});
        switch (c) {
        case 1:
          REQUIRE(el.rect == SDL_Rect{0, 1, 1, 8});
          break;
        case 2:
          REQUIRE(el.rect == SDL_Rect{1, 0, 8, 1});
          break;
        case 3:
          REQUIRE(el.rect == SDL_Rect{9, 1, 1, 8});
          break;
        case 4:
          REQUIRE(el.rect == SDL_Rect{1, 9, 8, 1});
          break;
        }
      }
      c++;
    });
    REQUIRE(c == 5);
  }
  SECTION("Border 0 Color")
  {
    presentElement(dList,
                   {0, 0, 10, 10},
                   theme.of<Element>().withBorderSize(EdgeSize::all(0)));
    auto c = dList.visit([&](const DisplayListItem& el) {
      REQUIRE(el.action == DisplayListAction::COLOR_BOX);
      // if (c == 0) {
      //   REQUIRE(el.color == SDL_Color{255, 255, 255, 255});
      //   REQUIRE(el.rect == SDL_Rect{1, 1, 8, 8});
      // } else {
      //   REQUIRE(el.color == SDL_Color{0, 0, 0, 255});
      //   switch (c) {
      //   case 1:
      //     REQUIRE(el.rect == SDL_Rect{0, 1, 1, 8});
      //     break;
      //   case 2:
      //     REQUIRE(el.rect == SDL_Rect{1, 0, 8, 1});
      //     break;
      //   case 3:
      //     REQUIRE(el.rect == SDL_Rect{9, 1, 1, 8});
      //     break;
      //   case 4:
      //     REQUIRE(el.rect == SDL_Rect{1, 9, 8, 1});
      //     break;
      //   }
      // }
      // c++;
    });
    REQUIRE(c == 1);
  }
}
