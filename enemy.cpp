#include "enemy.h"
#include <cstdlib>
#include <ctime>
#include <sstream>

Enemy::Enemy() : x(0), y(0), hp(10), maxHp(10), attack(3), id(0) {}

Enemy::Enemy(int startX, int startY, int enemyHp, int enemyAttack, int enemyId)
    : x(startX), y(startY), hp(enemyHp), maxHp(enemyHp), attack(enemyAttack), id(enemyId) {}

bool Enemy::isEnemyAt(int x, int y, const std::vector<Enemy>& enemies, int excludeId) {
    for (const auto& e : enemies) {
        if (e.isAlive() && e.id != excludeId && e.x == x && e.y == y) {
            return true;
        }
    }
    return false;
}

bool Enemy::moveTowardsPlayer(int playerX, int playerY, const Map& map, 
                               const std::vector<Enemy>& enemies) {
    int dx = 0, dy = 0;
    if (playerX > x) dx = 1;
    else if (playerX < x) dx = -1;
    if (playerY > y) dy = 1;
    else if (playerY < y) dy = -1;

    int moves[5][2] = {{dx, dy}, {dx, 0}, {0, dy}, {-dx, 0}, {0, -dy}};
    for (int i = 0; i < 5; ++i) {
        int newX = x + moves[i][0];
        int newY = y + moves[i][1];
        if (newX == playerX && newY == playerY) continue;
        if (map.isWalkable(newX, newY) && !isEnemyAt(newX, newY, enemies, id)) {
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
        x = newX;
        y = newY;
        return true;
    }
    return false;
}

void Enemy::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

bool Enemy::isAlive() const {
    return hp > 0;
}

DifficultySettings getDifficultySettings(Difficulty difficulty) {
    DifficultySettings settings;
    switch (difficulty) {
        case EASY:
            settings.enemyCount = 3;
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
    // 移除了 srand - 在 main 中已设置

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
                if (e.x == x && e.y == y) {
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
