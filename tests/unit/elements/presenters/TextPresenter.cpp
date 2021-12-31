#include "catch.hpp"
#include <cstring>
#include "core/DisplayList.hpp"
#include "elements/presenters/TextPresenter.hpp"

using namespace std;
using namespace boto;

struct DummyThemeType;
using DummyTheme = ThemeT<DummyThemeType>;

TEST_CASE("TextPresenter defaults")
{
  DummyTheme theme;
  theme.set<Font>(Font{nullptr, 8, 8, 16});
  DisplayList dList;
  SECTION("Single null char")
  {
    presentText(dList, '\0', {0, 0}, theme.of<Text>());
    auto c = dList.visit([&](const DisplayListItem& el) {
      REQUIRE(el.action == DisplayListAction::PARTIAL_TEXTURE_BOX);
      REQUIRE(el.rect == SDL_Rect{0, 0, 8, 8});
      REQUIRE(el.srcRect == SDL_Rect{0, 0, 8, 8});
      REQUIRE(el.color == SDL_Color{0, 0, 0, 255});
    });
    REQUIRE(c == 1);
  }
  SECTION("Single space char")
  {
    presentText(dList, ' ', {0, 0}, theme.of<Text>());
    auto c = dList.visit([&](const DisplayListItem& el) {
      REQUIRE(el.action == DisplayListAction::PARTIAL_TEXTURE_BOX);
      REQUIRE(el.rect == SDL_Rect{0, 0, 8, 8});
      REQUIRE(el.srcRect == SDL_Rect{0, 16, 8, 8});
      REQUIRE(el.color == SDL_Color{0, 0, 0, 255});
    });
    REQUIRE(c == 1);
  }
  SECTION("Empty")
  {
    presentText(dList, "", {0, 0}, theme.of<Text>());
    auto c = dList.visit([&](const DisplayListItem& el) {});
    REQUIRE(c == 0);
  }
}
