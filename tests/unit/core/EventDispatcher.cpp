#include "catch.hpp"
#include "core/EventDispatcher.hpp"

using namespace boto;
using namespace std;

TEST_CASE("EventDispatcher hover handling", "[event-dispatcher]")
{
  EventDispatcher dispatcher{};
  dispatcher.reset();
  dispatcher.movePointer({0, 0});

  SECTION("Ignore if not requested")
  {
    REQUIRE_FALSE(dispatcher.check(RequestEvent::NONE, {0, 0, 2, 2}).status() &
                  STATUS_HOVERED);
  }
  SECTION("Status is hover if square under the pointer pos")
  {
    REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() &
            STATUS_HOVERED);
  }
  SECTION("Status is not hover if square not under the pointer pos")
  {
    REQUIRE_FALSE(dispatcher.check(RequestEvent::HOVER, {1, 1, 2, 2}).status() &
                  STATUS_HOVERED);
  }
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

TEST_CASE("EventDispatcher hovered target can be discarded",
          "[event-dispatcher]")
{
  EventDispatcher dispatcher{};

  dispatcher.reset();
  dispatcher.movePointer({0, 0});
  if (auto target = dispatcher.check(RequestEvent::HOVER, {0, 0, 1, 1})) {
    REQUIRE(target.status() == STATUS_HOVERED);
    target.discard(STATUS_HOVERED);
  }
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
      REQUIRE(target.status() == STATUS_GRABBED);
      REQUIRE(target.event() == Event::NONE);

      dispatcher.reset();
      dispatcher.releasePointer(0);
      target = dispatcher.check(RequestEvent::ACTION, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.status() == STATUS_NONE);
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

TEST_CASE("EventDispatcher handle Action", "[event-dispatcher]")
{
  EventDispatcher dispatcher{};
  dispatcher.reset();
  dispatcher.movePointer({0, 0});
  dispatcher.pressPointer(0);

  // On first it hovers
  {
    auto target = dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
    REQUIRE(target.status() == (STATUS_HOVERED | STATUS_GRABBED));
    REQUIRE(target.event() == Event::GRAB);
  }

  // On second, it focus
  dispatcher.reset();
  {
    auto target = dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
    REQUIRE(target.event() == Event::FOCUS_GAINED);
    REQUIRE(target.status() ==
            (STATUS_HOVERED | STATUS_GRABBED | STATUS_FOCUSED));
  }

  // Next ones no event
  dispatcher.reset();
  {
    auto target = dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
    REQUIRE(target.status() ==
            (STATUS_HOVERED | STATUS_GRABBED | STATUS_FOCUSED));
    REQUIRE(target.event() == Event::NONE);
  }

  SECTION("Focus when do ACTION")
  {
    dispatcher.reset();
    dispatcher.releasePointer(0);
    {
      auto target =
        dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.event() == Event::ACTION);
      REQUIRE(target.status() == (STATUS_HOVERED | STATUS_FOCUSED));
    }

    dispatcher.reset();
    {
      auto target =
        dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.event() == Event::NONE);
      REQUIRE(target.status() == (STATUS_HOVERED | STATUS_FOCUSED));
    }
  }
  SECTION("Focus when do CANCEL by non left button")
  {
    dispatcher.reset();
    dispatcher.pressPointer(1);
    {
      auto target =
        dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.event() == Event::CANCEL);
      REQUIRE(target.status() == (STATUS_HOVERED | STATUS_FOCUSED));
    }

    dispatcher.reset();
    {
      auto target =
        dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.event() == Event::NONE);
      REQUIRE(target.status() == (STATUS_HOVERED | STATUS_FOCUSED));
    }
  }
  SECTION("Pointer moved outside rect")
  {
    dispatcher.reset();
    dispatcher.movePointer({3, 3});

    SECTION("Focus when not hovered anymore")
    {
      auto target =
        dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.status() == (STATUS_GRABBED | STATUS_FOCUSED));
      REQUIRE(target.event() == Event::NONE);
    }

    SECTION("Focus when do CANCEL by release left button on not hovered")
    {
      dispatcher.releasePointer(0);
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.event() == Event::CANCEL);
        REQUIRE(target.status() == STATUS_FOCUSED);
      }

      dispatcher.reset();
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.event() == Event::NONE);
        REQUIRE(target.status() == STATUS_FOCUSED);
      }
    }
    SECTION("Focus when do CANCEL by press non left button not hovered")
    {
      dispatcher.pressPointer(1);
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.event() == Event::CANCEL);
        REQUIRE(target.status() == STATUS_FOCUSED);
      }

      // dispatcher.reset();
      // {
      //   auto target =
      //     dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
      //   // REQUIRE(target.event() == Event::NONE);
      //   REQUIRE(target.status() == STATUS_NONE);
      // }
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
