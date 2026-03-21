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
    int dx = std::abs(x - playerX);
    int dy = std::abs(y - playerY);
    return (dx <= VIEW_RADIUS && dy <= VIEW_RADIUS);
}

bool Map::isExplored(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return false;
    return explored[y][x];
}

void Map::render(int playerX, int playerY, const int enemies[][2], int enemyCount,
                 const std::vector<std::pair<int,int>>& footprints,
                 const std::vector<std::pair<int,int>>& playerFootprints) const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    // 更新探索记录
    for (int dy = -VIEW_RADIUS; dy <= VIEW_RADIUS; ++dy) {
        for (int dx = -VIEW_RADIUS; dx <= VIEW_RADIUS; ++dx) {
            int x = playerX + dx;
            int y = playerY + dy;
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                explored[y][x] = true;
            }
        }
    }

    // 计算视口（更大的显示区域）
    int viewW = 31, viewH = 21;  // 增大显示区域
    int startX = std::max(0, playerX - viewW / 2);
    int startY = std::max(0, playerY - viewH / 2);
    int endX = std::min(WIDTH, startX + viewW);
    int endY = std::min(HEIGHT, startY + viewH);

    std::cout << "====== Dungeon Adventure (50x50) ======" << std::endl;
    std::cout << "Legend: @ = You, E = Enemy, # = Wall, . = Floor" << std::endl;
    std::cout << "        ? = Unknown, : = Explored, ' = EnemyFootprint, * = YourFootprint" << std::endl;
    std::cout << "Controls: W/A/S/D to move, Q to quit" << std::endl;
    std::cout << std::endl;

    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            bool visible = isVisible(playerX, playerY, x, y);
            bool isExploredTile = isExplored(x, y);

            // 未探索区域 - 完全不显示（黑色空格）
            if (!isExploredTile) {
                color(COLOR_BLACK);
                std::cout << ' ';  // 不显示任何符号
                resetColor();
                continue;
            }

            // 玩家位置
            if (x == playerX && y == playerY) {
                color(COLOR_BRIGHT_GREEN);
                std::cout << '@';
                resetColor();
                continue;
            }

            // 敌人位置（只在可见时显示）
            if (visible) {
                bool isEnemy = false;
                for (int i = 0; i < enemyCount; ++i) {
                    if (enemies[i][0] == x && enemies[i][1] == y) {
                        color(COLOR_RED);
                        std::cout << 'E';
                        resetColor();
                        isEnemy = true;
                        break;
                    }
                }
                if (isEnemy) continue;
            }

            // 足迹显示（只在可见区域）
            if (visible) {
                bool isFootprint = false;
                
                // 敌人足迹
                for (const auto& fp : footprints) {
                    if (fp.first == x && fp.second == y) {
                        color(COLOR_YELLOW);
                        std::cout << '\'';  // 敌人足迹符号
                        resetColor();
                        isFootprint = true;
                        break;
                    }
                }
                if (isFootprint) continue;

                // 玩家足迹
                for (const auto& fp : playerFootprints) {
                    if (fp.first == x && fp.second == y) {
                        color(COLOR_BRIGHT_GREEN);
                        std::cout << '*';  // 玩家足迹符号
                        resetColor();
                        isFootprint = true;
                        break;
                    }
                }
                if (isFootprint) continue;
            }

            // 显示地形
            if (visible) {
                if (grid[y][x] == '#') {
                    color(COLOR_BRIGHT_BLUE);
                    std::cout << '#';
                    resetColor();
                } else {
                    std::cout << '.';
                }
            } else {
                // 已探索但不可见 - 暗色显示
                color(COLOR_BLUE);
                if (grid[y][x] == '#') {
                    std::cout << '#';
                } else {
                    std::cout << ':';  // 暗色地板
                }
                resetColor();
            }
        }
        std::cout << std::endl;
    }
    
    std::cout << "Position: (" << playerX << ", " << playerY << ")" << std::endl;
}
