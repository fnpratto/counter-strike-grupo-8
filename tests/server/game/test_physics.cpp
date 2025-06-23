#include <gtest/gtest.h>

#include "common/physics/physics_config.h"
#include "common/utils/rectangle.h"
#include "common/utils/vector_2d.h"
#include "server/physics/rect_hitbox.h"

class TestRectHitbox: public ::testing::Test {
protected:
    RectHitbox not_rotated_rect_hitbox;
    RectHitbox rotated_rect_hitbox;

    TestRectHitbox():
            not_rotated_rect_hitbox(Rectangle(Vector2D(10, 10), 4, 4)),
            rotated_rect_hitbox(Rectangle(Vector2D(10, 10), 4, 2)) {}

    void SetUp() override {
        Rectangle rotated_rect(Vector2D(10, 10), 4, 2);
        rotated_rect.rotate(45.0f);
        rotated_rect_hitbox = RectHitbox(rotated_rect);
    }
};

TEST_F(TestRectHitbox, CollidesWithCircle) {
    Vector2D circle_center(11, 11);
    float radius = 1.0f;
    EXPECT_TRUE(not_rotated_rect_hitbox.collides_with_circle(circle_center, radius));
    EXPECT_TRUE(rotated_rect_hitbox.collides_with_circle(circle_center, radius));
}

TEST_F(TestRectHitbox, CollidesWithCircle_NoCollision) {
    Vector2D circle_center(20, 20);
    float radius = 1.0f;
    EXPECT_FALSE(not_rotated_rect_hitbox.collides_with_circle(circle_center, radius));
    EXPECT_FALSE(rotated_rect_hitbox.collides_with_circle(circle_center, radius));
}

TEST_F(TestRectHitbox, IsHit_RayHits) {
    Vector2D ray_start(0, 10);
    Vector2D ray_dir = Vector2D(1, 0).normalized(PhysicsConfig::meter_size);
    EXPECT_TRUE(not_rotated_rect_hitbox.get_hit_pos(ray_start, ray_dir).has_value());
    EXPECT_TRUE(rotated_rect_hitbox.get_hit_pos(ray_start, ray_dir).has_value());
}

TEST_F(TestRectHitbox, IsHit_RayDoesNotHit) {
    Vector2D ray_start(20, 11);
    Vector2D ray_dir = Vector2D(1, 0).normalized(PhysicsConfig::meter_size);
    EXPECT_FALSE(not_rotated_rect_hitbox.get_hit_pos(ray_start, ray_dir).has_value());
    EXPECT_FALSE(rotated_rect_hitbox.get_hit_pos(ray_start, ray_dir).has_value());
}

TEST_F(TestRectHitbox, IsHit_RayParallelDoesNotHit) {
    Vector2D ray_start(0, 5);
    Vector2D ray_dir = Vector2D(1, 0).normalized(PhysicsConfig::meter_size);
    EXPECT_FALSE(not_rotated_rect_hitbox.get_hit_pos(ray_start, ray_dir).has_value());
    EXPECT_FALSE(rotated_rect_hitbox.get_hit_pos(ray_start, ray_dir).has_value());
}

TEST_F(TestRectHitbox, IsInSameQuadrant_RayPointsToRects) {
    Vector2D ray_start(0, 12);
    Vector2D ray_dir = Vector2D(1, 0).normalized(PhysicsConfig::meter_size);
    EXPECT_TRUE(not_rotated_rect_hitbox.is_in_same_quadrant(ray_start, ray_dir));
    EXPECT_TRUE(rotated_rect_hitbox.is_in_same_quadrant(ray_start, ray_dir));
}

TEST_F(TestRectHitbox, IsInSameQuadrant_RayPointsAwayFromNotRotatedRect) {
    Vector2D ray_start(9, 10);
    Vector2D ray_dir = Vector2D(-1, 0).normalized(PhysicsConfig::meter_size);
    EXPECT_FALSE(not_rotated_rect_hitbox.is_in_same_quadrant(ray_start, ray_dir));
    EXPECT_TRUE(rotated_rect_hitbox.is_in_same_quadrant(ray_start, ray_dir));
}

TEST_F(TestRectHitbox, IsInSameQuadrant_RayFromBelowUp) {
    Vector2D ray_start(12, 16);
    Vector2D ray_dir = Vector2D(0, -1).normalized(PhysicsConfig::meter_size);
    EXPECT_TRUE(not_rotated_rect_hitbox.is_in_same_quadrant(ray_start, ray_dir));
    EXPECT_TRUE(rotated_rect_hitbox.is_in_same_quadrant(ray_start, ray_dir));
}

TEST_F(TestRectHitbox, IsInSameQuadrant_RayOppositeDir) {
    Vector2D ray_start(0, 0);
    Vector2D ray_dir = Vector2D(-1, -1).normalized(PhysicsConfig::meter_size);
    EXPECT_FALSE(not_rotated_rect_hitbox.is_in_same_quadrant(ray_start, ray_dir));
    EXPECT_FALSE(rotated_rect_hitbox.is_in_same_quadrant(ray_start, ray_dir));
}
