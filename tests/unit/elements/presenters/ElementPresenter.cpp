#include "catch.hpp"
#include <cstring>
#include "core/DisplayList.hpp"
#include "elements/presenters/ElementPresenter.hpp"

using namespace std;
using namespace boto;

TEST_CASE("Color Presenter")
{
  DisplayList dList;
  SDL_Color style{1, 2, 3, 4};
  REQUIRE(measure(style) == SDL_Point{0, 0});
  present(dList, {0, 0, 10, 10}, style);
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
  auto style = (SDL_Texture*)0xdeadbeef;
  REQUIRE(measure(style) == SDL_Point{0, 0});
  present(dList, {0, 0, 10, 10}, style);
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
  present(dList,
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
  present(dList,
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
  present(dList,
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
    present(dList, {0, 0, 10, 10}, theme.of<BackgroundColor>());
    auto c = dList.visit([&](const DisplayListItem& el) {
      REQUIRE(el.action == DisplayListAction::COLOR_BOX);
      REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
      REQUIRE(el.color == SDL_Color{255, 255, 255, 255});
    });
    REQUIRE(c == 1);
  }
  SECTION("Border Color")
  {
    present(dList, {0, 0, 10, 10}, theme.of<BorderColor>());
    auto c = dList.visit([&](const DisplayListItem& el) {
      REQUIRE(el.action == DisplayListAction::COLOR_BOX);
      REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
      REQUIRE(el.color == SDL_Color{0, 0, 0, 255});
    });
    REQUIRE(c == 1);
  }
  SECTION("Border Color")
  {
    REQUIRE(measure(theme.of<Element>()) == SDL_Point{2, 2});
    present(dList, {0, 0, 10, 10}, theme.of<Element>());
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
    present(dList,
            {0, 0, 10, 10},
            theme.of<Element>().withBorderSize(EdgeSize::all(0)));
    auto c = dList.visit([&](const DisplayListItem& el) {
      REQUIRE(el.action == DisplayListAction::COLOR_BOX);
      REQUIRE(el.color == SDL_Color{255, 255, 255, 255});
      REQUIRE(el.rect == SDL_Rect{0, 0, 10, 10});
    });
    REQUIRE(c == 1);
  }
}

struct DummyElement;

namespace boto {
template<>
struct StyleFor<DummyThemeType, DummyElement>
{
  static constexpr int get(DummyTheme&) { return 0; }
};
template<class T>
struct StyleFor<DummyThemeType, Hovered<T>>
{
  static constexpr int get(DummyTheme& t) { return t.template of<T>() | 1; }
};
template<class T>
struct StyleFor<DummyThemeType, Grabbed<T>>
{
  static constexpr int get(DummyTheme& t) { return t.template of<T>() | 2; }
};
template<class T>
struct StyleFor<DummyThemeType, Focused<T>>
{
  static constexpr int get(DummyTheme& t) { return t.template of<T>() | 4; }
};
} // namespace boto

TEST_CASE("Ensure correctly handling of status")
{
  DummyTheme theme;
  REQUIRE((elementStyle<DummyElement>(theme, Status::NONE)) == 0);
  REQUIRE((elementStyle<DummyElement>(theme, Status::HOVERED)) == 1);
  REQUIRE((elementStyle<DummyElement>(theme, Status::GRABBED)) == 2);
  REQUIRE((elementStyle<DummyElement>(theme,
                                      Status::GRABBED | Status::HOVERED)) == 3);
  REQUIRE((elementStyle<DummyElement>(theme, Status::FOCUSED)) == 4);
  REQUIRE((elementStyle<DummyElement>(theme,
                                      Status::FOCUSED | Status::HOVERED)) == 5);
  REQUIRE((elementStyle<DummyElement>(theme,
                                      Status::FOCUSED | Status::GRABBED)) == 6);
  REQUIRE((elementStyle<DummyElement>(
            theme, Status::FOCUSED | Status::GRABBED | Status::HOVERED)) == 7);
}
