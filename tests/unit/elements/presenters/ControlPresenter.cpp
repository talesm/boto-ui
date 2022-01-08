#include "catch.hpp"
#include <cstring>
#include "core/DisplayList.hpp"
#include "elements/presenters/ControlPresenter.hpp"

using namespace std;
using namespace boto;

struct DummyThemeType;
using DummyTheme = ThemeT<DummyThemeType>;

TEST_CASE("Default Control Style", "[presenters][control]")
{
  DisplayList dList;
  SECTION("ControlStyleT")
  {
    SECTION("With only background & defined")
    {
      using ControlStyle = ControlStyleT<SDL_Color>;
      present(dList, {}, {0, 0, 10, 10}, ControlStyle{{1, 2, 3, 4}});
      auto c = dList.visit([&](const DisplayListItem& el) {
        REQUIRE(el.action == DisplayListAction::COLOR_BOX);
        REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
        REQUIRE(el.color == SDL_Color{1, 2, 3, 4});
      });
      REQUIRE(c == 1);
    }
    SECTION("With only background & undefined")
    {
      using ControlStyle = ControlStyleT<SDL_Color>;
      present(
        dList, {}, {0, 0, Undefined, Undefined}, ControlStyle{{1, 2, 3, 4}});
      auto c = dList.visit([&](const DisplayListItem& el) {});
      REQUIRE(c == 0);
    }
  }
  // SECTION("Border Color")
  // {
  //   present(dList, {0, 0, 10, 10}, theme.of<BorderColor>());
  //   auto c = dList.visit([&](const DisplayListItem& el) {
  //     REQUIRE(el.action == DisplayListAction::COLOR_BOX);
  //     REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
  //     REQUIRE(el.color == SDL_Color{0, 0, 0, 255});
  //   });
  //   REQUIRE(c == 1);
  // }
  // SECTION("Border Color")
  // {
  //   present(dList, {0, 0, 10, 10}, theme.of<Element>());
  //   int c = 0;
  //   dList.visit([&](const DisplayListItem& el) {
  //     REQUIRE(el.action == DisplayListAction::COLOR_BOX);
  //     if (c == 0) {
  //       REQUIRE(el.color == SDL_Color{255, 255, 255, 255});
  //       REQUIRE(el.rect == SDL_Rect{1, 1, 8, 8});
  //     } else {
  //       REQUIRE(el.color == SDL_Color{0, 0, 0, 255});
  //       switch (c) {
  //       case 1:
  //         REQUIRE(el.rect == SDL_Rect{0, 1, 1, 8});
  //         break;
  //       case 2:
  //         REQUIRE(el.rect == SDL_Rect{1, 0, 8, 1});
  //         break;
  //       case 3:
  //         REQUIRE(el.rect == SDL_Rect{9, 1, 1, 8});
  //         break;
  //       case 4:
  //         REQUIRE(el.rect == SDL_Rect{1, 9, 8, 1});
  //         break;
  //       }
  //     }
  //     c++;
  //   });
  //   REQUIRE(c == 5);
  // }
  // SECTION("Border 0 Color")
  // {
  //   present(dList,
  //                  {0, 0, 10, 10},
  //                  theme.of<Element>().withBorderSize(EdgeSize::all(0)));
  //   auto c = dList.visit([&](const DisplayListItem& el) {
  //     REQUIRE(el.action == DisplayListAction::COLOR_BOX);
  //     REQUIRE(el.color == SDL_Color{255, 255, 255, 255});
  //     REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
  //   });
  //   REQUIRE(c == 1);
  // }
}
