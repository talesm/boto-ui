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
