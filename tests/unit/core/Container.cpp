#include "catch.hpp"
#include "core/Container.hpp"

using namespace boto;
using namespace std::literals;

TEST_CASE("Container creation", "[container]")
{
  EventDispatcher dispatcher;
  DisplayList dList;

  dispatcher.reset();
  dispatcher.movePointer({2, 2});

  SECTION("Common container")
  {
    ContainerState c{dList, dispatcher, "c1"sv, {1, 2, 3, 4}};
    REQUIRE(c.target.rect().x == 1);
    REQUIRE(c.target.rect().y == 2);
    REQUIRE(c.target.rect().w == 3);
    REQUIRE(c.target.rect().h == 4);
    REQUIRE(c.target.status() == Status::HOVERED);
    REQUIRE(c.offset.x == 1);
    REQUIRE(c.offset.y == 2);
    REQUIRE(c.endPos.x == 1);
    REQUIRE(c.endPos.y == 2);
    REQUIRE(c.caret().x == 1);
    REQUIRE(c.caret().y == 2);
    REQUIRE(c.size().x == 3);
    REQUIRE(c.size().y == 4);
  }
  SECTION("Infinite container")
  {
    ContainerState c{dList, dispatcher, "c1"sv, {1, 2, Undefined, Undefined}};
    REQUIRE(c.target.rect().x == 1);
    REQUIRE(c.target.rect().y == 2);
    REQUIRE(c.target.status() == Status::HOVERED);
    REQUIRE(c.offset.x == 1);
    REQUIRE(c.offset.y == 2);
    REQUIRE(c.endPos.x == 1);
    REQUIRE(c.endPos.y == 2);
    REQUIRE(c.caret().x == 1);
    REQUIRE(c.caret().y == 2);
    REQUIRE(c.size().x == 0);
    REQUIRE(c.size().y == 0);
    c.endPos.x *= 2;
    c.endPos.y *= 2;
    REQUIRE(c.size().x == 1);
    REQUIRE(c.size().y == 2);
  }
  SECTION("Offset container")
  {
    ContainerState c{dList, dispatcher, "c1"sv, {1, 2, 3, 4}, {5, 6}};
    REQUIRE(c.target.rect().x == 1);
    REQUIRE(c.target.rect().y == 2);
    REQUIRE(c.target.rect().w == 3);
    REQUIRE(c.target.rect().h == 4);
    REQUIRE(c.target.status() == Status::HOVERED);
    REQUIRE(c.offset.x == 6);
    REQUIRE(c.offset.y == 8);
    REQUIRE(c.endPos.x == 6);
    REQUIRE(c.endPos.y == 8);
    REQUIRE(c.caret().x == 6);
    REQUIRE(c.caret().y == 8);
    REQUIRE(c.size().x == 3);
    REQUIRE(c.size().y == 4);
  }
  SECTION("Offset infinite container")
  {
    ContainerState c{
      dList, dispatcher, "c1"sv, {1, 2, Undefined, Undefined}, {5, 6}};
    REQUIRE(c.target.rect().x == 1);
    REQUIRE(c.target.rect().y == 2);
    REQUIRE(c.target.status() == Status::HOVERED);
    REQUIRE(c.offset.x == 6);
    REQUIRE(c.offset.y == 8);
    REQUIRE(c.endPos.x == 6);
    REQUIRE(c.endPos.y == 8);
    REQUIRE(c.caret().x == 6);
    REQUIRE(c.caret().y == 8);
    REQUIRE(c.size().x == 5);
    REQUIRE(c.size().y == 6);
  }
  SECTION("Negative offset infinite container")
  {
    ContainerState c{
      dList, dispatcher, "c1"sv, {1, 2, Undefined, Undefined}, {-5, -6}};
    REQUIRE(c.target.rect().x == 1);
    REQUIRE(c.target.rect().y == 2);
    REQUIRE(c.target.status() == Status::HOVERED);
    REQUIRE(c.offset.x == -4);
    REQUIRE(c.offset.y == -4);
    REQUIRE(c.endPos.x == -4);
    REQUIRE(c.endPos.y == -4);
    REQUIRE(c.caret().x == -4);
    REQUIRE(c.caret().y == -4);
    REQUIRE(c.size().x == 0);
    REQUIRE(c.size().y == 0);
    c.endPos = {3, 4};
    REQUIRE(c.size().x == 2);
    REQUIRE(c.size().y == 2);
  }
  int i = 0;
  dList.visit([&](const DisplayListItem& item) { ++i; });

  REQUIRE(i == 2);
}

TEST_CASE("Container advance", "[container]")
{
  EventDispatcher dispatcher;
  DisplayList dList;

  SECTION("Layout::NONE")
  {
    ContainerState c{
      dList, dispatcher, "c1"sv, {1, 2, 3, 4}, {5, 6}, {}, Layout::NONE};
    REQUIRE(c.offset.x == c.endPos.x);
    REQUIRE(c.offset.y == c.endPos.y);
    REQUIRE(c.offset.x == c.caret().x);
    REQUIRE(c.offset.y == c.caret().y);

    c.advance({1, 1});
    REQUIRE(c.offset.x == c.caret().x);
    REQUIRE(c.offset.y == c.caret().y);
    REQUIRE(c.endPos.x == 7);
    REQUIRE(c.endPos.y == 9);

    c.advance({2, 2});
    REQUIRE(c.endPos.x == 8);
    REQUIRE(c.endPos.y == 10);

    c.advance({1, 1});
    REQUIRE(c.endPos.x == 8);
    REQUIRE(c.endPos.y == 10);
  }
  SECTION("Layout::HORIZONTAL")
  {
    ContainerState c{dList,
                     dispatcher,
                     "c1"sv,
                     {1, 2, 3, 4},
                     {5, 6},
                     {},
                     Layout::HORIZONTAL,
                     0};
    REQUIRE(c.offset.x == c.endPos.x);
    REQUIRE(c.offset.y == c.endPos.y);
    REQUIRE(c.offset.x == c.caret().x);
    REQUIRE(c.offset.y == c.caret().y);

    c.advance({1, 1});
    REQUIRE(c.offset.x == c.caret().x);
    REQUIRE(c.offset.y == c.caret().y);
    REQUIRE(c.endPos.x == 7);
    REQUIRE(c.endPos.y == 9);

    c.advance({2, 2});
    REQUIRE(c.endPos.x == 9);
    REQUIRE(c.endPos.y == 10);

    c.elementSpacing = 2;

    c.advance({1, 1});
    REQUIRE(c.endPos.x == 12);
    REQUIRE(c.endPos.y == 10);
  }
  SECTION("Layout::VERTICAL")
  {
    ContainerState c{
      dList, dispatcher, "c1"sv, {1, 2, 3, 4}, {5, 6}, {}, Layout::VERTICAL, 0};
    REQUIRE(c.offset.x == c.endPos.x);
    REQUIRE(c.offset.y == c.endPos.y);
    REQUIRE(c.offset.x == c.caret().x);
    REQUIRE(c.offset.y == c.caret().y);

    c.advance({1, 1});
    REQUIRE(c.offset.x == c.caret().x);
    REQUIRE(c.offset.y == c.caret().y);
    REQUIRE(c.endPos.x == 7);
    REQUIRE(c.endPos.y == 9);

    c.advance({2, 2});
    REQUIRE(c.endPos.x == 8);
    REQUIRE(c.endPos.y == 11);

    c.elementSpacing = 2;

    c.advance({1, 1});
    REQUIRE(c.endPos.x == 8);
    REQUIRE(c.endPos.y == 14);
  }
}

TEST_CASE("Frame on regular container", "[container][frame]")
{
  State state{nullptr};
  Frame f = frame(state);

  auto c1 = f.container("c1"sv, {1, 2, 3, 4});
  {
    auto& s1 = c1.state();
    REQUIRE(s1.offset.x == 1);
    REQUIRE(s1.offset.y == 2);
    REQUIRE(s1.endPos.x == 1);
    REQUIRE(s1.endPos.y == 2);

    SECTION("Child inherits the offset")
    {
      {
        auto c2 = c1.container("c2"sv, {0, 0, 2, 2});
        auto& s2 = c2.state();
        REQUIRE(s2.offset.x == 1);
        REQUIRE(s2.offset.y == 2);
      }
      auto& s1 = c1.state();
      REQUIRE(s1.offset.x == 1);
      REQUIRE(s1.offset.y == 2);
      REQUIRE(s1.endPos.x == 3);
      REQUIRE(s1.endPos.y == 4);
    }
  }
}
TEST_CASE("Frame with regular checking", "[container][frame]")
{
  State state{nullptr};
  Frame f = frame(state);

  auto c1 = f.container("c1"sv, {1, 2, 3, 4});
  {
    auto& s1 = c1.state();
    REQUIRE(s1.offset.x == 1);
    REQUIRE(s1.offset.y == 2);
    REQUIRE(s1.endPos.x == 1);
    REQUIRE(s1.endPos.y == 2);

    SECTION("Child inherits the offset")
    {
      {
        auto c2 = c1.container("c2"sv, {0, 0, Undefined, Undefined});
        auto& s2 = c2.state();
        REQUIRE(s2.offset.x == 1);
        REQUIRE(s2.offset.y == 2);
        REQUIRE(s2.endPos.x == 1);
        REQUIRE(s2.endPos.y == 2);
        auto state = c2.element("el3"sv, {0, 0, 2, 2});
        REQUIRE(s2.endPos.x == 3);
        REQUIRE(s2.endPos.y == 4);
        REQUIRE(state.rect.x == 1);
        REQUIRE(state.rect.y == 2);
      }
      auto& s1 = c1.state();
      REQUIRE(s1.offset.x == 1);
      REQUIRE(s1.offset.y == 2);
      REQUIRE(s1.endPos.x == 3);
      REQUIRE(s1.endPos.y == 4);
    }
  }
}
