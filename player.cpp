#include "player.h"
#include "enemy.h"
#include "map.h"
#include <sstream>

Player::Player() {
    reset();
}

void Player::reset() {
    x = Map::WIDTH / 2;
    y = Map::HEIGHT / 2;
    hp = 20;
    maxHp = 20;
    attack = 5;
}

bool Player::move(int dx, int dy, const Map& map, const std::vector<Enemy>& enemies) {
    int newX = x + dx;
    int newY = y + dy;

    if (!map.isWalkable(newX, newY)) {
        return false;
    }

    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.x == newX && enemy.y == newY) {
            return false;
        }
    }

    x = newX;
    y = newY;
    return true;
}

void Player::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

bool Player::isAlive() const {
    return hp > 0;
}

std::string Player::getStatus() const {
    std::ostringstream oss;
    oss << "HP: " << hp << "/" << maxHp << " | ATK: " << attack;
    return oss.str();
}

std::string Player::getHealthBar() const {
    std::ostringstream oss;
    oss << "HP: " << hp << "/" << maxHp;
    return oss.str();
}
