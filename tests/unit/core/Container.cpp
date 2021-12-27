#include "catch.hpp"
#include "core/Container.hpp"
#include "elements/Target.hpp"

using namespace boto;
using namespace std::literals;

TEST_CASE("Frame on regular container", "[container][frame]")
{
  State state{nullptr};
  Frame f = frame(state);

  auto c1 = Target{f}.container("c1"sv, {1, 2, 3, 4});
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

  auto c1 = Target{f}.container("c1"sv, {1, 2, 3, 4});
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
