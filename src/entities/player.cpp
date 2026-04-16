#include "player.h"
#include "enemy.h"
#include "map.h"
#include <sstream>

Player::Player() : Entity(0, 0, 20, 5) {
    reset();
}

void Player::reset() {
    // 重置位置到地图中心
    x = Map::WIDTH / 2;
    y = Map::HEIGHT / 2;
    // 重置生命值和攻击力
    hp = 20;
    maxHp = 20;
    attack = 5;
    // 清空足迹
    footprints.clear();
}

bool Player::move(int dx, int dy, const Map& map, const std::vector<Enemy>& enemies) {
    int newX = x + dx;
    int newY = y + dy;

    // 检查地图边界和墙壁
    if (!map.isWalkable(newX, newY)) {
        return false;
    }

    // 检查敌人碰撞
    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getX() == newX && enemy.getY() == newY) {
            return false;
        }
    }

    // 记录移动前的位置作为足迹
    addFootprint(x, y);

    // 更新位置
    x = newX;
    y = newY;
    return true;
}

void Player::addFootprint(int x, int y) {
    footprints.push_back({x, y});
    // 保留最近 MAX_FOOTPRINTS 个足迹
    if (footprints.size() > MAX_FOOTPRINTS) {
        footprints.erase(footprints.begin());
    }
}

std::string Player::getStatus() const {
    std::ostringstream oss;
    oss << "HP: " << hp << "/" << maxHp << " | ATK: " << attack;
    return oss.str();
}
