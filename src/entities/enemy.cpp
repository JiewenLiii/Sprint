#include "enemy.h"
#include <cstdlib>
#include <ctime>
#include <sstream>

Enemy::Enemy() : Entity(0, 0, 10, 3), id(0) {}

Enemy::Enemy(int startX, int startY, int enemyHp, int enemyAttack, int enemyId)
    : Entity(startX, startY, enemyHp, enemyAttack), id(enemyId) {}

bool Enemy::isEnemyAt(int x, int y, const std::vector<Enemy>& enemies, int excludeId) {
    for (const auto& e : enemies) {
        if (e.isAlive() && e.getId() != excludeId && e.getX() == x && e.getY() == y) {
            return true;
        }
    }
    return false;
}

bool Enemy::moveTowardsPlayer(int playerX, int playerY, const Map& map,
                               const std::vector<Enemy>& enemies) {
    // 只允许上下左右移动（4 方向）
    int moves[4][2] = {
        {0, -1},  // 上
        {0, 1},   // 下
        {-1, 0},  // 左
        {1, 0}    // 右
    };

    // 优先选择靠近玩家的方向
    for (int i = 0; i < 4; ++i) {
        int newX = x + moves[i][0];
        int newY = y + moves[i][1];

        // 检查是否更接近玩家
        int oldDist = std::abs(playerX - x) + std::abs(playerY - y);
        int newDist = std::abs(playerX - newX) + std::abs(playerY - newY);

        if (newDist < oldDist && map.isWalkable(newX, newY) && !isEnemyAt(newX, newY, enemies, id)) {
            // 记录移动前的位置作为足迹
            addFootprint(x, y);
            
            x = newX;
            y = newY;
            return true;
        }
    }
    return false;
}

bool Enemy::randomMove(const Map& map, const std::vector<Enemy>& enemies) {
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    int dir = std::rand() % 4;
    int newX = x + directions[dir][0];
    int newY = y + directions[dir][1];
    if (map.isWalkable(newX, newY) && !isEnemyAt(newX, newY, enemies, id)) {
        // 记录移动前的位置作为足迹
        addFootprint(x, y);
        
        x = newX;
        y = newY;
        return true;
    }
    return false;
}

void Enemy::addFootprint(int x, int y) {
    footprints.push_back({x, y});
    // 保留最近 MAX_FOOTPRINTS 个足迹
    if (footprints.size() > MAX_FOOTPRINTS) {
        footprints.erase(footprints.begin());
    }
}

DifficultySettings getDifficultySettings(Difficulty difficulty) {
    DifficultySettings settings;
    switch (difficulty) {
        case EASY:
            settings.enemyCount = 4;
            settings.enemyHp = 10;
            settings.enemyAttack = 3;
            break;
        case HARD:
            settings.enemyCount = 8;
            settings.enemyHp = 15;
            settings.enemyAttack = 6;
            break;
    }
    return settings;
}

void spawnEnemies(const Map& map, std::vector<Enemy>& enemies, int count,
                  int hp, int attack, int playerX, int playerY) {
    enemies.clear();

    int spawned = 0;
    int attempts = 0;
    const int maxAttempts = 2000;
    int enemyId = 0;

    while (spawned < count && attempts < maxAttempts) {
        int x = std::rand() % (Map::WIDTH - 2) + 1;
        int y = std::rand() % (Map::HEIGHT - 2) + 1;

        if (std::abs(x - playerX) < 5 && std::abs(y - playerY) < 5) {
            attempts++;
            continue;
        }

        if (map.isWalkable(x, y)) {
            bool occupied = false;
            for (const auto& e : enemies) {
                if (e.getX() == x && e.getY() == y) {
                    occupied = true;
                    break;
                }
            }
            if (!occupied) {
                enemies.emplace_back(x, y, hp, attack, enemyId++);
                spawned++;
            }
        }
        attempts++;
    }
}
