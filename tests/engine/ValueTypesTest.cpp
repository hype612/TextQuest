// Unit tests for the small value types: vec2f, Rect, EntityDistance.
#include "EntityDistance.h"
#include "Rect.h"
#include "Vec2f.h"
#include <algorithm>
#include <cmath>
#include <gtest/gtest.h>
#include <vector>

// =============
//  vec2f
// =============

TEST(Vec2f, AdditionAddsComponents) {
  // Arrange
  const vec2f a{1.f, 2.f}, b{3.f, -4.f};
  // Act
  const vec2f r = a + b;
  // Assert
  EXPECT_EQ(r, (vec2f{4.f, -2.f}));
}

TEST(Vec2f, SubtractionSubtractsComponents) {
  const vec2f a{1.f, 2.f}, b{3.f, -4.f};
  const vec2f r = a - b;
  EXPECT_EQ(r, (vec2f{-2.f, 6.f}));
}

TEST(Vec2f, ScalarMultiplicationScalesBothComponents) {
  const vec2f a{1.5f, -2.f};
  const vec2f r = a * 2.f;
  EXPECT_EQ(r, (vec2f{3.f, -4.f}));
}

TEST(Vec2f, DotProductOfPerpendicularVectorsIsZero) {
  const vec2f a{1.f, 0.f}, b{0.f, 5.f};
  EXPECT_FLOAT_EQ(a.dot(b), 0.f);
  EXPECT_FLOAT_EQ((vec2f{2.f, 3.f}).dot(vec2f{4.f, 5.f}), 23.f);
}

TEST(Vec2f, CrossProductSignFollowsOrientation) {
  const vec2f x{1.f, 0.f}, y{0.f, 1.f};
  EXPECT_FLOAT_EQ(x.cross(y), 1.f);
  EXPECT_FLOAT_EQ(y.cross(x), -1.f);
  EXPECT_FLOAT_EQ(x.cross(x), 0.f);
}

TEST(Vec2f, CompoundAssignmentOperatorsModifyInPlaceAndReturnSelf) {
  // Arrange
  vec2f v{1.f, 1.f};
  // Act
  vec2f &r1 = (v += vec2f{2.f, 3.f});
  // Assert
  EXPECT_EQ(&r1, &v);
  EXPECT_EQ(v, (vec2f{3.f, 4.f}));
  v -= vec2f{1.f, 1.f};
  EXPECT_EQ(v, (vec2f{2.f, 3.f}));
  v *= 2.f;
  EXPECT_EQ(v, (vec2f{4.f, 6.f}));
}

TEST(Vec2f, EqualityIsToleratedWithinEpsilon) {
  const vec2f a{1.f, 1.f};
  EXPECT_TRUE(a == (vec2f{1.f + 1e-6f, 1.f - 1e-6f}));
  EXPECT_FALSE(a == (vec2f{1.f + 1e-3f, 1.f}));
  EXPECT_TRUE(a != (vec2f{1.f, 1.f + 1e-3f}));
  EXPECT_FALSE(a != a);
}

TEST(Vec2f, NormalizeGivesUnitLength) {
  vec2f v{3.f, 4.f};
  v.normalize();
  EXPECT_NEAR(std::sqrt(v.x * v.x + v.y * v.y), 1.f, 1e-6f);
  EXPECT_EQ(v, (vec2f{0.6f, 0.8f}));
}

TEST(Vec2f, NormalizeOnZeroVectorLeavesItUnchanged) {
  vec2f v{0.f, 0.f};
  v.normalize();
  EXPECT_EQ(v, (vec2f{0.f, 0.f}));
}

TEST(Vec2f, NormalizedReturnsUnitVector) {
  vec2f v{0.f, 5.f};
  const vec2f n = v.normalized();
  EXPECT_EQ(n, (vec2f{0.f, 1.f}));
}

TEST(Vec2f, NormalizedDoesNotChangeTheOriginal) {
  // Arrange
  vec2f v{3.f, 4.f};
  // Act
  (void)v.normalized();
  // Assert
  EXPECT_EQ(v, (vec2f{3.f, 4.f}));
}

// =============
// Rect
// =============

TEST(Rect, DisjointRectsDoNotOverlap) {
  const Rect a{0, 0, 5, 5}, b{10, 10, 5, 5};
  EXPECT_FALSE(a.overlaps(b));
  EXPECT_FALSE(b.overlaps(a));
}

TEST(Rect, PartiallyIntersectingRectsOverlap) {
  const Rect a{0, 0, 5, 5}, b{3, 3, 5, 5};
  EXPECT_TRUE(a.overlaps(b));
  EXPECT_TRUE(b.overlaps(a));
}

TEST(Rect, NestedRectsOverlap) {
  const Rect outer{0, 0, 10, 10}, inner{2, 2, 3, 3};
  EXPECT_TRUE(outer.overlaps(inner));
  EXPECT_TRUE(inner.overlaps(outer));
}

// rects that only share an edge count as overlapping.
TEST(Rect, TouchingEdgesCountAsOverlapping) {
  const Rect a{0, 0, 5, 5};
  const Rect right{5, 0, 5, 5}, below{0, 5, 5, 5}, corner{5, 5, 5, 5};
  EXPECT_TRUE(a.overlaps(right));
  EXPECT_TRUE(a.overlaps(below));
  EXPECT_TRUE(a.overlaps(corner));
  // One cell of gap is no longer touching.
  EXPECT_FALSE(a.overlaps(Rect{6, 0, 5, 5}));
}

TEST(Rect, ContainsRectAcceptsInnerAndEqualRects) {
  const Rect outer{0, 0, 10, 10};
  EXPECT_TRUE(outer.contains(Rect{2, 2, 3, 3}));
  EXPECT_TRUE(outer.contains(outer));
}

TEST(Rect, ContainsRectRejectsPartlyOutsideRects) {
  const Rect outer{0, 0, 10, 10};
  EXPECT_FALSE(outer.contains(Rect{8, 8, 5, 5}));
  EXPECT_FALSE(outer.contains(Rect{-1, 0, 5, 5}));
  EXPECT_FALSE(outer.contains(Rect{20, 20, 2, 2}));
}

TEST(Rect, ContainsPointIncludesTopLeftAndExcludesRightBottomEdges) {
  const Rect r{2, 3, 4, 5}; // x in [2,6), y in [3,8)
  EXPECT_TRUE(r.contains(2.f, 3.f));
  EXPECT_TRUE(r.contains(5.99f, 7.99f));
  EXPECT_FALSE(r.contains(6.f, 4.f));
  EXPECT_FALSE(r.contains(3.f, 8.f));
  EXPECT_FALSE(r.contains(1.99f, 4.f));
  EXPECT_FALSE(r.contains(3.f, 2.99f));
}

// ===============
// entityDistance
// ===============

TEST(EntityDistance, LessThanComparesDistanceOnly) {
  const EntityDistance near{7, 1.f}, far{2, 5.f};
  EXPECT_TRUE(near < far);
  EXPECT_FALSE(far < near);
  EXPECT_FALSE(near < near);
}

TEST(EntityDistance, StdSortOrdersByAscendingDistance) {
  // Arrange
  std::vector<EntityDistance> v{{0, 3.f}, {1, 1.f}, {2, 2.f}, {3, 0.5f}};
  // Act
  std::sort(v.begin(), v.end());
  // Assert
  ASSERT_EQ(v.size(), 4u);
  EXPECT_EQ(v[0].id, 3);
  EXPECT_EQ(v[1].id, 1);
  EXPECT_EQ(v[2].id, 2);
  EXPECT_EQ(v[3].id, 0);
}
