#include "map.h"
#include "enemy.h"
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

void Map::render(int playerX, int playerY,
                 const std::vector<Enemy>& enemies,
                 const std::vector<std::vector<std::pair<int,int>>>& enemyFootprints,
                 const std::vector<std::pair<int,int>>& playerFootprints) const {
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

    // Top border
    std::cout << "  ";
    color(COLOR_BRIGHT_CYAN);
    std::cout << "+";
    for (int x = 0; x < WIDTH; ++x) std::cout << "-";
    std::cout << "+";
    resetColor();
    std::cout << std::endl;

    for (int y = 0; y < HEIGHT; ++y) {
        std::cout << "  ";
        color(COLOR_BRIGHT_CYAN);
        std::cout << "|";
        resetColor();

        for (int x = 0; x < WIDTH; ++x) {
            bool visible = isVisible(playerX, playerY, x, y);
            bool isExploredTile = isExplored(x, y);

            if (!isExploredTile) {
                color(COLOR_BLACK);
                std::cout << "?";
                resetColor();
                continue;
            }

            if (x == playerX && y == playerY) {
                color(COLOR_BRIGHT_GREEN);
                std::cout << '@';
                resetColor();
                continue;
            }

            if (visible) {
                bool isEnemy = false;
                for (const auto& enemy : enemies) {
                    if (enemy.isAlive() && enemy.x == x && enemy.y == y) {
                        int colorIdx = enemy.getId() % 4;
                        switch (colorIdx) {
                            case 0: color(COLOR_RED); break;
                            case 1: color(COLOR_BRIGHT_RED); break;
                            case 2: color(COLOR_MAGENTA); break;
                            case 3: color(COLOR_BRIGHT_MAGENTA); break;
                        }
                        std::cout << enemy.getSymbol();
                        resetColor();
                        isEnemy = true;
                        break;
                    }
                }
                if (isEnemy) continue;
            }

            if (visible) {
                bool isFootprint = false;

                // 敌人足迹 - 每个敌人有自己的足迹，显示最近 5 个，渐变效果
                for (size_t e = 0; e < enemyFootprints.size(); ++e) {
                    const auto& eFootprints = enemyFootprints[e];
                    for (size_t i = 0; i < eFootprints.size(); ++i) {
                        const auto& fp = eFootprints[i];
                        if (fp.first == x && fp.second == y) {
                            size_t age = eFootprints.size() - i;  // 1=最新，5=最老
                            // 渐变：亮黄 -> 黄（最老也是黄色，不再是黑色）
                            if (age <= 2) {
                                color(COLOR_BRIGHT_YELLOW);  // 最近的 2 个：亮黄色
                            } else {
                                color(COLOR_YELLOW);  // 其余：普通黄色（不再是黑色）
                            }
                            std::cout << ',';
                            resetColor();
                            isFootprint = true;
                            break;
                        }
                    }
                    if (isFootprint) break;
                }
                if (isFootprint) continue;

                // 玩家足迹 - 显示最近 5 个，渐变效果
                for (size_t i = 0; i < playerFootprints.size(); ++i) {
                    const auto& fp = playerFootprints[i];
                    if (fp.first == x && fp.second == y) {
                        size_t age = playerFootprints.size() - i;  // 1=最新，5=最老
                        // 渐变：亮绿 -> 绿（最老也是绿色，不再是黑色）
                        if (age <= 2) {
                            color(COLOR_BRIGHT_GREEN);  // 最近的 2 个：亮绿色
                        } else {
                            color(COLOR_GREEN);  // 其余：普通绿色（不再是黑色）
                        }
                        std::cout << '*';
                        resetColor();
                        isFootprint = true;
                        break;
                    }
                }
                if (isFootprint) continue;
            }

            if (visible) {
                if (grid[y][x] == '#') {
                    color(COLOR_BRIGHT_BLUE);
                    std::cout << '#';
                    resetColor();
                } else {
                    std::cout << '.';
                }
            } else {
                if (grid[y][x] == '.') {
                    color(COLOR_BLACK);
                    std::cout << ':';
                    resetColor();
                } else {
                    color(COLOR_BLUE);
                    std::cout << '#';
                    resetColor();
                }
            }
        }

        color(COLOR_BRIGHT_CYAN);
        std::cout << "|";
        resetColor();
        std::cout << std::endl;
    }

    // Bottom border
    std::cout << "  ";
    color(COLOR_BRIGHT_CYAN);
    std::cout << "+";
    for (int x = 0; x < WIDTH; ++x) std::cout << "-";
    std::cout << "+";
    resetColor();
    std::cout << std::endl;

    std::cout << "  Position: (" << playerX << ", " << playerY << ")";
}
