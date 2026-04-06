#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "map.h"
#include <vector>

class Enemy;

/**
 * @brief 玩家类 - 继承 Entity 基类
 * 
 * 职责：
 * - 玩家移动逻辑
 * - 玩家状态管理
 * - 碰撞检测（地图、敌人）
 */
class Player : public Entity {
public:
    Player();

    // 移动方法 - 需要地图和敌人信息进行碰撞检测
    bool move(int dx, int dy, const Map& map, const std::vector<Enemy>& enemies);
    
    // 重置玩家状态
    void reset();
    
    // 获取玩家状态信息
    std::string getStatus() const;

    // 实现 Entity 纯虚函数
    char getSymbol() const override { return '@'; }
    std::string getName() const override { return "Player"; }

private:
    // 足迹记录
    std::vector<std::pair<int, int>> footprints;
    static const size_t MAX_FOOTPRINTS = 5;

public:
    // 足迹管理
    void addFootprint(int x, int y);
    const std::vector<std::pair<int, int>>& getFootprints() const { return footprints; }
    void clearFootprints() { footprints.clear(); }
};

#endif // PLAYER_H
