#include <gtest/gtest.h>

#include "common/map/map.h"
#include "server/map/map_builder.h"

TEST(YamlTest, LoadMap) {
    YAML::Node data = YAML::LoadFile("./tests/server/map/map.yaml");

    ASSERT_TRUE(data["name"]);
    ASSERT_TRUE(data["max_players"]);
    ASSERT_TRUE(data["height"]);
    ASSERT_TRUE(data["width"]);
    ASSERT_TRUE(data["tiles"]);
}

TEST(YamlTest, BuildMap) {
    MapBuilder builder("./tests/server/map/map3.yaml");
    Map map = builder.build();

    EXPECT_EQ(map.get_name(), "test_map");
    EXPECT_EQ(map.get_max_players(), 10);
    EXPECT_EQ(map.get_height(), 5);
    EXPECT_EQ(map.get_width(), 5);
    EXPECT_EQ(map.get_tiles().size(), 5);
    EXPECT_EQ(map.get_tiles()[0].size(), 5);
    EXPECT_EQ(map.get_collidables().size(), 17);
    EXPECT_EQ(map.get_spawns_tts().size(), 1);
    EXPECT_EQ(map.get_spawns_cts().size(), 1);
    EXPECT_EQ(map.get_bomb_sites().size(), 1);
    EXPECT_EQ(map.get_guns().size(), 2);
}
