// Unit tests for TextureMapper: parsing, mask, nearest-neighbour scaling and
// column sampling. Textures are rows joined with '\n'; width = index of the
// first '\n'.
#include "TextureMapper.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace {

// 8 shading variants of a 4x4 texture; variant i is filled with the char
// '0'+i, so a sample tells which variant was used.
std::vector<std::string> shadingVariants() {
  std::vector<std::string> v;
  for (int i = 0; i < 8; ++i) {
    const std::string row = std::string(4, char('0' + i)) + "\n";
    v.push_back(row + row + row + row);
  }
  return v;
}

// 4x2 texture whose columns are '0','1','2','3' (same in both rows).
const std::string kColumns = "0123\n0123\n";

} // namespace

TEST(TextureMapper, ParsesWidthAndHeightFromTheTexture) {
  // Arrange: 2 wide, 3 high
  TextureMapper tm(std::string("ab\ncd\nef\n"));
  // Act: scaling to the same size is the identity only if the parsed
  // dimensions are right
  std::string same = tm.scaledTex(2, 3, 0);
  // Assert
  EXPECT_EQ(same, "ab\ncd\nef\n");
  EXPECT_EQ(tm.getMask().size(), 6u);
}

TEST(TextureMapper, MaskIsZeroForSpacesAndOneForOtherCharacters) {
  // Arrange
  TextureMapper tm(std::string(" x\nx \n"));
  // Act
  const std::vector<int> &mask = tm.getMask();
  // Assert
  EXPECT_EQ(mask, (std::vector<int>{0, 1, 1, 0}));
}

TEST(TextureMapper, UpscalingTwoByTwoToFourByFourDuplicatesEachTexel) {
  // Arrange
  TextureMapper tm(std::string("ab\ncd\n"));
  // Act
  std::string out = tm.nxyInterpolationScale(4, 4, 0);
  // Assert: every row ends in '\n'; 2x2 -> 4x4 gives 2x2 blocks of each texel
  EXPECT_EQ(out, "aabb\n"
                 "aabb\n"
                 "ccdd\n"
                 "ccdd\n");
}

TEST(TextureMapper, ScaledOutputHasNewlineAtTheEndOfEveryRow) {
  TextureMapper tm(std::string("ab\ncd\n"));
  std::string out = tm.nxyInterpolationScale(5, 3, 0);
  ASSERT_EQ(out.size(), 18u);
  EXPECT_EQ(out[5], '\n');
  EXPECT_EQ(out[11], '\n');
  EXPECT_EQ(out[17], '\n');
}

TEST(TextureMapper, ColumnLengthEqualsTheVisibleRange) {
  TextureMapper tm(kColumns);
  EXPECT_EQ(tm.getTexColumnAt(10, 0.5f, 0, 10).size(), 10u);
  EXPECT_EQ(tm.getTexColumnAt(10, 0.5f, 3, 7).size(), 4u);
  EXPECT_EQ(tm.getTexColumnAt(10, 0.5f, 5, 5).size(), 0u);
}

TEST(TextureMapper, HitpointZeroSamplesTheFirstColumn) {
  TextureMapper tm(kColumns);
  EXPECT_EQ(tm.getTexColumnAt(4, 0.f, 0, 4), "0000");
}

TEST(TextureMapper, HitpointJustBelowOneSamplesTheLastColumn) {
  TextureMapper tm(kColumns);
  EXPECT_EQ(tm.getTexColumnAt(4, 0.999f, 0, 4), "3333");
}

TEST(TextureMapper, ShadingIndexSelectsTheMatchingVariant) {
  // Arrange
  TextureMapper tm(shadingVariants());
  // Act / Assert
  for (int i = 0; i < 8; ++i) {
    SCOPED_TRACE(i);
    EXPECT_EQ(tm.getTexColumnAt(4, 0.5f, 0, 4, i),
              std::string(4, char('0' + i)));
    EXPECT_EQ(tm.scaledTex(2, 2, i)[0], char('0' + i));
  }
}

TEST(TextureMapper, ColumnWithoutShadingIndexUsesTheBrightestVariant) {
  TextureMapper tm(shadingVariants());
  EXPECT_EQ(tm.getTexColumnAt(4, 0.5f, 0, 4), "0000");
}

TEST(TextureMapper, OutOfRangeShadingIndexIsClampedToExistingVariants) {
  TextureMapper tm(shadingVariants());
  EXPECT_EQ(tm.getTexColumnAt(4, 0.5f, 0, 4, 99), "7777");
  EXPECT_EQ(tm.getTexColumnAt(4, 0.5f, 0, 4, -3), "0000");
}
