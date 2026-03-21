#include "player.h"
#include "enemy.h"

Player::Player() {
    reset();
}

void Player::reset() {
    x = 5;
    y = 5;
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

    // 检查是否有敌人在目标位置
    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.x == newX && enemy.y == newY) {
            return false;  // 不能走入敌人格子
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
