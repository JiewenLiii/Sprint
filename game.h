#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "player.h"
#include "enemy.h"
#include <vector>
#include <string>
#include <chrono>
#include <thread>

enum GameResult {
    VICTORY = 0,
    DEFEAT = 1,
    QUIT = 2
};

class Game {
public:
    Game();
    bool run();  // 返回 true 表示返回主菜单，false 表示退出游戏

private:
    Map map;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<std::vector<std::pair<int,int>>> enemyFootprints;  // 每个敌人有自己的足迹
    std::vector<std::pair<int,int>> playerFootprints;  // 玩家足迹
    bool isRunning;
    GameResult result;
    Difficulty difficulty;
    DifficultySettings settings;
    std::vector<std::string> combatLog;

    static const int CHASE_RADIUS = 5;

    void render();
    bool handleInput();
    void updateEnemies();
    void combat(size_t enemyIndex, bool enemyFirst = false);
    void addCombatLog(const std::string& message);
    void checkGameOver();
    void showPlayerStatus();
    int countAliveEnemies() const;
    bool showMainMenu();
    bool showEndScreen();
    bool confirmQuit();
    void clearScreen();
    void drawLine(int width = 50);
    void drawCenteredText(const std::string& text, int width = 50);
    void initGame();
    int calculateDamage(int attackerAtk, int defenderDef) const;
    void drawHealthBarInline(int hp, int maxHp);
};

#endif // GAME_H
