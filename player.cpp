#include "player.h"
#include "map.h"

Player::Player() {
    reset();
}

void Player::reset() {
    x = 5;
    y = 5;
    hp = 20;
    attack = 5;
    maxHp = 20;
}

bool Player::move(int dx, int dy, const Map& map) {
    int newX = x + dx;
    int newY = y + dy;

    if (map.isWalkable(newX, newY)) {
        x = newX;
        y = newY;
        return true;
    }
    return false;
}

void Player::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) {
        hp = 0;
    }
}

bool Player::isAlive() const {
    return hp > 0;
}
