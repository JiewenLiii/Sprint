#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "player.h"
#include "enemy.h"
#include <vector>
#include <string>

class Game {
public:
    Game();
    bool run();  // 返回 true 表示重玩

private:
    Map map;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<std::pair<int,int>> footprints;  // 敌人足迹
    std::vector<std::pair<int,int>> playerFootprints;  // 玩家足迹
    bool isRunning;
    bool isVictory;
    Difficulty difficulty;
    DifficultySettings settings;
    std::vector<std::string> combatLog;

    static const int CHASE_RADIUS = 5;

    void render();
    bool handleInput();
    void updateEnemies();
    void combat(size_t enemyIndex, bool enemyFirst = false);
    void showCombatLog(const std::string& message);
    void showGameOver();
    void checkGameOver();
    void showPlayerStatus();
    int countAliveEnemies() const;
    bool showStartMenu();
    bool showEndScreen();
    void clearScreen();
};

#endif // GAME_H
