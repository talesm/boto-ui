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
      THEN("It correctly use defaults")
      {
        REQUIRE((theme.of<DummyWithDefault>()) == 10);
        AND_THEN("Hovered successfully defaults to normal")
        {
          REQUIRE((theme.of<Hovered<DummyWithDefault>>()) == 10);
        }
      }
    }
  }
}
struct DummyWithHovered;
struct DummyWithGrabbed;
struct DummyWithFocused;
struct DummyWithAll;
namespace boto {
template<>
struct StyleFor<DummyTheme, DummyWithHovered>
{
  static constexpr int get(ThemeT<DummyTheme>&) { return 20; }
};
template<>
struct StyleFor<DummyTheme, Hovered<DummyWithHovered>>
{
  static constexpr int get(ThemeT<DummyTheme>&) { return 21; }
};

template<>
struct StyleFor<DummyTheme, DummyWithGrabbed>
{
  static constexpr int get(ThemeT<DummyTheme>&) { return 30; }
};
template<>
struct StyleFor<DummyTheme, Hovered<DummyWithGrabbed>>
{
  static constexpr int get(ThemeT<DummyTheme>&) { return 31; }
};

template<>
struct StyleFor<DummyTheme, Grabbed<DummyWithGrabbed>>
{
  static constexpr int get(ThemeT<DummyTheme>&) { return 32; }
};

template<>
struct StyleFor<DummyTheme, DummyWithFocused>
{
  static constexpr int get(ThemeT<DummyTheme>&) { return 40; }
};
template<>
struct StyleFor<DummyTheme, Hovered<DummyWithFocused>>
{
  static constexpr int get(ThemeT<DummyTheme>&) { return 41; }
};
template<>
struct StyleFor<DummyTheme, Grabbed<DummyWithFocused>>
{
  static constexpr int get(ThemeT<DummyTheme>&) { return 42; }
};
template<>
struct StyleFor<DummyTheme, Hovered<Grabbed<DummyWithFocused>>>
{
  static constexpr int get(ThemeT<DummyTheme>&) { return 43; }
};
template<>
struct StyleFor<DummyTheme, Focused<DummyWithFocused>>
{
  static constexpr int get(ThemeT<DummyTheme>&) { return 44; }
};

template<class T>
struct AllStyle
{
  static constexpr int get(ThemeT<DummyTheme>&) { return 0; }
};

template<>
struct StyleFor<DummyTheme, DummyWithAll> : AllStyle<DummyWithAll>
{};

template<class T>
struct AllStyle<Hovered<T>>
{
  static constexpr int get(ThemeT<DummyTheme>& t)
  {
    return (t.of<T>() * 10) | 1;
  }
};
template<>
struct StyleFor<DummyTheme, Hovered<DummyWithAll>>
  : AllStyle<Hovered<DummyWithAll>>
{};
template<class T>
struct AllStyle<Grabbed<T>>
{
  static constexpr int get(ThemeT<DummyTheme>& t)
  {
    return (t.of<T>() * 10) | 2;
  }
};
template<>
struct StyleFor<DummyTheme, Grabbed<DummyWithAll>>
  : AllStyle<Grabbed<DummyWithAll>>
{};
template<>
struct StyleFor<DummyTheme, Hovered<Grabbed<DummyWithAll>>>
  : AllStyle<Hovered<Grabbed<DummyWithAll>>>
{};
template<class T>
struct AllStyle<Focused<T>>
{
  static constexpr int get(ThemeT<DummyTheme>& t)
  {
    return (t.of<T>() * 10) | 4;
  }
};
template<>
struct StyleFor<DummyTheme, Focused<DummyWithAll>>
  : AllStyle<Focused<DummyWithAll>>
{};
template<>
struct StyleFor<DummyTheme, Hovered<Focused<DummyWithAll>>>
  : AllStyle<Hovered<Focused<DummyWithAll>>>
{};
template<>
struct StyleFor<DummyTheme, Grabbed<Focused<DummyWithAll>>>
  : AllStyle<Grabbed<Focused<DummyWithAll>>>
{};
template<>
struct StyleFor<DummyTheme, Hovered<Grabbed<Focused<DummyWithAll>>>>
  : AllStyle<Hovered<Grabbed<Focused<DummyWithAll>>>>
{};

}

SCENARIO("Getting an style with status", "[theme]")
{
  GIVEN("An empty theme")
  {
    ThemeT<DummyTheme> theme;
    WHEN("Get normal value")
    {
      THEN("It results in its default value")
      {
        REQUIRE((theme.of<DummyWithDefault>()) == 10);
        REQUIRE((theme.of<DummyWithHovered>()) == 20);
        REQUIRE((theme.of<DummyWithGrabbed>()) == 30);
        REQUIRE((theme.of<DummyWithFocused>()) == 40);
        REQUIRE((theme.of<DummyWithAll>()) == 0);
      }
    }
    WHEN("Get hovered value")
    {
      THEN("It results in its default hovered value if defined")
      {
        REQUIRE((theme.of<Hovered<DummyWithHovered>>()) == 21);
        REQUIRE((theme.of<Hovered<DummyWithGrabbed>>()) == 31);
        REQUIRE((theme.of<Hovered<DummyWithFocused>>()) == 41);
        REQUIRE((theme.of<Hovered<DummyWithAll>>()) == 1);
      }
      THEN("It results in its default value if has not a specific hovered")
      {
        REQUIRE((theme.of<Hovered<DummyWithDefault>>()) == 10);
      }
    }
    WHEN("Get grabbed value")
    {
      THEN("It results in its default grabbed value if defined")
      {
        REQUIRE((theme.of<Grabbed<DummyWithGrabbed>>()) == 32);
        REQUIRE((theme.of<Grabbed<DummyWithFocused>>()) == 42);
        REQUIRE((theme.of<Grabbed<DummyWithAll>>()) == 2);
      }
      THEN("It results in its default value if has not a specific grabbed")
      {
        REQUIRE((theme.of<Grabbed<DummyWithDefault>>()) == 10);
        REQUIRE((theme.of<Grabbed<DummyWithHovered>>()) == 20);
      }
    }
    WHEN("Get hovered & grabbed value")
    {
      THEN("It results in its default h&g value if defined")
      {
        REQUIRE((theme.of<Hovered<Grabbed<DummyWithFocused>>>()) == 43);
        REQUIRE((theme.of<Hovered<Grabbed<DummyWithAll>>>()) == 21);
      }
      THEN("It results in grabbed if defined and specific h&g is not")
      {
        REQUIRE((theme.of<Hovered<Grabbed<DummyWithGrabbed>>>()) == 32);
      }
      THEN("It results in its default value if has not a specific h&g")
      {
        REQUIRE((theme.of<Hovered<Grabbed<DummyWithDefault>>>()) == 10);
        REQUIRE((theme.of<Hovered<Grabbed<DummyWithHovered>>>()) == 20);
      }
    }
    WHEN("Get focused value")
    {
      THEN("It results in its default focused value if defined")
      {
        REQUIRE((theme.of<Focused<DummyWithFocused>>()) == 44);
        REQUIRE((theme.of<Focused<DummyWithAll>>()) == 4);
      }
      THEN("It results in its default value if has not a specific focused")
      {
        REQUIRE((theme.of<Focused<DummyWithDefault>>()) == 10);
        REQUIRE((theme.of<Focused<DummyWithHovered>>()) == 20);
        REQUIRE((theme.of<Focused<DummyWithGrabbed>>()) == 30);
      }
    }
    WHEN("Get hovered & focused value")
    {
      THEN("It results in its default h&f value if defined")
      {
        REQUIRE((theme.of<Hovered<Focused<DummyWithAll>>>()) == 41);
      }
      THEN("It results in focused if defined and specific h&f is not")
      {
        REQUIRE((theme.of<Hovered<Focused<DummyWithFocused>>>()) == 44);
      }
      THEN("It results in its default value if has not a specific h&f")
      {
        REQUIRE((theme.of<Hovered<Focused<DummyWithDefault>>>()) == 10);
        REQUIRE((theme.of<Hovered<Focused<DummyWithHovered>>>()) == 20);
        REQUIRE((theme.of<Hovered<Focused<DummyWithGrabbed>>>()) == 30);
      }
    }
    WHEN("Get grabbed & focused value")
    {
      THEN("It results in its default g&f value if defined")
      {
        REQUIRE((theme.of<Grabbed<Focused<DummyWithAll>>>()) == 42);
      }
      THEN("It results in focused if defined and specific g&f is not")
      {
        REQUIRE((theme.of<Grabbed<Focused<DummyWithFocused>>>()) == 44);
      }
      THEN("It results in its default value if has not a specific g&f")
      {
        REQUIRE((theme.of<Grabbed<Focused<DummyWithDefault>>>()) == 10);
        REQUIRE((theme.of<Grabbed<Focused<DummyWithHovered>>>()) == 20);
        REQUIRE((theme.of<Grabbed<Focused<DummyWithGrabbed>>>()) == 30);
      }
    }
    WHEN("Get hovered & grabbed & focused value")
    {
      THEN("It results in its default g&f value if defined")
      {
        REQUIRE((theme.of<Hovered<Grabbed<Focused<DummyWithAll>>>>()) == 421);
      }
      THEN("It results in focused if defined and specific g&f is not")
      {
        REQUIRE((theme.of<Hovered<Grabbed<Focused<DummyWithFocused>>>>()) ==
                44);
      }
      THEN("It results in its default value if has not a specific g&f")
      {
        REQUIRE((theme.of<Hovered<Grabbed<Focused<DummyWithDefault>>>>()) ==
                10);
        REQUIRE((theme.of<Hovered<Grabbed<Focused<DummyWithHovered>>>>()) ==
                20);
        REQUIRE((theme.of<Hovered<Grabbed<Focused<DummyWithGrabbed>>>>()) ==
                30);
      }
    }
  }
}
