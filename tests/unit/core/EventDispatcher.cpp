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
                  Status::HOVERED);
  }
  SECTION("Status is hover if square under the pointer pos")
  {
    REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() &
            Status::HOVERED);
  }
  SECTION("Status is not hover if square not under the pointer pos")
  {
    REQUIRE_FALSE(dispatcher.check(RequestEvent::HOVER, {1, 1, 2, 2}).status() &
                  Status::HOVERED);
  }
}

TEST_CASE("EventDispatcher hovers only one element per turn",
          "[event-dispatcher]")
{
  EventDispatcher dispatcher{};
  dispatcher.movePointer({0, 0});

  dispatcher.reset();
  REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() &
          Status::HOVERED);
  REQUIRE_FALSE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() &
                Status::HOVERED);
  dispatcher.reset();
  REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() &
          Status::HOVERED);
}

TEST_CASE("EventDispatcher hovered target can be discarded",
          "[event-dispatcher]")
{
  EventDispatcher dispatcher{};

  dispatcher.reset();
  dispatcher.movePointer({0, 0});
  if (auto target = dispatcher.check(RequestEvent::HOVER, {0, 0, 1, 1})) {
    REQUIRE(target.status() == Status::HOVERED);
    target.discard(Status::HOVERED);
  }
  REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() &
          Status::HOVERED);
}

TEST_CASE("EventDispatcher grab handling", "[event-dispatcher]")
{
  EventDispatcher dispatcher{};
  dispatcher.reset();
  dispatcher.movePointer({0, 0});

  SECTION("The status is not changed before press")
  {
    REQUIRE(
      dispatcher.check(RequestEvent::GRAB, {1, 1, 1, 1}, "id0"sv).status() ==
      Status::NONE);
    REQUIRE(
      dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "id0"sv).status() ==
      Status::HOVERED);
  }

  dispatcher.reset();
  dispatcher.pressPointer(0);

  SECTION("grab an element")
  {
    REQUIRE_FALSE(
      dispatcher.check(RequestEvent::GRAB, {1, 1, 1, 1}, "id0"sv).status());
    {
      auto target = dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.status() == (Status::HOVERED | Status::GRABBED));
      REQUIRE(target.event() == Event::GRAB);
    }

    dispatcher.reset();
    {
      auto target = dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.status() == (Status::HOVERED | Status::GRABBED));
      REQUIRE(target.event() == Event::NONE);
    }

    dispatcher.reset();
    SECTION("release while still hovering")
    {
      dispatcher.releasePointer(0);
      auto target = dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.status() == Status::HOVERED);
      REQUIRE(target.event() == Event::ACTION);
    }
    SECTION("release after moving out")
    {
      dispatcher.movePointer({2, 2});
      auto target = dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.status() == Status::GRABBED);
      REQUIRE(target.event() == Event::NONE);

      dispatcher.reset();
      dispatcher.releasePointer(0);
      target = dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.status() == Status::NONE);
      REQUIRE(target.event() == Event::CANCEL);
    }
    SECTION("pressing another button")
    {
      dispatcher.reset();
      dispatcher.pressPointer(1);
      auto target = dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "id1"sv);
      REQUIRE_FALSE(target.status() & Status::GRABBED);
      REQUIRE(target.event() == Event::CANCEL);
    }
  }
}

TEST_CASE("EventDispatcher handle Focus gain", "[event-dispatcher]")
{
  EventDispatcher dispatcher{};
  dispatcher.reset();
  dispatcher.movePointer({0, 0});
  dispatcher.pressPointer(0);

  // On first it hovers
  {
    auto target = dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
    REQUIRE(target.status() == (Status::HOVERED | Status::GRABBED));
    REQUIRE(target.event() == Event::GRAB);
  }

  // On second, it focus
  dispatcher.reset();
  {
    auto target = dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
    REQUIRE(target.event() == Event::FOCUS_GAINED);
    REQUIRE(target.status() ==
            (Status::HOVERED | Status::GRABBED | Status::FOCUSED));
  }

  // Next ones no event
  dispatcher.reset();
  {
    auto target = dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
    REQUIRE(target.status() ==
            (Status::HOVERED | Status::GRABBED | Status::FOCUSED));
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
      REQUIRE(target.status() == (Status::HOVERED | Status::FOCUSED));
    }

    dispatcher.reset();
    {
      auto target =
        dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.event() == Event::NONE);
      REQUIRE(target.status() == (Status::HOVERED | Status::FOCUSED));
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
      REQUIRE(target.status() == (Status::HOVERED | Status::FOCUSED));
    }

    dispatcher.reset();
    {
      auto target =
        dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
      REQUIRE(target.event() == Event::NONE);
      REQUIRE(target.status() == (Status::HOVERED | Status::FOCUSED));
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
      REQUIRE(target.status() == (Status::GRABBED | Status::FOCUSED));
      REQUIRE(target.event() == Event::NONE);
    }

    SECTION("Focus when do CANCEL by release left button on not hovered")
    {
      dispatcher.releasePointer(0);
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.event() == Event::CANCEL);
        REQUIRE(target.status() == Status::FOCUSED);
      }

      dispatcher.reset();
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.event() == Event::NONE);
        REQUIRE(target.status() == Status::FOCUSED);
      }
    }
  }
}
TEST_CASE("EventDispatcher handle focus lost", "[event-dispatcher]")
{
  EventDispatcher dispatcher{};
  dispatcher.reset();
  dispatcher.movePointer({0, 0});
  dispatcher.pressPointer(0);
  dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv); // grab

  dispatcher.reset();
  dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv); // Focus gain

  SECTION("Losing focus by press non left button not hovered")
  {
    dispatcher.reset();
    dispatcher.movePointer({3, 3});
    dispatcher.pressPointer(1);

    SECTION("Focus lost without replacement")
    {
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.event() == Event::CANCEL);
        REQUIRE(target.status() == Status::FOCUSED);
      }

      dispatcher.reset();
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.status() == Status::NONE);
        REQUIRE(target.event() == Event::FOCUS_LOST);
      }
      dispatcher.reset();
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.status() == Status::NONE);
        REQUIRE(target.event() == Event::NONE);
      }
    }
    SECTION("Focus lost with replacement after it")
    {
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.event() == Event::CANCEL);
        REQUIRE(target.status() == Status::FOCUSED);
      }
      {
        auto target2 =
          dispatcher.check(RequestEvent::FOCUS, {3, 3, 1, 1}, "id2"sv);
        REQUIRE(target2.status() == Status::HOVERED);
        REQUIRE(target2.event() == Event::NONE);
      }

      dispatcher.reset();
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.status() == Status::NONE);
        REQUIRE(target.event() == Event::FOCUS_LOST);
      }
      {
        auto target2 =
          dispatcher.check(RequestEvent::FOCUS, {3, 3, 1, 1}, "id2"sv);
        REQUIRE(target2.status() == (Status::HOVERED | Status::FOCUSED));
        REQUIRE(target2.event() == Event::FOCUS_GAINED);
      }
    }
    SECTION("Focus lost with replacement before it")
    {
      {
        auto target2 =
          dispatcher.check(RequestEvent::FOCUS, {3, 3, 1, 1}, "id2"sv);
        REQUIRE(target2.status() == Status::HOVERED);
        REQUIRE(target2.event() == Event::NONE);
      }
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.event() == Event::CANCEL);
        REQUIRE(target.status() == Status::FOCUSED);
      }

      dispatcher.reset();
      {
        auto target2 =
          dispatcher.check(RequestEvent::FOCUS, {3, 3, 1, 1}, "id2"sv);
        REQUIRE(target2.status() == (Status::HOVERED | Status::FOCUSED));
        REQUIRE(target2.event() == Event::FOCUS_GAINED);
      }
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.status() == Status::NONE);
        REQUIRE(target.event() == Event::FOCUS_LOST);
      }
    }
  }
  SECTION("Focused element not grabbed")
  {
    dispatcher.reset();
    dispatcher.movePointer({3, 3});
    dispatcher.releasePointer(0);
    REQUIRE(
      dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv).status() ==
      Status::FOCUSED);

    SECTION("Focus lost without replacement")
    {
      dispatcher.reset();
      dispatcher.pressPointer(0);
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.status() == Status::NONE);
        REQUIRE(target.event() == Event::FOCUS_LOST);
      }

      dispatcher.reset();
      {
        auto target =
          dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
        REQUIRE(target.status() == Status::NONE);
        REQUIRE(target.event() == Event::NONE);
      }
    }
    SECTION("Focus lost due to other element being grabbed")
    {
      dispatcher.reset();
      dispatcher.pressPointer(0);
      SECTION("Focus lost with replacement after it")
      {
        {
          auto target =
            dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
          REQUIRE(target.status() == Status::NONE);
          REQUIRE(target.event() == Event::FOCUS_LOST);
        }
        {
          auto target2 =
            dispatcher.check(RequestEvent::FOCUS, {3, 3, 1, 1}, "id2"sv);
          REQUIRE(target2.status() == (Status::GRABBED | Status::HOVERED));
          REQUIRE(target2.event() == Event::GRAB);
        }

        dispatcher.reset();
        {
          auto target =
            dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
          REQUIRE(target.status() == Status::NONE);
          REQUIRE(target.event() == Event::NONE);
        }
        {
          auto target2 =
            dispatcher.check(RequestEvent::FOCUS, {3, 3, 1, 1}, "id2"sv);
          REQUIRE(target2.status() ==
                  (Status::GRABBED | Status::HOVERED | Status::FOCUSED));
          REQUIRE(target2.event() == Event::FOCUS_GAINED);
        }
      }
      SECTION("Focus lost with replacement before it")
      {
        {
          auto target2 =
            dispatcher.check(RequestEvent::FOCUS, {3, 3, 1, 1}, "id2"sv);
          REQUIRE(target2.status() == (Status::GRABBED | Status::HOVERED));
          REQUIRE(target2.event() == Event::GRAB);
        }
        {
          auto target =
            dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
          REQUIRE(target.status() == Status::NONE);
          REQUIRE(target.event() == Event::FOCUS_LOST);
        }

        dispatcher.reset();
        {
          auto target2 =
            dispatcher.check(RequestEvent::FOCUS, {3, 3, 1, 1}, "id2"sv);
          REQUIRE(target2.status() ==
                  (Status::GRABBED | Status::HOVERED | Status::FOCUSED));
          REQUIRE(target2.event() == Event::FOCUS_GAINED);
        }
        {
          auto target =
            dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
          REQUIRE(target.status() == Status::NONE);
          REQUIRE(target.event() == Event::NONE);
        }
      }
    }

    SECTION("Focus lost due to other element being focused, but not grabbed")
    {
      dispatcher.reset();
      dispatcher.pressPointer(1);
      SECTION("Focus lost with replacement after it")
      {
        {
          auto target =
            dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
          REQUIRE(target.status() == Status::NONE);
          REQUIRE(target.event() == Event::FOCUS_LOST);
        }
        {
          auto target2 =
            dispatcher.check(RequestEvent::FOCUS, {3, 3, 1, 1}, "id2"sv);
          REQUIRE(target2.event() == Event::FOCUS_GAINED);
          REQUIRE(target2.status() == (Status::HOVERED | Status::FOCUSED));
        }

        dispatcher.reset();
        {
          auto target =
            dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
          REQUIRE(target.status() == Status::NONE);
          REQUIRE(target.event() == Event::NONE);
        }
        {
          auto target2 =
            dispatcher.check(RequestEvent::FOCUS, {3, 3, 1, 1}, "id2"sv);
          REQUIRE(target2.event() == Event::NONE);
          REQUIRE(target2.status() == (Status::HOVERED | Status::FOCUSED));
        }
      }
      SECTION("Focus lost with replacement before it")
      {
        {
          auto target2 =
            dispatcher.check(RequestEvent::FOCUS, {3, 3, 1, 1}, "id2"sv);
          REQUIRE(target2.status() == (Status::FOCUSED | Status::HOVERED));
          REQUIRE(target2.event() == Event::FOCUS_GAINED);
        }
        {
          auto target =
            dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
          REQUIRE(target.status() == Status::NONE);
          REQUIRE(target.event() == Event::FOCUS_LOST);
        }

        dispatcher.reset();
        {
          auto target2 =
            dispatcher.check(RequestEvent::FOCUS, {3, 3, 1, 1}, "id2"sv);
          REQUIRE(target2.status() == (Status::HOVERED | Status::FOCUSED));
          REQUIRE(target2.event() == Event::NONE);
        }
        {
          auto target =
            dispatcher.check(RequestEvent::FOCUS, {0, 0, 1, 1}, "id1"sv);
          REQUIRE(target.status() == Status::NONE);
          REQUIRE(target.event() == Event::NONE);
        }
      }
    }
  }
}

TEST_CASE("EventDispatcher EventTarget stacks", "[event-dispatcher]")
{
  EventDispatcher dispatcher{};
  dispatcher.reset();

  SECTION("sub target can be hovered")
  {
    dispatcher.movePointer({0, 0});
    {
      auto target = dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2});
      REQUIRE(target.status() == Status::HOVERED);
      REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() ==
              Status::HOVERED);
      REQUIRE(target.status() == Status::HOVERED);
    }
    REQUIRE(dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2}).status() ==
            Status::NONE);
  }
  SECTION("sub target can be hovered only if its parent is hovered")
  {
    dispatcher.movePointer({2, 2});
    {
      auto target = dispatcher.check(RequestEvent::HOVER, {0, 0, 2, 2});
      REQUIRE(target.status() == Status::NONE);
      REQUIRE(dispatcher.check(RequestEvent::HOVER, {1, 1, 2, 2}).status() ==
              Status::NONE);
      REQUIRE(target.status() == Status::NONE);
    }
    REQUIRE(dispatcher.check(RequestEvent::HOVER, {1, 1, 2, 2}).status() ==
            Status::HOVERED);
  }
}
