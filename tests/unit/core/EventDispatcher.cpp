#include "catch.hpp"
#include "core/EventDispatcher.hpp"

using namespace boto;
using namespace std;

TEST_CASE("EventDispatcher hover handling", "[event-dispatcher]")
{
  EventDispatcher dispatcher{};
  dispatcher.movePointer({0, 0});

  dispatcher.reset();
  REQUIRE_FALSE(dispatcher.check(RequestEvent::NONE, {0, 0, 2, 2}).status() &
                STATUS_HOVERED);
  dispatcher.reset();
  REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() &
          STATUS_HOVERED);
  dispatcher.reset();
  REQUIRE_FALSE(dispatcher.check(RequestEvent::HOVER, {1, 1, 2, 2}).status() &
                STATUS_HOVERED);
  dispatcher.reset();
}

TEST_CASE("EventDispatcher hovers only one element per turn",
          "[event-dispatcher]")
{
  EventDispatcher dispatcher{};
  dispatcher.movePointer({0, 0});

  dispatcher.reset();
  REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() &
          STATUS_HOVERED);
  REQUIRE_FALSE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() &
                STATUS_HOVERED);
  dispatcher.reset();
  REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() &
          STATUS_HOVERED);
}

TEST_CASE("EventDispatcher grab handling", "[event-dispatcher]")
{
  EventDispatcher dispatcher{};
  dispatcher.reset();
  dispatcher.movePointer({0, 0});

  SECTION("The status is not changed before press")
  {
    REQUIRE_FALSE(
      dispatcher.check(RequestEvent::ACTION, {1, 1, 1, 1}, "id0"sv).status());
    REQUIRE(
      dispatcher.check(RequestEvent::ACTION, {0, 0, 1, 1}, "id0"sv).status() ==
      STATUS_HOVERED);
  }

  dispatcher.reset();
  dispatcher.pressPointer(0);

  SECTION("grab an element")
  {
    REQUIRE_FALSE(
      dispatcher.check(RequestEvent::ACTION, {1, 1, 1, 1}, "id0"sv).status());
    auto target = dispatcher.check(RequestEvent::ACTION, {0, 0, 1, 1}, "id1"sv);
    REQUIRE(target.status() == (STATUS_HOVERED | STATUS_GRABBED));
    REQUIRE(target.event() == Event::GRAB);

    dispatcher.reset();
    target = dispatcher.check(RequestEvent::ACTION, {0, 0, 1, 1}, "id1"sv);
    REQUIRE(target.status() == (STATUS_HOVERED | STATUS_GRABBED));
    REQUIRE(target.event() == Event::NONE);

    dispatcher.reset();
    SECTION("release while still hovering")
    {
      dispatcher.releasePointer(0);
      target = dispatcher.check(RequestEvent::ACTION, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.status() == STATUS_HOVERED);
      REQUIRE(target.event() == Event::ACTION);
    }
    SECTION("release after moving out")
    {
      dispatcher.movePointer({2, 2});
      target = dispatcher.check(RequestEvent::ACTION, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.status() == 0);
      REQUIRE(target.event() == Event::NONE);

      dispatcher.reset();
      dispatcher.releasePointer(0);
      target = dispatcher.check(RequestEvent::ACTION, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.status() == 0);
      REQUIRE(target.event() == Event::CANCEL);
    }
    SECTION("pressing another button")
    {
      dispatcher.reset();
      dispatcher.pressPointer(1);
      target = dispatcher.check(RequestEvent::ACTION, {0, 0, 1, 1}, "id1"sv);
      REQUIRE_FALSE(target.status() & STATUS_GRABBED);
      REQUIRE(target.event() == Event::CANCEL);
    }
  }
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
      REQUIRE(target.rect() == SDL_Rect{0, 0, 2, 2});
      REQUIRE(dispatcher.check(RequestEvent::NONE, {1, 1, 3, 3}).rect() ==
              SDL_Rect{1, 1, 1, 1});
    }
    REQUIRE(dispatcher.check(RequestEvent::NONE, {1, 1, 3, 3}).rect() ==
            SDL_Rect{1, 1, 3, 3});
  }

  SECTION("sub target can be hovered")
  {
    if (auto target = dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2})) {
      REQUIRE(target.status() & STATUS_HOVERED);
      REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() &
              STATUS_HOVERED);
    }
    REQUIRE_FALSE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() &
                  STATUS_HOVERED);
  }
}
