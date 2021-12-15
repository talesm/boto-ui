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

TEST_CASE("EventDispatcher hovers only one element per turn",
          "[event-dispatcher]")
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

inline bool
operator==(const SDL_Rect& lhs, const SDL_Rect& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y && lhs.w == rhs.w && lhs.h == rhs.h;
}

TEST_CASE("EventDispatcher EventTarget stacks", "[event-dispatcher]")
{
  EventDispatcher dispatcher{};
  dispatcher.movePointer({0, 0});

  dispatcher.reset();

  SECTION("sub EventTarget is intersected by super target")
  {
    if (auto target = dispatcher.check(RequestEvent::NONE, {0, 0, 2, 2})) {
      REQUIRE(target.rect == SDL_Rect{0, 0, 2, 2});
      REQUIRE(dispatcher.check(RequestEvent::NONE, {1, 1, 3, 3}).rect ==
              SDL_Rect{1, 1, 1, 1});
    }
    REQUIRE(dispatcher.check(RequestEvent::NONE, {1, 1, 3, 3}).rect ==
            SDL_Rect{1, 1, 3, 3});
  }
}
