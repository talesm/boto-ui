#include "catch.hpp"
#include "core/DisplayList.hpp"

TEST_CASE("DisplayList visits from last to first", "[display-list]")
{
  boto::DisplayList dList;

  dList.push({0, 0, 10, 10}, {0}, SDL_BLENDMODE_BLEND);
  if (auto cl = dList.clip({0, 0, 50, 50})) {
    dList.push({10, 10, 20, 20},
               {0},
               SDL_BLENDMODE_BLEND,
               reinterpret_cast<SDL_Texture*>(16),
               {0, 0, 10, 10});
  }
  int i = 0;
  dList.visit([&](const auto& item) {
    switch (i++) {
    case 0:
      REQUIRE(item.action == boto::DisplayListAction::SET_CLIP);
      REQUIRE(item.rect.w == 50);
      break;
    case 1:
      REQUIRE(item.action == boto::DisplayListAction::TEXTURE_BOX);
      REQUIRE(item.rect.x == 10);
      break;
    case 2:
      REQUIRE(item.action == boto::DisplayListAction::RESET_CLIP);
      break;
    case 3:
      REQUIRE(item.action == boto::DisplayListAction::COLOR_BOX);
      break;
    }
  });
}

TEST_CASE("DisplayList discards rects out", "[display-list]")
{
  boto::DisplayList dList;

  if (auto cl = dList.clip({0, 0, 50, 50})) {
    dList.push({0, 0, 10, 10}, {0}, SDL_BLENDMODE_BLEND);
    dList.push({60, 10, 20, 20}, {0}, SDL_BLENDMODE_BLEND);
  }
  int i = 0;
  dList.visit([&](const auto& item) {
    switch (i++) {
    case 0:
      REQUIRE(item.action == boto::DisplayListAction::SET_CLIP);
      REQUIRE(item.rect.w == 50);
      break;
    case 1:
      REQUIRE(item.action == boto::DisplayListAction::COLOR_BOX);
      break;
    case 2:
      REQUIRE(item.action == boto::DisplayListAction::RESET_CLIP);
      break;
    }
  });
}

TEST_CASE("DisplayList intersects recursive clips", "[display-list]")
{
  boto::DisplayList dList;

  if (auto cl = dList.clip({0, 0, 50, 50})) {
    dList.push({0, 0, 10, 10}, {0}, SDL_BLENDMODE_BLEND);
    if (auto cl = dList.clip({20, 20, 40, 40})) {
      dList.push({10, 10, 20, 20}, {0}, SDL_BLENDMODE_BLEND);
    }
    dList.push({30, 30, 20, 20}, {0}, SDL_BLENDMODE_BLEND);
  }
  int i = 0;
  dList.visit([&](const auto& item) {
    switch (i++) {
    case 0:
    case 4:
      REQUIRE(item.action == boto::DisplayListAction::SET_CLIP);
      REQUIRE(item.rect.x == 0);
      REQUIRE(item.rect.y == 0);
      REQUIRE(item.rect.w == 50);
      REQUIRE(item.rect.h == 50);
      break;
    case 1:
    case 3:
    case 5:
      REQUIRE(item.action == boto::DisplayListAction::COLOR_BOX);
      break;
    case 2:
      REQUIRE(item.action == boto::DisplayListAction::SET_CLIP);
      REQUIRE(item.rect.x == 20);
      REQUIRE(item.rect.y == 20);
      REQUIRE(item.rect.w == 30);
      REQUIRE(item.rect.h == 30);
      break;
    case 6:
      REQUIRE(item.action == boto::DisplayListAction::RESET_CLIP);
      break;
    }
  });
}
