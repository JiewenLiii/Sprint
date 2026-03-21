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
    void run();

private:
    Map map;
    Player player;
    std::vector<Enemy> enemies;
    bool isRunning;
    bool isVictory;

    static const int ENEMY_COUNT = 3;

    void render();
    bool handleInput();
    void updateEnemies();
    void combat(size_t enemyIndex);
    void showCombatLog(const std::string& message);
    void showGameOver();
    void checkGameOver();
    void showPlayerStatus();
    int countAliveEnemies() const;
};

#endif // GAME_H
