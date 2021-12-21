#include "catch.hpp"
#include "core/State.hpp"

using namespace boto;

TEST_CASE("State frame lifecycle", "[state]")
{
  State state{nullptr};

  REQUIRE(state.isInFrame() == false);

  {
    auto frame = state.frame();
    REQUIRE(state.isInFrame() == true);
  }
  REQUIRE(state.isInFrame() == false);
}
