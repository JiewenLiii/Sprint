#ifndef BATTLE_H
#define BATTLE_H

#include "player.h"
#include "enemy.h"
#include <vector>
#include <string>

/**
 * @brief 战斗系统类 - 负责所有战斗逻辑
 * 
 * 职责：
 * - 回合制战斗计算
 * - 伤害计算
 * - 战斗日志管理
 * - 先手判定
 */
class Battle {
public:
    Battle();

    /**
     * @brief 执行回合制战斗
     * @param player 玩家对象
     * @param enemy 敌人对象
     * @param enemyFirst 敌人是否先手
     * @param combatLog 战斗日志（引用传递，用于记录）
     * @return true 玩家胜利，false 玩家失败
     */
    bool executeCombat(Player& player, Enemy& enemy, bool enemyFirst, 
                       std::vector<std::string>& combatLog);

    /**
     * @brief 计算伤害
     * @param attackerAtk 攻击力
     * @param defenderDef 防御力（当前固定为 0）
     * @return 伤害值（最小为 1）
     */
    int calculateDamage(int attackerAtk, int defenderDef = 0) const;

    /**
     * @brief 添加战斗日志
     * @param message 日志消息
     * @param combatLog 战斗日志向量
     */
    void addCombatLog(const std::string& message, std::vector<std::string>& combatLog) const;

private:
    static const size_t MAX_COMBAT_LOG_SIZE = 10;  // 最大战斗日志条数
};

#endif // BATTLE_H
