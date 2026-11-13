// Wiring check for the engine test binary: it compiles, links against the
// engine library only, and runs. Also exercises the shared support code.
#include "FakeRenderer.h"
#include "TestHelpers.h"
#include "Vec2f.h"
#include <gtest/gtest.h>

TEST(EngineSanity, Vec2fAdditionAddsComponents) {
  // Arrange
  const vec2f a{1.f, 2.f};
  const vec2f b{3.f, 4.f};
  // Act
  const vec2f sum = a + b;
  // Assert
  EXPECT_EQ(sum, (vec2f{4.f, 6.f}));
}

TEST(EngineSanity, FakeRendererHandsOutIncreasingIdsAndRejectsOnRequest) {
  // Arrange
  FakeRenderer renderer;
  // Act
  OverlayId first = renderer.createOverlay(Rect{1, 1, 5, 5});
  renderer.rejectNext = true;
  OverlayId rejected = renderer.createOverlay(Rect{10, 10, 5, 5});
  OverlayId second = renderer.createOverlay(Rect{20, 10, 5, 5});
  // Assert
  EXPECT_EQ(first, 0);
  EXPECT_EQ(rejected, -1);
  EXPECT_EQ(second, 1);
  EXPECT_EQ(renderer.creates.size(), 3u);
}

TEST(EngineSanity, MakeEntityBuildsEntityWithRequestedState) {
  // Arrange
  ControllerScript script;
  EntityOpts opts;
  opts.hp = 42;
  // Act
  Entity e = makeEntity({2.5f, 3.5f}, 1.f, script, opts);
  // Assert
  EXPECT_EQ(e.health(), 42);
  EXPECT_EQ(e.transform().position, (vec2f{2.5f, 3.5f}));
  EXPECT_EQ(e.ID(), -1);
}
