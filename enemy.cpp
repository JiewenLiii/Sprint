#include "enemy.h"
#include <cstdlib>
#include <ctime>

Enemy::Enemy() : x(0), y(0), hp(10), attack(3), maxHp(10) {}

Enemy::Enemy(int startX, int startY, int enemyHp, int enemyAttack)
    : x(startX), y(startY), hp(enemyHp), attack(enemyAttack), maxHp(enemyHp) {}

void Enemy::reset(int startX, int startY) {
    x = startX;
    y = startY;
    hp = maxHp;
}

bool Enemy::moveTowardsPlayer(int playerX, int playerY, const Map& map) {
    int dx = 0, dy = 0;

    if (playerX > x) dx = 1;
    else if (playerX < x) dx = -1;
    
    if (playerY > y) dy = 1;
    else if (playerY < y) dy = -1;

    int moves[5][2] = {
        {dx, dy},
        {dx, 0},
        {0, dy},
        {-dx, 0},
        {0, -dy}
    };

    for (int i = 0; i < 5; ++i) {
        int newX = x + moves[i][0];
        int newY = y + moves[i][1];

        if (newX == playerX && newY == playerY) {
            continue;
        }

        if (map.isWalkable(newX, newY)) {
            x = newX;
            y = newY;
            return true;
        }
    }

    return false;
}

bool Enemy::randomMove(const Map& map) {
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    int dir = std::rand() % 4;
    int newX = x + directions[dir][0];
    int newY = y + directions[dir][1];

    if (map.isWalkable(newX, newY)) {
        x = newX;
        y = newY;
        return true;
    }

    return false;
}

void Enemy::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) {
        hp = 0;
    }
}

bool Enemy::isAlive() const {
    return hp > 0;
}

void spawnEnemies(const Map& map, std::vector<Enemy>& enemies, int count, int playerX, int playerY) {
    enemies.clear();

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int spawned = 0;
    int attempts = 0;
    const int maxAttempts = 1000;

    while (spawned < count && attempts < maxAttempts) {
        int x = std::rand() % (Map::WIDTH - 2) + 1;
        int y = std::rand() % (Map::HEIGHT - 2) + 1;

        if (map.isWalkable(x, y) && !(x == playerX && y == playerY)) {
            bool occupied = false;
            for (const auto& enemy : enemies) {
                if (enemy.x == x && enemy.y == y) {
                    occupied = true;
                    break;
                }
            }

            if (!occupied) {
                enemies.emplace_back(x, y, 10, 3);
                spawned++;
            }
        }

        attempts++;
    }
}
