#include "doctest.h"
#include "../world/map.h"
#include "../ui/colors.h"

// ============================================================
// 测试 2：Map 地图模块
// ============================================================

TEST_CASE("Map Dimensions") {
    CHECK(Map::WIDTH == 20);
    CHECK(Map::HEIGHT == 20);
    CHECK(Map::VIEW_RADIUS == 6);
}

TEST_CASE("Map Border Walls") {
    Map map;
    // 上边界
    for (int x = 0; x < Map::WIDTH; ++x) {
        CHECK(map.getTile(x, 0) == '#');
    }
    // 下边界
    for (int x = 0; x < Map::WIDTH; ++x) {
        CHECK(map.getTile(x, Map::HEIGHT - 1) == '#');
    }
    // 左边界
    for (int y = 0; y < Map::HEIGHT; ++y) {
        CHECK(map.getTile(0, y) == '#');
    }
    // 右边界
    for (int y = 0; y < Map::HEIGHT; ++y) {
        CHECK(map.getTile(Map::WIDTH - 1, y) == '#');
    }
}

TEST_CASE("Map Inner Floor") {
    Map map;
    CHECK(map.getTile(1, 1) == '.');
    CHECK(map.getTile(10, 10) == '.');
    CHECK(map.getTile(Map::WIDTH - 2, Map::HEIGHT - 2) == '.');
}

TEST_CASE("IsWalkable Floor") {
    Map map;
    CHECK(map.isWalkable(1, 1) == true);
    CHECK(map.isWalkable(10, 10) == true);
    CHECK(map.isWalkable(Map::WIDTH - 2, Map::HEIGHT - 2) == true);
}

TEST_CASE("IsWalkable Wall") {
    Map map;
    CHECK(map.isWalkable(0, 0) == false);
    CHECK(map.isWalkable(Map::WIDTH - 1, 0) == false);
    CHECK(map.isWalkable(0, Map::HEIGHT - 1) == false);
}

TEST_CASE("IsWalkable Out Of Bounds") {
    Map map;
    CHECK(map.isWalkable(-1, 0) == false);
    CHECK(map.isWalkable(0, -1) == false);
    CHECK(map.isWalkable(Map::WIDTH, 0) == false);
    CHECK(map.isWalkable(0, Map::HEIGHT) == false);
    CHECK(map.isWalkable(-100, -100) == false);
}

TEST_CASE("GetTile Out Of Bounds Returns Wall") {
    Map map;
    CHECK(map.getTile(-1, -1) == '#');
    CHECK(map.getTile(-5, 10) == '#');
    CHECK(map.getTile(10, -5) == '#');
    CHECK(map.getTile(Map::WIDTH, 0) == '#');
    CHECK(map.getTile(0, Map::HEIGHT) == '#');
}

TEST_CASE("IsVisible Center") {
    Map map;
    CHECK(map.isVisible(10, 10, 10, 10) == true);
}

TEST_CASE("IsVisible Within Radius") {
    Map map;
    CHECK(map.isVisible(10, 10, 11, 10) == true);   // 距离 1
    CHECK(map.isVisible(10, 10, 15, 10) == true);   // 距离 5
    CHECK(map.isVisible(10, 10, 16, 10) == true);   // 距离 6 = VIEW_RADIUS
}

TEST_CASE("IsVisible Beyond Radius") {
    Map map;
    CHECK(map.isVisible(10, 10, 17, 10) == false);  // 距离 7 > VIEW_RADIUS
    CHECK(map.isVisible(10, 10, 0, 0) == false);    // 距离 ~14
}

TEST_CASE("IsVisible Diagonal") {
    Map map;
    // (10+4, 10+4) 距离 = sqrt(32) ≈ 5.66 < 6
    CHECK(map.isVisible(10, 10, 14, 14) == true);
    // (10+5, 10+5) 距离 = sqrt(50) ≈ 7.07 > 6
    CHECK(map.isVisible(10, 10, 15, 15) == false);
}

TEST_CASE("UpdateExplored") {
    Map map;
    CHECK(map.isExplored(10, 10) == false);

    map.updateExplored(10, 10);

    CHECK(map.isExplored(10, 10) == true);
    CHECK(map.isExplored(11, 10) == true);
    CHECK(map.isExplored(16, 10) == true);   // 距离 6
    CHECK(map.isExplored(17, 10) == false);  // 距离 7
}

TEST_CASE("IsExplored Out Of Bounds") {
    Map map;
    CHECK(map.isExplored(-1, 0) == false);
    CHECK(map.isExplored(0, -1) == false);
    CHECK(map.isExplored(Map::WIDTH, 0) == false);
}
