#include "map.h"
#include "colors.h"
#include <iostream>
#include <algorithm>
#include <cmath>

const int Map::WIDTH;
const int Map::HEIGHT;
const int Map::VIEW_RADIUS;

Map::Map() {
    generate();
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            explored[y][x] = false;
        }
    }
}

void Map::generate() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                grid[y][x] = '#';
            } else {
                grid[y][x] = '.';
            }
        }
    }
}

bool Map::isWalkable(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return false;
    return grid[y][x] == '.';
}

char Map::getTile(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return '#';
    return grid[y][x];
}

bool Map::isVisible(int playerX, int playerY, int x, int y) const {
    int dx = x - playerX;
    int dy = y - playerY;
    // 欧几里得距离，圆形视野
    return (dx*dx + dy*dy <= VIEW_RADIUS * VIEW_RADIUS);
}

bool Map::isExplored(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return false;
    return explored[y][x];
}

void Map::updateExplored(int playerX, int playerY) const {
    // Update explored tiles - 圆形视野
    for (int dy = -VIEW_RADIUS; dy <= VIEW_RADIUS; ++dy) {
        for (int dx = -VIEW_RADIUS; dx <= VIEW_RADIUS; ++dx) {
            int x = playerX + dx;
            int y = playerY + dy;
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                // 欧几里得距离，圆形探索区域
                if (dx*dx + dy*dy <= VIEW_RADIUS * VIEW_RADIUS) {
                    explored[y][x] = true;
                }
            }
        }
    }
}
