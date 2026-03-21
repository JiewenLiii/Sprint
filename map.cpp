#include "map.h"
#include <iostream>

Map::Map() {
    generate();
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
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return false;
    }
    return grid[y][x] == '.';
}

char Map::getTile(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return '#';
    }
    return grid[y][x];
}

void Map::render(int playerX, int playerY, const int enemies[][2], int enemyCount) const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "=== Dungeon Adventure ===" << std::endl;
    std::cout << "Legend: @ = Player, E = Enemy, # = Wall, . = Floor" << std::endl;
    std::cout << "Controls: W/A/S/D to move, Q to quit" << std::endl;
    std::cout << std::endl;

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == playerX && y == playerY) {
                std::cout << '@';
                continue;
            }

            bool isEnemy = false;
            for (int i = 0; i < enemyCount; ++i) {
                if (enemies[i][0] == x && enemies[i][1] == y) {
                    std::cout << 'E';
                    isEnemy = true;
                    break;
                }
            }

            if (!isEnemy) {
                std::cout << grid[y][x];
            }
        }
        std::cout << std::endl;
    }
}
