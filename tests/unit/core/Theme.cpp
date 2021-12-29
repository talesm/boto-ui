#include "catch.hpp"
#include "core/Theme.hpp"

using namespace boto;
struct DummyTheme;

template<class T>
struct StyleTypeT<DummyTheme, T>
{
  using type = int;
};

struct DummyWithDefault;
namespace boto {
template<>
struct StyleFor<DummyTheme, DummyWithDefault>
{
  static constexpr int get(ThemeT<DummyTheme>&) { return 10; }
};
}

SCENARIO("Getting an style", "[theme]")
{
  GIVEN("An empty theme")
  {
    ThemeT<DummyTheme> theme;
    WHEN("Set a value for a given type")
    {
      struct DummyType;
      theme.set<DummyType>(0);
      THEN("Subsequent get return same value")
      {
        auto& v = theme.of<DummyType>();
        REQUIRE(v == 0);
        REQUIRE((theme.of<Hovered<DummyType>>()) == 0);
      }
      AND_WHEN("Set value of a status specific variant")
      {
        theme.set<Hovered<DummyType>>(2);
        THEN("Subsequent get return same value")
        {
          REQUIRE(theme.of<DummyType>() == 0);
          REQUIRE(theme.of<Hovered<DummyType>>() == 2);
        }
      }
      AND_WHEN("Set a value for another type")
      {
        struct DummyType2;
        theme.set<DummyType2>(1);
        THEN("Subsequent gets return proper values")
        {
          REQUIRE((theme.of<DummyType>()) == 0);
          REQUIRE((theme.of<DummyType2>()) == 1);
        }
      }
    }
    WHEN("Have a type that has a default value")
    {
      // DummyWithDefault
      THEN("It is added on the first time you ask for it")
      {
        REQUIRE((theme.of<DummyWithDefault>()) == 10);
        REQUIRE((theme.of<Hovered<DummyWithDefault>>()) == 10);
      }
    }
  }
}
