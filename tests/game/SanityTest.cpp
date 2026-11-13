// Wiring check for the game test binary: it links against the game and engine
// libraries (constructing the controller pulls in code from libtqgame.a).
#include "GameSpecific/Headers/PlayerBehaviorController.h"
#include "RecordingObservers.h"
#include "ScriptedInput.h"
#include "TestHelpers.h"
#include <gtest/gtest.h>

TEST(GameSanity, PlayerControllerCanBeConstructedAndObserved) {
  // Arrange
  ScriptedInput input;
  RecordingHealthObserver health;
  PlayerBehaviorController controller(input);
  // Act
  controller.setHealthObserver(&health);
  // Assert
  EXPECT_TRUE(health.values.empty());
}

TEST(GameSanity, ScriptedInputReportsKeyState) {
  // Arrange
  ScriptedInput input;
  // Act
  input.setKeyDown(EntityAction::FORWARD, true);
  input.pressRaw('x');
  // Assert
  EXPECT_TRUE(input.keyDown(EntityAction::FORWARD));
  EXPECT_FALSE(input.keyDown(EntityAction::BACKWARD));
  EXPECT_TRUE(input.rawKeyPressed('x'));
}

TEST(GameSanity, MakeMapConcatenatesRows) {
  // Arrange / Act
  MapData m = makeMap({"###", "#.#", "###"});
  // Assert
  EXPECT_EQ(m.map, "####.####");
  EXPECT_EQ(m.w, 3);
  EXPECT_EQ(m.h, 3);
}
