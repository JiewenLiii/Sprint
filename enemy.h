#ifndef ENEMY_H
#define ENEMY_H

#include "map.h"
#include <vector>
#include <string>

enum Difficulty {
    EASY = 0,
    HARD = 1
};

struct DifficultySettings {
    int enemyCount;
    int enemyHp;
    int enemyAttack;
};

class Enemy {
public:
    int x, y;
    int hp;
    int maxHp;
    int attack;
    int id;

    Enemy();
    Enemy(int startX, int startY, int enemyHp, int enemyAttack, int enemyId);

    bool moveTowardsPlayer(int playerX, int playerY, const Map& map,
                           const std::vector<Enemy>& enemies);
    bool randomMove(const Map& map, const std::vector<Enemy>& enemies);
    void takeDamage(int damage);
    bool isAlive() const;

    std::string getName() const { return "Enemy " + std::to_string(id + 1); }
    std::string getHealthBar() const;
    char getSymbol() const { return 'E'; }
    int getId() const { return id; }

private:
    static bool isEnemyAt(int x, int y, const std::vector<Enemy>& enemies, int excludeId);
};

DifficultySettings getDifficultySettings(Difficulty difficulty);
void spawnEnemies(const Map& map, std::vector<Enemy>& enemies, int count,
                  int hp, int attack, int playerX, int playerY);

#endif // ENEMY_H
