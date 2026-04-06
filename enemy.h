#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "map.h"
#include <vector>
#include <string>

/**
 * @brief 敌人类 - 继承 Entity 基类
 * 
 * 职责：
 * - 敌人 AI（追击/巡逻）
 * - 敌人生成逻辑
 * - 支持多态扩展（不同类型的敌人）
 */
class Enemy : public Entity {
public:
    Enemy();
    Enemy(int startX, int startY, int enemyHp, int enemyAttack, int enemyId);

    // AI 移动方法
    bool moveTowardsPlayer(int playerX, int playerY, const Map& map,
                           const std::vector<Enemy>& enemies);
    bool randomMove(const Map& map, const std::vector<Enemy>& enemies);

    // 实现 Entity 纯虚函数
    char getSymbol() const override { return 'E'; }
    std::string getName() const override { return "Enemy " + std::to_string(id + 1); }
    int getId() const { return id; }

    // 足迹管理
    void addFootprint(int x, int y);
    const std::vector<std::pair<int, int>>& getFootprints() const { return footprints; }
    void clearFootprints() { footprints.clear(); }

private:
    int id;  // 敌人唯一标识
    
    // 足迹记录
    std::vector<std::pair<int, int>> footprints;
    static const size_t MAX_FOOTPRINTS = 5;

    // 静态辅助方法
    static bool isEnemyAt(int x, int y, const std::vector<Enemy>& enemies, int excludeId);
};

// 难度设置
enum Difficulty {
    EASY = 0,
    HARD = 1
};

struct DifficultySettings {
    int enemyCount;
    int enemyHp;
    int enemyAttack;
};

DifficultySettings getDifficultySettings(Difficulty difficulty);
void spawnEnemies(const Map& map, std::vector<Enemy>& enemies, int count,
                  int hp, int attack, int playerX, int playerY);

#endif // ENEMY_H
