#ifndef PLAYER_H
#define PLAYER_H

class Map;

class Player {
public:
    int x, y;
    int hp;
    int attack;
    int maxHp;

    Player();

    bool move(int dx, int dy, const Map& map);
    void takeDamage(int damage);
    bool isAlive() const;
    void reset();
};

#endif // PLAYER_H
