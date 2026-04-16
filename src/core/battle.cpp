#include "battle.h"
#include <algorithm>
#include <iostream>
#include "colors.h"

Battle::Battle() {}

int Battle::calculateDamage(int attackerAtk, int defenderDef) const {
    int damage = attackerAtk - defenderDef;
    return std::max(1, damage);
}

void Battle::addCombatLog(const std::string& message, std::vector<std::string>& combatLog) const {
    combatLog.push_back(message);
    // 保留最近 MAX_COMBAT_LOG_SIZE 条日志
    if (combatLog.size() > MAX_COMBAT_LOG_SIZE) {
        combatLog.erase(combatLog.begin());
    }
}

bool Battle::executeCombat(Player& player, Enemy& enemy, bool enemyFirst, 
                           std::vector<std::string>& combatLog) {
    // 回合制战斗 - 固定顺序，不交换
    while (enemy.isAlive() && player.isAlive()) {
        if (enemyFirst) {
            // 敌人先手：敌人攻击 → 玩家反击
            int enemyDamage = calculateDamage(enemy.getAttack());
            player.takeDamage(enemyDamage);

            color(COLOR_RED);
            std::cout << "  " << enemy.getName() << " attacks you for " << enemyDamage << " damage!" << std::endl;
            resetColor();
            addCombatLog(enemy.getName() + " hit you for " + std::to_string(enemyDamage), combatLog);

            if (!player.isAlive()) {
                color(COLOR_RED);
                std::cout << "  You were defeated..." << std::endl;
                resetColor();
                addCombatLog("You were defeated", combatLog);
                return false;  // 玩家失败
            }

            int playerDamage = calculateDamage(player.getAttack());
            enemy.takeDamage(playerDamage);

            color(COLOR_GREEN);
            std::cout << "  You attack " << enemy.getName() << " for " << playerDamage << " damage!" << std::endl;
            resetColor();
            addCombatLog("Hit " + enemy.getName() + " for " + std::to_string(playerDamage), combatLog);

            if (!enemy.isAlive()) {
                color(COLOR_BRIGHT_GREEN);
                std::cout << "  " << enemy.getName() << " defeated!" << std::endl;
                resetColor();
                addCombatLog(enemy.getName() + " defeated", combatLog);
                return true;  // 玩家胜利
            }
        } else {
            // 玩家先手：玩家攻击 → 敌人反击
            int playerDamage = calculateDamage(player.getAttack());
            enemy.takeDamage(playerDamage);

            color(COLOR_GREEN);
            std::cout << "  You attack " << enemy.getName() << " for " << playerDamage << " damage!" << std::endl;
            resetColor();
            addCombatLog("Hit " + enemy.getName() + " for " + std::to_string(playerDamage), combatLog);

            if (!enemy.isAlive()) {
                color(COLOR_BRIGHT_GREEN);
                std::cout << "  " << enemy.getName() << " defeated!" << std::endl;
                resetColor();
                addCombatLog(enemy.getName() + " defeated", combatLog);
                return true;  // 玩家胜利
            }

            int enemyDamage = calculateDamage(enemy.getAttack());
            player.takeDamage(enemyDamage);

            color(COLOR_RED);
            std::cout << "  " << enemy.getName() << " attacks you for " << enemyDamage << " damage!" << std::endl;
            resetColor();
            addCombatLog(enemy.getName() + " hit you for " + std::to_string(enemyDamage), combatLog);

            if (!player.isAlive()) {
                color(COLOR_RED);
                std::cout << "  You were defeated..." << std::endl;
                resetColor();
                addCombatLog("You were defeated", combatLog);
                return false;  // 玩家失败
            }
        }

        std::cout << std::endl;
        std::cout << "  " << enemy.getName() << ": " << enemy.getHealthBar() << std::endl;
        std::cout << "  You: " << player.getHealthBar() << std::endl;
        std::cout << std::endl;
    }

    // 理论上不会到达这里（循环内已有返回）
    return player.isAlive();
}
