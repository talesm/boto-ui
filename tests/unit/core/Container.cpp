#include "catch.hpp"
#include "core/Container.hpp"

using namespace boto;
using namespace std::literals;

TEST_CASE("Frame on regular container", "[container][frame]")
{
  State state{nullptr};
  Frame f = frame(state);

  Container c1{f, "c1"sv, {1, 2, 3, 4}};
  auto& s1 = c1.state();
  REQUIRE(s1.offset.x == 1);
  REQUIRE(s1.offset.y == 2);
  REQUIRE(s1.endPos.x == 1);
  REQUIRE(s1.endPos.y == 2);

  SECTION("Child inherits the offset")
  {
    {
      Container c2{c1, "c2"sv, {0, 0, 2, 2}};
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
TEST_CASE("Frame with regular checking", "[container][frame]")
{
  State state{nullptr};
  Frame f = frame(state);

  Container c1{f, "c1"sv, {1, 2, 3, 4}};
  auto& s1 = c1.state();
  REQUIRE(s1.offset.x == 1);
  REQUIRE(s1.offset.y == 2);
  REQUIRE(s1.endPos.x == 1);
  REQUIRE(s1.endPos.y == 2);

  SECTION("Child inherits the offset")
  {
    {
      Container c2{c1, "c2"sv, {0, 0, Undefined, Undefined}};
      {
        auto& s2 = c2.state();
        REQUIRE(s2.offset.x == 1);
        REQUIRE(s2.offset.y == 2);
        REQUIRE(s2.endPos.x == 1);
        REQUIRE(s2.endPos.y == 2);
      }
      auto state =
        Container{c2, "el3"sv, {0, 0, 2, 2}}.state().eventTarget.state();
      auto& s2 = c2.state();
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
