#include <gtest/gtest.h>

#include "server/map/map.h"
#include "server/map/map_builder.h"

TEST(YamlTest, LoadMap) {
    YAML::Node data = YAML::LoadFile("../tests/server/map/map.yaml");

    ASSERT_TRUE(data["name"]);
    ASSERT_TRUE(data["tile_size"]);
    EXPECT_NO_THROW(data["name"].as<std::string>());
    EXPECT_NO_THROW(data["tile_size"].as<int>());
}
