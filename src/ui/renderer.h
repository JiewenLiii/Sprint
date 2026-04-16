#ifndef RENDERER_H
#define RENDERER_H

#include "map.h"
#include "player.h"
#include "enemy.h"
#include <vector>

/**
 * @brief 渲染系统类 - 负责所有控制台渲染逻辑
 * 
 * 职责：
 * - 地图渲染（包括迷雾、足迹）
 * - 实体渲染（玩家、敌人）
 * - UI 元素渲染（边框、文字居中）
 * - 颜色管理
 */
class Renderer {
public:
    Renderer();

    /**
     * @brief 渲染完整游戏画面
     * @param map 地图对象
     * @param player 玩家对象
     * @param enemies 敌人列表
     */
    void renderGame(const Map& map, const Player& player, 
                    const std::vector<Enemy>& enemies) const;

    /**
     * @brief 渲染地图（包括迷雾、足迹、实体）
     * @param map 地图对象
     * @param player 玩家对象
     * @param enemies 敌人列表
     */
    void renderMap(const Map& map, const Player& player, 
                   const std::vector<Enemy>& enemies) const;

    /**
     * @brief 渲染玩家状态
     * @param player 玩家对象
     * @param aliveEnemies 存活敌人数量
     * @param totalEnemies 敌人总数
     * @param combatLog 战斗日志
     * @param enemies 敌人列表指针（可选，用于显示存活敌人）
     */
    void renderPlayerStatus(const Player& player, int aliveEnemies, int totalEnemies,
                            const std::vector<std::string>& combatLog,
                            const std::vector<Enemy>* enemies = nullptr) const;

    /**
     * @brief 渲染战斗画面
     * @param enemy 敌人对象
     * @param player 玩家对象
     * @param enemyFirst 敌人是否先手
     */
    void renderCombatHeader(const Enemy& enemy, const Player& player, bool enemyFirst) const;

    /**
     * @brief 绘制分隔线
     * @param width 宽度
     */
    void drawLine(int width = 50) const;

    /**
     * @brief 绘制居中文字
     * @param text 文字内容
     * @param width 宽度
     */
    void drawCenteredText(const std::string& text, int width = 50) const;

    /**
     * @brief 清屏
     */
    void clearScreen() const;

private:
    /**
     * @brief 渲染单个实体（玩家或敌人）
     * @param symbol 实体符号
     * @param colorIdx 颜色索引
     */
    void renderEntity(char symbol, int colorIdx) const;

    /**
     * @brief 渲染足迹
     * @param x, y 坐标
     * @param footprints 足迹列表
     * @param isPlayer 是否玩家足迹
     */
    bool renderFootprint(int x, int y, 
                         const std::vector<std::pair<int, int>>& footprints, 
                         bool isPlayer) const;

    /**
     * @brief 渲染敌人足迹
     * @param x, y 坐标
     * @param enemyFootprints 所有敌人的足迹
     */
    bool renderEnemyFootprint(int x, int y,
                              const std::vector<std::vector<std::pair<int, int>>>& enemyFootprints) const;
};

#endif // RENDERER_H
