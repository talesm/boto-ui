#include "catch.hpp"
#include "core/EventDispatcher.hpp"

using namespace boto;

TEST_CASE("EventDispatcher hover handling", "[event-dispatcher")
{
  EventDispatcher dispatcher{};
  dispatcher.movePointer({0, 0});

  REQUIRE_FALSE(dispatcher.check(RequestEvent::NONE, {0, 0, 2, 2}).status &
                STATUS_HOVERED);
  REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status &
          STATUS_HOVERED);
  REQUIRE_FALSE(dispatcher.check(RequestEvent::HOVER, {1, 1, 2, 2}).status &
                STATUS_HOVERED);
}
