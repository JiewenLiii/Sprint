#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "map.h"

class Enemy;

class Player {
public:
    int x, y;
    int hp;
    int maxHp;
    int attack;

    Player();

    bool move(int dx, int dy, const Map& map, const std::vector<Enemy>& enemies);
    void takeDamage(int damage);
    bool isAlive() const;
    void reset();
    std::string getStatus() const;
    std::string getHealthBar() const;
};

#endif // PLAYER_H
