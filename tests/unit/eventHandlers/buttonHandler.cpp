#include "catch.hpp"
#include "core/EventDispatcher.hpp"
#include "core/StatusStreamAdaptor.hpp"
#include "eventHandlers/buttonHandler.hpp"

using namespace std;
using namespace boto;

SCENARIO("Pressing a push button")
{
  GIVEN("An event dispatcher with pointer press on 0,0")
  {
    EventDispatcher dispatcher;
    dispatcher.reset();
    dispatcher.movePointer({0, 0});
    dispatcher.pressPointer(0);

    WHEN("check the button requesting GRAB")
    {
      {
        auto el = dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "btn1"sv);
        REQUIRE(el.status() == (Status::GRABBED | Status::HOVERED));
        REQUIRE(el.event() == Event::GRAB);

        THEN("element is not doing action")
        {
          auto doAction = handlePushButton(el.state());
          REQUIRE(doAction == false);
        }
      }
      AND_WHEN("The button is released")
      {
        dispatcher.reset();
        dispatcher.releasePointer(0);

        auto el = dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "btn1"sv);
        REQUIRE(el.status() == Status::HOVERED);
        REQUIRE(el.event() == Event::ACTION);

        THEN("element is doing action")
        {
          auto doAction = handlePushButton(el.state());
          REQUIRE(doAction == true);
        }
      }
      AND_WHEN("The button is released out of range")
      {
        dispatcher.reset();
        dispatcher.movePointer({1, 1});
        dispatcher.releasePointer(0);

        auto el = dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "btn1"sv);
        REQUIRE(el.status() == Status::NONE);
        REQUIRE(el.event() == Event::CANCEL);

        THEN("element is doing action")
        {
          auto doAction = handlePushButton(el.state());
          REQUIRE(doAction == false);
        }
      }
    }
  }
}
SCENARIO("Pressing a toggle button")
{
  GIVEN("An event dispatcher with pointer press on 0,0")
  {
    EventDispatcher dispatcher;
    dispatcher.reset();
    dispatcher.movePointer({0, 0});
    dispatcher.pressPointer(0);

    AND_GIVEN("The pressState state is false")
    {
      bool pressState = false;
      WHEN("check the button requesting GRAB")
      {
        {
          auto el =
            dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "btn1"sv);
          REQUIRE(el.status() == (Status::GRABBED | Status::HOVERED));
          REQUIRE(el.event() == Event::GRAB);

          THEN("element is not doing action")
          {
            auto doAction = handleCheckButton(el.state(), &pressState);
            REQUIRE(doAction == false);
            REQUIRE(pressState == false);
          }
        }
        AND_WHEN("The button is released")
        {
          dispatcher.reset();
          dispatcher.releasePointer(0);

          auto el =
            dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "btn1"sv);
          REQUIRE(el.status() == Status::HOVERED);
          REQUIRE(el.event() == Event::ACTION);

          THEN("element is doing action")
          {
            auto doAction = handleCheckButton(el.state(), &pressState);
            REQUIRE(doAction == true);
            REQUIRE(pressState == true);
          }
        }
        AND_WHEN("The button is released out of range")
        {
          dispatcher.reset();
          dispatcher.movePointer({1, 1});
          dispatcher.releasePointer(0);

          auto el =
            dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "btn1"sv);
          REQUIRE(el.status() == Status::NONE);
          REQUIRE(el.event() == Event::CANCEL);

          THEN("element is doing action")
          {
            auto doAction = handleCheckButton(el.state(), &pressState);
            REQUIRE(doAction == false);
            REQUIRE(pressState == false);
          }
        }
      }
    }
    AND_GIVEN("The pressState state is true")
    {
      bool pressState = true;
      WHEN("check the button requesting GRAB")
      {
        {
          auto el =
            dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "btn1"sv);
          REQUIRE(el.status() == (Status::GRABBED | Status::HOVERED));
          REQUIRE(el.event() == Event::GRAB);

          THEN("element is not doing action")
          {
            auto doAction = handleCheckButton(el.state(), &pressState);
            REQUIRE(doAction == false);
            REQUIRE(pressState == true);
          }
        }
        AND_WHEN("The button is released")
        {
          dispatcher.reset();
          dispatcher.releasePointer(0);

          auto el =
            dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "btn1"sv);
          REQUIRE(el.status() == Status::HOVERED);
          REQUIRE(el.event() == Event::ACTION);

          THEN("element is doing action")
          {
            auto doAction = handleCheckButton(el.state(), &pressState);
            REQUIRE(doAction == true);
            REQUIRE(pressState == false);
          }
        }
        AND_WHEN("The button is released out of range")
        {
          dispatcher.reset();
          dispatcher.movePointer({1, 1});
          dispatcher.releasePointer(0);

          auto el =
            dispatcher.check(RequestEvent::GRAB, {0, 0, 1, 1}, "btn1"sv);
          REQUIRE(el.status() == Status::NONE);
          REQUIRE(el.event() == Event::CANCEL);

          THEN("element is doing action")
          {
            auto doAction = handleCheckButton(el.state(), &pressState);
            REQUIRE(doAction == false);
            REQUIRE(pressState == true);
          }
        }
      }
    }
  }
}
