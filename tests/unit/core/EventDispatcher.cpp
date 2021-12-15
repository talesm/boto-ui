#include "catch.hpp"
#include "core/EventDispatcher.hpp"

using namespace boto;

TEST_CASE("EventDispatcher hover handling", "[event-dispatcher]")
{
  EventDispatcher dispatcher{};
  dispatcher.movePointer({0, 0});

  dispatcher.reset();
  REQUIRE_FALSE(dispatcher.check(RequestEvent::NONE, {0, 0, 2, 2}).status &
                STATUS_HOVERED);
  dispatcher.reset();
  REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status &
          STATUS_HOVERED);
  dispatcher.reset();
  REQUIRE_FALSE(dispatcher.check(RequestEvent::HOVER, {1, 1, 2, 2}).status &
                STATUS_HOVERED);
  dispatcher.reset();
}

TEST_CASE("EventDispatcher hovers only one element per turn")
{
  EventDispatcher dispatcher{};
  dispatcher.movePointer({0, 0});

  dispatcher.reset();
  REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status &
          STATUS_HOVERED);
  REQUIRE_FALSE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status &
                STATUS_HOVERED);
  dispatcher.reset();
  REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status &
          STATUS_HOVERED);
}
