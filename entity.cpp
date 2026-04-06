#include "entity.h"
#include <sstream>

Entity::Entity(int x, int y, int hp, int attack)
    : x(x), y(y), hp(hp), maxHp(hp), attack(attack) {}

void Entity::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void Entity::setHp(int hp) {
    this->hp = hp;
    if (this->hp > maxHp) {
        this->hp = maxHp;
    }
    if (this->hp < 0) {
        this->hp = 0;
    }
}

void Entity::setAttack(int attack) {
    this->attack = attack;
}

void Entity::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

bool Entity::isAlive() const {
    return hp > 0;
}

std::string Entity::getHealthBar() const {
    std::ostringstream oss;
    oss << "HP: " << hp << "/" << maxHp;
    return oss.str();
}
