#include <gtest/gtest.h>

#include "server/map/map.h"
#include "server/map/map_builder.h"

class TestMap: public ::testing::Test {
protected:
    Map map;

    TestMap(): map(MapBuilder("map.yaml").build()) {}
};
