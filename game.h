#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "player.h"
#include "enemy.h"
#include "battle.h"
#include "renderer.h"
#include "game_types.h"
#include <vector>
#include <string>
#include <chrono>
#include <thread>

/**
 * @brief 游戏主控制类 - 协调各模块，实现游戏流程
 * 
 * 职责：
 * - 游戏主循环控制
 * - 菜单系统
 * - 游戏初始化
 * - 输入处理
 * - 游戏状态管理
 * 
 * 注意：战斗、渲染等职责已委托给专门的类
 */
class Game {
public:
    Game();
    bool run();  // 返回 true 表示返回主菜单，false 表示退出游戏

private:
    // 组合其他模块（低耦合设计）
    Map map;                    // 地图模块
    Player player;              // 玩家模块
    std::vector<Enemy> enemies; // 敌人模块
    Battle battle;              // 战斗模块（委托）
    Renderer renderer;          // 渲染模块（委托）
    
    // 游戏状态
    bool isRunning;
    GameResult result;
    Difficulty difficulty;
    DifficultySettings settings;
    
    // 战斗日志
    std::vector<std::string> combatLog;

    // 敌人追击半径
    static const int CHASE_RADIUS = 5;

    // 游戏流程控制
    void initGame();
    void render();  // 委托给 Renderer
    bool handleInput();
    void updateEnemies();
    void checkGameOver();
    
    // 菜单系统
    bool showMainMenu();
    bool showEndScreen();
    bool confirmQuit();
    
    // 辅助方法
    int countAliveEnemies() const;
    void showPlayerStatus();
    void combat(size_t enemyIndex, bool enemyFirst = false);
};

#endif // GAME_H
