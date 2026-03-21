#ifndef ENEMY_H
#define ENEMY_H

#include "map.h"
#include <vector>

class Enemy {
public:
    int x, y;
    int hp;
    int attack;
    int maxHp;

    Enemy();
    Enemy(int startX, int startY, int enemyHp, int enemyAttack);

    bool moveTowardsPlayer(int playerX, int playerY, const Map& map);
    bool randomMove(const Map& map);
    void takeDamage(int damage);
    bool isAlive() const;
    void reset(int startX, int startY);
};

void spawnEnemies(const Map& map, std::vector<Enemy>& enemies, int count, int playerX, int playerY);

#endif // ENEMY_H
